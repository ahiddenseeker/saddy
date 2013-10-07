#include "p2d/body.h"
#include "p2d/world.h"
#include "p2d/circle.h"
#include "p2d/line.h"

DECLARE_SOBJ(sad::p2d::Body);


double sad::p2d::Body::timeStep() const
{
	return TimeStep;
}

void sad::p2d::Body::notifyRotate(const double & delta)
{
	m_current->rotate(delta);
}

void sad::p2d::Body::notifyMove(const sad::p2d::Vector & delta)
{
	m_current->move(delta);
}

void sad::p2d::Body::setUserObject(sad::Object * o)
{
	m_user_object = o;
}


sad::Object * sad::p2d::Body::userObject() const
{
	return m_user_object;
}

const sad::String & sad::p2d::Body::userType() const
{
	if (m_user_object == NULL)
	{
		return this->metaData()->name();
	}
	return m_user_object->metaData()->name();
}

sad::p2d::CollisionShape & sad::p2d::Body::at(double time, int index) const
{
	sad::p2d::Body * me = const_cast<sad::p2d::Body *>(this);

	// Light optimization, because most of our collision shapes are POD-like structures
	// We can reduce amount of allocations, using untyped copying instead all of high-level
	// operations
	memcpy(me->Temporary + index, me->m_current, me->m_shapesize);

	(me->Temporary + index)->move(m_tangential->positionDelta(time, me->TimeStep));
	(me->Temporary + index)->rotate(m_angular->positionDelta(time, me->TimeStep));
	return *(me->Temporary + index);
}

void sad::p2d::Body::stepDiscreteChangingValues(double time)
{
	m_ghost->step();
	m_tangential->forces().step(time);
	m_angular->forces().step(time);
}

void sad::p2d::Body::stepPositionsAndVelocities(double time)
{
	m_tangential->step(time, this->timeStep());
	m_angular->step(time, this->timeStep());
}

void sad::p2d::Body::trySetTransformer()
{
	if (m_world)
	{
		if (this->m_current->metaData()->canBeCastedTo("p2d::Circle"))
		{
			p2d::Circle * c = hst::checked_cast<p2d::Circle>(m_current);
			c->setTransformer(m_world->transformer());
		}
	}
}

sad::p2d::Body::Body()
{
	m_weight = new p2d::Weight();
	m_ghost = new p2d::GhostOptionsFlow();
	m_world = NULL;
	m_user_object = NULL;
	
	m_tangential = new p2d::TangentialMovement();
	m_tangential->addListener( new move_t(this, &p2d::Body::notifyMove) );
	m_tangential->setWeight(m_weight);

	m_angular = new p2d::AngularMovement();
	m_angular->addListener( new rotate_t(this, &p2d::Body::notifyRotate) );
	m_angular->setWeight(m_weight);

	p2d::Line * l = new p2d::Line();
	l->setCutter(p2d::cutter(0,0,0,0));
	m_current =  l;

	Temporary = NULL;
	m_lastsampleindex = -1;
	m_samples_are_cached = false;

	m_fixed = false;
	this->TimeStep = 0.0;
}

sad::p2d::Body::~Body()
{
	delete m_weight;
	delete m_tangential;
	delete m_angular;
	delete m_current;
	delete[] Temporary;
}

void sad::p2d::Body::setWeight(sad::p2d::Weight * weight)
{
	delete m_weight;
	m_weight = weight;
	m_tangential->setWeight(m_weight);
	m_angular->setWeight(m_weight);
}

void sad::p2d::Body::setWeight(const sad::p2d::Weight & weight)
{
	*m_weight = weight;
}

const sad::p2d::Weight & sad::p2d::Body::weight() const
{
	return *m_weight;
}

void sad::p2d::Body::setCurrentGO(sad::p2d::GhostOptions * ghost)
{
	m_ghost->setCurrent(ghost);
}

void sad::p2d::Body::sheduleGO(sad::p2d::GhostOptions *next)
{
	m_ghost->push(next);	
}

bool sad::p2d::Body::isGhost() const
{
	return m_ghost->value();
}

void sad::p2d::Body::setWorld(sad::p2d::World * world)
{
	m_world = world;
	this->trySetTransformer();
}

sad::p2d::World * sad::p2d::Body::world()
{
	return m_world;
}

void sad::p2d::Body::setShape(sad::p2d::CollisionShape * shape)
{
	delete m_current;
	m_current = shape;
	this->trySetTransformer();
	m_current->move(this->m_tangential->position());
	m_current->rotate(this->m_angular->position());
	m_shapesize = m_current->sizeOfType();

	delete Temporary;
	Temporary = NULL;
	if (m_lastsampleindex > -1)
		Temporary = m_current->clone(m_lastsampleindex + 1);	
}


void sad::p2d::Body::setCurrentPosition(const sad::p2d::Point & p)
{
	m_tangential->setCurrentPosition(p);
	buildCaches();
}

void sad::p2d::Body::shedulePosition(const sad::p2d::Point & p)
{
	m_tangential->setNextPosition(p);
	buildCaches();
}

void sad::p2d::Body::shedulePositionAt(const sad::p2d::Point & p, double time)
{
	m_tangential->setNextPositionAt(p, time);
	buildCaches();
}


const sad::p2d::Vector & sad::p2d::Body::position() const
{
	return m_tangential->position();
}

bool sad::p2d::Body::willPositionChange() const
{
	return m_tangential->willPositionChange();
}

sad::p2d::Vector sad::p2d::Body::nextPosition() const
{
	return m_tangential->nextPosition();
}

void sad::p2d::Body::setCurrentTangentialVelocity(const p2d::Vector & v)
{
	m_tangential->setCurrentVelocity(v);
	buildCaches();
}

void sad::p2d::Body::sheduleTangentialVelocity(const p2d::Vector & v)
{
	m_tangential->setNextVelocity(v);
}

void sad::p2d::Body::sheduleTangentialVelocityAt(const p2d::Vector & v, double time)
{
	m_tangential->setNextVelocityAt(v, time);
}

const sad::p2d::Vector & sad::p2d::Body::tangentialVelocity() const
{
	return m_tangential->velocity();
}

bool sad::p2d::Body::willTangentialVelocityChange() const
{
	return m_tangential->willVelocityChange();
}

sad::p2d::Vector sad::p2d::Body::nextTangentialVelocity() const
{
	return m_tangential->nextVelocity();
}

void sad::p2d::Body::setCurrentAngle(double angle)
{
	m_angular->setCurrentPosition(angle);
	buildCaches();
}

void sad::p2d::Body::sheduleAngle(double angle)
{
	m_angular->setNextPosition(angle);	
	buildCaches();
}

void sad::p2d::Body::sheduleAngleAt(double angle, double time)
{
	m_angular->setNextPositionAt(angle, time);
	buildCaches();
}

double sad::p2d::Body::angle() const
{
	return m_angular->position();
}

bool sad::p2d::Body::willAngleChange() const
{
	return m_angular->willPositionChange();
}

double sad::p2d::Body::nextAngle() const
{
	return m_angular->nextPosition();
}

void sad::p2d::Body::setCurrentAngularVelocity(double v)
{
	m_angular->setCurrentVelocity(v);
	buildCaches();
}

void sad::p2d::Body::sheduleAngularVelocity(double v)
{
	m_angular->setNextVelocity(v);
}

void sad::p2d::Body::sheduleAngularVelocityAt(double v, double time)
{
	m_angular->setNextVelocityAt(v, time);
}

double sad::p2d::Body::angularVelocity() const
{
	return m_angular->velocity();
}

double sad::p2d::Body::angularVelocityAt(double time) const
{
	return m_angular->velocityAt(time, this->timeStep());
}

bool sad::p2d::Body::willAngularVelocityChange() const
{
	return m_angular->willVelocityChange();
}

double sad::p2d::Body::nextAngularVelocity() const
{
	return m_angular->nextVelocity();
}

void sad::p2d::Body::move(const p2d::Vector & v)
{
	return m_tangential->setCurrentPosition(m_tangential->position() + v);
}


void sad::p2d::Body::rotate(double delta)
{
	return m_angular->setCurrentPosition(m_angular->position() + delta);
}


sad::p2d::CollisionShape * sad::p2d::Body::currentShape()
{
	return m_current;
}


void sad::p2d::Body::clearListeners()
{
	m_tangential->clearListeners();
	m_angular->clearListeners();
}

sad::p2d::TangentialActingForces & sad::p2d::Body::tangentialForces()
{
	return m_tangential->forces();
}

sad::p2d::AngularActingForces & sad::p2d::Body::angularForces()
{
	return m_angular->forces();
}

sad::p2d::Vector sad::p2d::Body::averageChangeIndependentTangentialVelocity()
{
	return m_tangential->averageChangeIndependentVelocityPer(this->timeStep());
}

sad::p2d::Vector sad::p2d::Body::tangentialVelocityAt(double time)
{
	return m_tangential->velocityAt(time, this->timeStep());
}

void sad::p2d::Body::buildCaches()
{
	m_tangential->cacheAcceleration();
	m_angular->cacheAcceleration();
	double t = this->TimeStep;
	int k = m_lastsampleindex + 1;
	double slice = t / k;
	for(int i = 0; i < k; i++)
	{
		// Saves inner data, using at. After that, caches results can be used by 
		// any kind of detector to build data
		this->at(t * (i+1), i );
	}
}


sad::p2d::Weight & sad::p2d::Body::weight()
{
	return *m_weight;
}


void sad::p2d::Body::correctPosition(const sad::p2d::Vector & distance)
{
	if (this->willPositionChange())
	{
		sad::p2d::Vector position = distance;
		position += this->nextPosition() - this->position();
		position /= 2.0;
		this->shedulePosition(this->position() + position);
	}	
	else
	{
		sad::p2d::Vector position = this->position();
		position += distance;
		this->shedulePosition(position);
	}
}

void sad::p2d::Body::correctTangentialVelocity(const p2d::Vector & v)
{
	if (this->willTangentialVelocityChange())
	{
		// Merge two impulses into one
		sad::p2d::Vector impulse = this->nextTangentialVelocity();
		impulse -= this->tangentialVelocity();

		impulse += v - this->tangentialVelocity();
			
		impulse /= 2.0;
		// Here sum of speeds is computed
		impulse +=  this->tangentialVelocity();
		this->sheduleTangentialVelocity(impulse);
	}
	else
	{
		this->sheduleTangentialVelocity(v);
	}
}


void sad::p2d::Body::setSamplingCount(int samples)
{	
	delete Temporary;
	Temporary = m_current->clone(samples);
	m_lastsampleindex = samples - 1;
}




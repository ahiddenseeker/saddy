#include <p2d/bouncesolver.h>
#include <log/log.h>

p2d::BounceSolver::BounceSolver()
{
	m_find = new p2d::FindContactPoints();
	m_first = NULL;
	m_second = NULL;
	m_debug = false;
	this->resetCoefficients();
}

p2d::BounceSolver::~BounceSolver()
{	
	delete m_find;
}


void p2d::BounceSolver::bounce(p2d::Body * b1, p2d::Body * b2)
{
	m_first = b1;
	m_second = b2;
	p2d::SetOfPointsPair pairs;
	this->solveTOIFCP(pairs);
	if (pairs.size() > 0 &&  (m_toi > 0 || is_fuzzy_zero(m_toi, COLLISION_PRECISION * 1000)))
	{
		this->performBouncing(pairs);
		this->resetCoefficients();
	}
	else
	{
		const char * reason = "Can't find pairs";
		if (pairs.size() > 0)
			reason = "TOI is negative";
		logFCPError(reason);
	}
}


void p2d::BounceSolver::solveTOIFCP(p2d::SetOfPointsPair & pairs)
{
	m_av1 = m_first->averageChangeIndependentTangentialVelocity();
	m_av2 = m_second->averageChangeIndependentTangentialVelocity();

	pairs = m_find->invoke(m_first->currentShape(),   m_av1,
						   m_second->currentShape(),  m_av2
						   );
	if (pairs.size()  == 2)
	{
		pairs[0].set1((pairs[0].p1() + pairs[1].p1()) / 2.0);
		pairs[0].set2((pairs[0].p2() + pairs[1].p2()) / 2.0);
		pairs.removeAt(1);
		m_shouldperformrotationfriction = false;
	}
	
	if (pairs.size() > 0)
	{
		// Compute time of impact
		double x1 = pairs[0].p1().x();
		double y1 = pairs[0].p1().y();
		double x2 = pairs[0].p2().x();
		double y2 = pairs[0].p2().y();

		double avx1 = m_av1.x();
		double avy1 = m_av1.y();
		double avx2 = m_av2.x();
		double avy2 = m_av2.y();

		double time = 0;
		if (non_fuzzy_zero(avx2 - avx1))
		{
			time = (x1 - x2) / (avx2 - avx1);
		}
		else
		{
			time = (y1 - y2) / (avy2 - avy1);
		}
				
		time -= COLLISION_PRECISION;
		m_toi = time;
	}
}

/*! Implement solving for bouncing
	\param[in] pairs pairs of contact points
 */
void p2d::BounceSolver::performBouncing(const p2d::SetOfPointsPair & pairs)
{
	//TODO: Implement another types of friction

	double m1 = m_first->weight().value();
	double m2 = m_second->weight().value();

	p2d::Point normal1; 
	m_first->currentShape()->normalToPointOnSurface(pairs[0].p1(), normal1);
	double pivot1 = m_first->currentShape()->center().distanceTo(pairs[0].p1());
	m_force_moment[0] = (pairs[0].p1() - m_first->currentShape()->center());

	p2d::Point normal2;
	m_second->currentShape()->normalToPointOnSurface(pairs[0].p2(), normal2);
	double pivot2 = m_first->currentShape()->center().distanceTo(pairs[0].p2());
	m_force_moment[1] = (pairs[0].p2() - m_second->currentShape()->center());

	p2d::Vector v1 = m_first->tangentialVelocityAt(m_toi);
	double project1 = p2d::scalar(v1, normal1);
	p2d::Vector normalPart1 = normal1;
	normalPart1 *= project1;
	p2d::Vector cachedNormal1 = normalPart1;

	p2d::Vector tangentialPart1 = v1;
	tangentialPart1 -= normalPart1;

	p2d::Vector v2 = m_second->tangentialVelocityAt(m_toi);
	double project2 = p2d::scalar(v2, normal2);
	p2d::Vector normalPart2 = normal2;
	normalPart2 *= project2;
	p2d::Vector cachedNormal2 = normalPart2;

	p2d::Vector tangentialPart2 = v2;
	tangentialPart2 -= normalPart2;

	this->resolveNormalSpeed(m_first, normalPart1, m_second, cachedNormal2, 0);		
	m_first->correctTangentialVelocity(normalPart1 + tangentialPart1);		
	m_first->correctPosition(m_av1 * m_toi);
	this->tryResolveFriction(m_first, tangentialPart1, normalPart1 - cachedNormal1, 0, pivot1);	
	this->resolveNormalSpeed(m_second, normalPart2, m_first, cachedNormal1, 1);		
	m_second->correctTangentialVelocity(normalPart2 + tangentialPart2);		
	m_second->correctPosition(m_av2 * m_toi);		
	this->tryResolveFriction(m_second, tangentialPart2, normalPart2 - cachedNormal2, 1, pivot2);	

}


void p2d::BounceSolver::logFCPError(const char * m)
{
	if (m_debug)
	{
	hst::string tpl = "Cannot find a contact points. Performing an object dump\n";
	tpl <<  "1st body: \n{0}\n";
	tpl <<  "velocity ({1},{2})\n";
	tpl <<  "2nd body: \n{3}\n";
	tpl <<  "velocity ({4},{5})\nTOI: {6} Reason: {7} \n";
		
	p2d::Point center1 = m_first->currentShape()->center();

	p2d::Point center2 = m_second->currentShape()->center();

	SL_DEBUG  (fmt::Format(tpl) << m_first->currentShape()->dump()
							     << m_av1.x() << m_av1.y()
								 << m_second->currentShape()->dump()
								 << m_av2.x() << m_av2.y()
								 << m_toi << m
			   );
	}
}


static int boundspeed_solving_branches[3][3] =
{
	{0, 2, 2},
	{1, 0, 2},
	{1, 1, 0}
};

static int bound_solver_get_branch_index(p2d::Body * b)
{
	int index = 0;
	if (b->fixed())
	{
		index = 2;
	}
	else
	{
		if (b->weight().isInfinite()) 
			index = 1;
	}
	return index;

}
void p2d::BounceSolver::resolveNormalSpeed(p2d::Body * b1, 
									  p2d::Vector & n1, 
									  p2d::Body * b2, 
									  const p2d::Vector & n2,
									  int index
									  )
{
	p2d::Vector vn1 = n1;
	n1 *= -1;
	int index1 = bound_solver_get_branch_index(b1);
	int index2 = bound_solver_get_branch_index(b2);

	// Get outcome from table
	int branch = boundspeed_solving_branches[index1][index2];

	if ( branch == 0 )
	{
		double m1 = b1->weight().value();
		double m2 = b2->weight().value();
		// If both object has infinite weight, we resolve them as 
		// equal weight objects
		if (b1->weight().isInfinite())
		{
			m1 = 1;
			m2 = 1;
		}
		n1 += ((vn1 * m1 + n2 * m2) / (m1 + m2)) * 2 ;
		n1 *= m_resilience[index];
		return;
	}
	// If only first object has infinite weight
	// his speed won't change
	if (branch == 1)
	{
		n1 *= -(m_resilience[index]);
		return;
	}
	if (branch == 2)
	{
		n1 =  (n2 * 2 - vn1);
		n1 *= m_resilience[index];
		return;
	}
	
}


void p2d::BounceSolver::resetCoefficients()
{
	m_resilience[0] = 1.0;
	m_resilience[1] = 1.0;
	m_rotationfriction[0] = 0.0;
	m_rotationfriction[1] = 0.0;
	m_shouldperformrotationfriction = true;
}

void p2d::BounceSolver::tryResolveFriction(p2d::Body * b, 
									       const p2d::Vector & t, 
									       const p2d::Vector & ni, 
								           int index,
									       double pivot
									      )
{
	if (non_fuzzy_zero(m_rotationfriction[index]) 
		&& b->weight().isInfinite() == false 
		&& m_shouldperformrotationfriction)
	{
		double w = b->angularVelocityAt(m_toi);
		p2d::Vector tangential = m_force_moment[index];
		p2d::mutableUnit(tangential);
		p2d::mutableNormalizedOrtho(tangential, p2d::OVI_DEG_90);

		p2d::Vector tangentialUnit = tangential;
		double forcemomentlength = p2d::modulo(m_force_moment[index]);
		double rotation_tangential_modulo = w * forcemomentlength;
		tangential *= rotation_tangential_modulo;
		tangential += t;

		if (non_fuzzy_zero(p2d::modulo(tangential)) && b->weight().isInfinite() == false)
		{
			tangential *= (1 - m_rotationfriction[index] / b->weight().value());
			// Compute new tangential velocity
			// What should we do here
			
			// Since we have no sliding friction force, a tangential part of velocity
			// won't change, so we must convert a tangential part to velocity. 
			tangential -= t;
			double wR = p2d::scalar(tangential, tangentialUnit);
			double w = wR / forcemomentlength;

			b->sheduleAngularVelocity(w);
		}
	}
}
#include "animations/animationswayinstance.h"

#include "animations/animationssavedobjectposition.h"

#include "animations/setstate/methodcall.h"
#include "animations/setstate/setproperty.h"
#include "animations/setstate/dummycommand.h"
#include "animations/setstate/setpositionproperty.h"
#include "animations/setstate/setpositionviaareacall.h"


#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fuzzyequal.h>

#include <math.h>
#include <fstream>

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

DECLARE_SOBJ_INHERITANCE(sad::animations::WayInstance, sad::animations::Instance);

// ============================ PUBLIC METHODS ============================

sad::animations::WayInstance::WayInstance()
{
	
}

sad::animations::WayInstance::~WayInstance()
{
	
}

static sad::db::schema::Schema* WayAnimationInstanceSchema = NULL;

sad::db::schema::Schema* sad::animations::WayInstance::basicSchema()
{
if (WayAnimationInstanceSchema == NULL)
    {
        WayAnimationInstanceSchema = new sad::db::schema::Schema();
		WayAnimationInstanceSchema->addParent(sad::animations::Instance::basicSchema());

        WayAnimationInstanceSchema->add(
            "way",
            new sad::db::MethodPair<sad::animations::WayInstance, unsigned long long>(
				&sad::animations::WayInstance::wayMajorId,
                &sad::animations::WayInstance::setWayMajorId
            )
        );
        
        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(WayAnimationInstanceSchema);
    }
    return WayAnimationInstanceSchema;
}

sad::db::schema::Schema* sad::animations::WayInstance::schema() const
{
	return sad::animations::WayInstance::basicSchema();
}

void sad::animations::WayInstance::setTable(sad::db::Table* t)
{
	this->sad::animations::Instance::setTable(t);
	m_way_link.setDatabase(t->database());
}

sad::String AnimationsWayInstanceName = "sad::animations::WayInstance"; 
const sad::String& sad::animations::WayInstance::serializableName() const
{
	return AnimationsWayInstanceName;
}

void sad::animations::WayInstance::setWayMajorId(unsigned long long majorid)
{
	m_way_link.setMajorId(majorid);
	m_valid = true;
}

unsigned long long sad::animations::WayInstance::wayMajorId() const
{
	return m_way_link.majorId();
}

void sad::animations::WayInstance::setWay(sad::p2d::app::Way* i)
{
	m_way_link.setObject(i);
	m_valid = true;
}

sad::p2d::app::Way* sad::animations::WayInstance::way() const
{
	sad::db::Object* i = const_cast<sad::animations::WayInstance*>(this)->m_way_link.get();
	sad::p2d::app::Way* result = NULL;
	if (i)
	{
		if (i->serializableName() == "sad::p2d::app::Way")
		{
			result = static_cast<sad::p2d::app::Way*>(i);
		}
	}
	return result; 
}

// ============================ PROTECTED METHODS ============================


void sad::animations::WayInstance::start(sad::animations::Animations* animations)
{
	this->checkIfValid(animations);
    if (m_valid)
    {
		this->saveStateAndCompile(animations);
		this->processTime(animations, m_start_time);
        m_timer.start();
        m_started = true;
    }
    else
    {
        this->markAsFinished();
    }
}

double sad::animations::WayInstance::computeTime(sad::animations::Animations* animations)
{
	double elapsed = m_start_time + m_timer.elapsed();
    double result = elapsed;

	sad::p2d::app::Way* w = static_cast<sad::p2d::app::Way*>(m_way_link.get());
    // If time is expired, animation is or looped, or should be stopped
    if (elapsed > w->totalTime())
    {
        if (w->closed())
        {
            double t = elapsed - floor(elapsed / w->totalTime()) * w->totalTime();
            result = t;
        }
        else
        {
            m_started = false;
            m_timer.stop();
            this->markAsFinished();
            if (m_finished)
            {
                this->restoreObjectState(animations);
            }
            else
            {
                result = m_start_time;
            }
        }
    }
    return result;
}

void sad::animations::WayInstance::processTime(sad::animations::Animations* animations, double time)
{
	sad::p2d::app::Way* way = static_cast<sad::p2d::app::Way*>(m_way_link.get());

	m_local_link.CurrentTime = time - 0.1;
	m_local_link.LinkedWay = way;
	sad::Point2D pos;
	way->step(&m_local_link, 0.1, pos);

	this->stateCommandAs<sad::Point2D>()->call(pos);
	if (m_body)
	{
		m_body->shedulePosition(pos);
	}
}

void sad::animations::WayInstance::checkIfValid(sad::animations::Animations* animations)
{
	sad::p2d::app::Way* w = this->way();
	sad::db::Object* o = this->object();
	
	m_valid = false;

	if (w)
	{
		if (w && o)
		{
			m_valid = o->getProperty<sad::Rect2D>("area").exists() && !sad::is_fuzzy_zero(w->totalTime());
		}
	}	
}

void sad::animations::WayInstance::saveStateAndCompile(sad::animations::Animations* animations)
{
	// Save state
	sad::db::Object* o = this->object();
	sad::String name = "sad::animations::SavedObjectSize";
	if (animations->cache().lookup(o, name) == false)
    {
		sad::animations::SavedObjectState* state = new sad::animations::SavedObjectPosition(o);
		if (m_body)
		{
			state->storeBodyState(m_body);
		}
        animations->cache().saveState(o, name, state);
    }
    else
    {
        animations->cache().increment(o, name);
    }

	// Set state command
	if (o)
	{
		if (o->isInstanceOf("sad::Sprite2D"))
		{
			setStateCommand(new sad::animations::setstate::SetPositionViaMethodCall<sad::Sprite2D>(o));
		}
		else
		{
			if (o->isInstanceOf("sad::Label"))
			{
				setStateCommand(new sad::animations::setstate::SetPositionViaMethodCall<sad::Label>(o));
			}
			else
			{
				if (o->isInstanceOf("sad::db::custom::Object"))
				{
					setStateCommand(new sad::animations::setstate::SetPositionViaMethodCall<sad::db::custom::Object>(o));
				}
				else
				{
					setStateCommand(new sad::animations::setstate::SetPositionProperty(o));
				}
			}
		}
	}
}

void sad::animations::WayInstance::restoreObjectState(sad::animations::Animations* animations)
{
	sad::String name = "sad::animations::SavedObjectSize";
	animations->cache().restore(m_object.get(), name);
}

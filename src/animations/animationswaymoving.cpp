#include "animations/animationswaymoving.h"
#include "animations/animationsinstance.h"
#include "animations/animationssavedobjectposition.h"

#include "animations/setstate/setpositionproperty.h"
#include "animations/setstate/setpositionviaareacall.h"
#include "animations/setstate/dummycommand.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"
#include "geometry2d.h"

#include "db/dbtable.h"
#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"

#include "label.h"
#include "sprite2d.h"
#include "db/custom/customobject.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>


DECLARE_SOBJ_INHERITANCE(sad::animations::WayMoving, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::WayMoving::WayMoving()
{
	m_creators.pushCreator<sad::animations::SavedObjectPosition>("sad::animations::SavedObjectPosition");
}

sad::animations::WayMoving::~WayMoving()
{
	
}

void sad::animations::WayMoving::setTable(sad::db::Table * table)
{
	m_way.setDatabase(table->database());	
}


bool sad::animations::WayMoving::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<unsigned long long> vector = picojson::to_type<unsigned long long>(
										           picojson::get_property(v, "way")
												);
		bool result = vector.exists();
		if (result)
		{
			setWayObjectId(vector.value());
		}

		flag = flag && result;
	}
	return flag;	
}

void sad::animations::WayMoving::setWay(sad::p2d::app::Way * way)
{
	m_way.setObject(way);
}

sad::p2d::app::Way* sad::animations::WayMoving::way(bool check) const
{
	sad::p2d::app::Way* result = NULL;
	sad:db::Object* o = const_cast<sad::animations::WayMoving*>(this)->m_way.get();
	if (o)
	{
		if (check)
		{
			if (o->isInstanceOf("sad::p2d::app::Way"))
			{
				result = static_cast<sad::p2d::app::Way*>(o);
			}
		}
		else
		{
			result = static_cast<sad::p2d::app::Way*>(o);
		}
	}
	return result;
}

void sad::animations::WayMoving::setWayObjectId(unsigned long long id)
{
	m_way.setMajorId(id);
}

unsigned long long sad::animations::WayMoving::wayObjectId() const
{
	return m_way.majorId();
}

void sad::animations::WayMoving::setState(sad::animations::Instance* i, double time)
{
	sad::p2d::app::Way* way = this->way(false);

	sad::p2d::app::Way::WayLink link;

	link.CurrentTime = time - 0.1;
	link.LinkedWay = way;
	sad::Point2D pos;
	way->step(&link, 0.1, pos);

	i->stateCommandAs<sad::Point2D>()->call(pos);
	if (i->body())
	{
		i->body()->shedulePosition(pos);
	}
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::WayMoving::stateCommand(sad::db::Object* o)
{
	if (o)
	{
		if (o->isInstanceOf("sad::Sprite2D"))
		{
			return new sad::animations::setstate::SetPositionViaMethodCall<sad::Sprite2D>(o);
		}
		
		if (o->isInstanceOf("sad::Label"))
		{
			return new sad::animations::setstate::SetPositionViaMethodCall<sad::Label>(o);
		}
			
		if (o->isInstanceOf("sad::db::custom::Object"))
		{
			return new sad::animations::setstate::SetPositionViaMethodCall<sad::db::custom::Object>(o);
		}
		
		return new sad::animations::setstate::SetPositionProperty(o);
	}
	return new sad::animations::setstate::DummyCommand<sad::Point2D>();
}

bool sad::animations::WayMoving::applicableTo(sad::db::Object* o)
{
	bool result = false;
    if (o && way(true) != NULL)
    {
		result = o->getProperty<sad::Rect2D>("area").exists() && !sad::is_fuzzy_zero(way(false)->totalTime());
		if (result)
		{
			setTime(way(false)->totalTime());
		}
    }
    return result;
}

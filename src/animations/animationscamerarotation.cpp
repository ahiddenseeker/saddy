#include "animations/animationscamerarotation.h"
#include "animations/animationsinstance.h"

#include "fuzzyequal.h"
#include "scene.h"
#include "camera.h"

#include "db/schema/schema.h"
#include "db/dbproperty.h"
#include "db/save.h"
#include "db/load.h"
#include "db/dbfield.h"
#include "db/dbmethodpair.h"
#include "db/dbtable.h"

#include <util/fs.h>

#include <resource/physicalfile.h>

#include <3rdparty/picojson/valuetotype.h>

#include <fstream>



DECLARE_SOBJ_INHERITANCE(sad::animations::CameraRotation, sad::animations::Animation);

// =============================== PUBLIC METHODS ==========================

sad::animations::CameraRotation::CameraRotation() : m_min_angle(0), m_max_angle(0)
{
	m_creators.pushCreator<sad::animations::SavedCameraRotation>("sad::animations::SavedCameraRotation");
}

sad::animations::CameraRotation::~CameraRotation()
{
	
}

static sad::db::schema::Schema* AnimationCameraRotationSchema = NULL;

sad::db::schema::Schema* sad::animations::CameraRotation::basicSchema()
{
    if (AnimationCameraRotationSchema == NULL)
    {
        AnimationCameraRotationSchema = new sad::db::schema::Schema();
        AnimationCameraRotationSchema->addParent(sad::animations::Animation::basicSchema());

        AnimationCameraRotationSchema->add(
            "min_angle",
            new sad::db::MethodPair<sad::animations::CameraRotation, double>(
				&sad::animations::CameraRotation::minAngle,
                &sad::animations::CameraRotation::setMinAngle
            )
        );
		AnimationCameraRotationSchema->add(
            "max_angle",
            new sad::db::MethodPair<sad::animations::CameraRotation, double>(
				&sad::animations::CameraRotation::maxAngle,
                &sad::animations::CameraRotation::setMaxAngle
            )
        );		
		AnimationCameraRotationSchema->add(
            "pivot",
			new sad::db::MethodPair<sad::animations::CameraRotation, sad::Point3D>(
				&sad::animations::CameraRotation::pivot,
				&sad::animations::CameraRotation::setPivot
            )
        );
		        
        sad::ClassMetaDataContainer::ref()->pushGlobalSchema(AnimationCameraRotationSchema);
    }
    return AnimationCameraRotationSchema;
}

sad::db::schema::Schema* sad::animations::CameraRotation::schema() const
{
    return sad::animations::CameraRotation::basicSchema();
}


bool sad::animations::CameraRotation::loadFromValue(const picojson::value& v)
{
	bool flag = this->sad::animations::Animation::loadFromValue(v);
	if (flag)
	{
		sad::Maybe<double> minangle     = picojson::to_type<double>(
										      picojson::get_property(v, "min_angle")
										  );
		sad::Maybe<double> maxangle     = picojson::to_type<double>(
										      picojson::get_property(v, "max_angle")
										  );
		sad::Maybe<sad::Point3D> pivot  = picojson::to_type<sad::Point3D>(
										          picojson::get_property(v, "pivot")
										      );
		bool result = minangle.exists() && maxangle.exists() && pivot.exists();
		if (result)
		{
			m_min_angle = minangle.value();
			m_max_angle = maxangle.value();
			m_pivot = pivot.value();
		}

		flag = flag && result;
	}
	return flag;
}

void sad::animations::CameraRotation::setMinAngle(double angle)
{
	m_min_angle = angle;
}

double sad::animations::CameraRotation::minAngle() const
{
	return m_min_angle;
}

void sad::animations::CameraRotation::setMaxAngle(double angle)
{
	m_max_angle = angle;
}

double sad::animations::CameraRotation::maxAngle() const
{
	return m_max_angle;
}

void sad::animations::CameraRotation::setPivot(const sad::Point3D& p)
{
	m_pivot = p;	
}

const sad::Point3D& sad::animations::CameraRotation::pivot() const
{
	return m_pivot;
}

void sad::animations::CameraRotation::setState(sad::animations::Instance* i, double time)
{
	double angle = m_min_angle + (m_max_angle - m_min_angle) * time / m_time;;
	i->stateCommandAs<double>()->call(angle);
}

sad::animations::setstate::AbstractSetStateCommand* sad::animations::CameraRotation::stateCommand(sad::db::Object* o)
{
	if (this->applicableTo(o) && o)
	{
		return new sad::animations::setstate::SetCameraRotation(static_cast<sad::Scene*>(o), m_pivot);
	}
	return new sad::animations::setstate::DummyCommand<double>();
}

bool sad::animations::CameraRotation::applicableTo(sad::db::Object* o)
{
	bool result = false;
	if (o && m_valid)
	{
		result = o->isInstanceOf("sad::Scene");
	}
	return result;
}

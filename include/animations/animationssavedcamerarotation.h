/*! \file animations\animationssavedcamerarotation.h
	\author HiddenSeeker

	Defines saved camera rotation settings as saved element
 */
#pragma once
#include "animations/animationssavedobjectstate.h"

#include "../scene.h"
#include "../camera.h"


namespace sad
{

namespace animations
{

/*! A saved camera rotation settings for scene
 */
class  SavedCameraRotation: public sad::animations::SavedObjectState
{
public:
    /*! Creates a new object and sets a scene
		\parma[in] o scene, which settings will be saved
	 */
	SavedCameraRotation(sad::db::Object* o);
	/*! Can be inherited
	 */
	virtual ~SavedCameraRotation();
	/*! Restores last object state
	 */
	virtual void restore();
protected:
	/*! An old angle for camera
	 */
	double m_angle;
	/*! An old pivot for translation vector
	 */
	sad::Vector3D m_pivot;
	/*! An old rotation vector
	 */
	sad::Vector3D m_rotationdirection;
};

}

}

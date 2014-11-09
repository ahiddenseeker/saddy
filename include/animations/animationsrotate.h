/*! \file animations/animationsrotate.h
	\author HiddenSeeker

	An animations as animation of rotating an object
 */
#pragma once

#include "../fuzzyequal.h"

#include "animationsanimation.h"

namespace sad
{
	
namespace animations
{

/*! An animation of rotating an object
 */
class Rotate: public sad::animations::Animation
{
SAD_OBJECT
public:
	/*! Creates new empty animation
	 */
	Rotate();
	/*! Can be inherited
	 */
	virtual ~Rotate();
    /*! Tries to load animation from value
        \param[in] v value
        \return whether it was successfull
     */
    virtual bool loadFromValue(const picojson::value& v);
	/*! Sets minimal angle
		\param[in] angle minimal angle
	 */
	void setMinAngle(double angle);
	/*! Returns minimal angle
		\return minimal angle
	 */
	double minAngle() const;
	/*! Sets maximal angle
		\param[in] angle maximal angle
	 */
	void setMaxAngle(double angle);
	/*! Returns maximal angle
		\return maximal angle
	 */
	double maxAngle() const;
	/*! Sets state of object from animation
        \param[in] i an animation instance
        \param[in] time a time of playing of animation
     */
    virtual void setState(sad::animations::Instance* i, double time);
    /*! Creates a state command for an object
        \param[in] o object
        \return state command
     */
    virtual sad::animations::setstate::AbstractSetStateCommand* stateCommand(sad::db::Object* o);
    /*! Checks, whether animation is applicable to an object
        \param[in] o object
        \return whether animation is applicable to that object
     */
    virtual bool applicableTo(sad::db::Object* o);
protected:
	/*! A minimal angle
	 */
	double m_min_angle;
	/*! A maximal angle
	 */
	double m_max_angle;
};

}

}

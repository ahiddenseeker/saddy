/*! \file simplecollisiondetector.h
	\author HiddenSeeker

	Determines a simple collision detector, which detects a collision
	by testing it at a specified moment, defined as proportion to time
	limit
 */
#include "collisiondetector.h"
#pragma once

namespace p2d
{
/*! A simple collision detector, which tests collision shapes at specified
	time, defined as proportion to the time limit
 */
class SimpleCollisionDetector: public p2d::CollisionDetector
{
 private:
	 p2d::CollisionTest * m_tester; // !< A tester, which tests shapes for collisions
	 double  m_moment;  //!< A moment, when we should determine collision
 public:
	 /*! Creates new detector
		 \param[in] moment moment when tests will be performed as proportion to time
		 \param[in] t      a shape collision testing callbacks
	  */
	 inline SimpleCollisionDetector(
		 double moment  = 1.0,
		 p2d::CollisionTest * t = new p2d::CollisionTest()
	 )
	 : m_tester(t), m_moment(moment)
	 {
	 }

	 /*! Tests, whether two bodies collide within specified limit
		  testing their movement in interval [0, limit] and returning
		  time if possible
		  \param[in] b1 first body
		  \param[in] b2 second body
		  \param[in] time time limit
		  \return if detected collision - moment * limit
	   */
	  virtual p2d::MaybeTime collides(p2d::Body * b1, 
									  p2d::Body * b2, 
									  double limit);

	 ~SimpleCollisionDetector();
};

}


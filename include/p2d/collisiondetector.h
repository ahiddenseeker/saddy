/*! \file collisiondetector.h
	\author HiddenSeeker

	Describes a basic collision detector, used to determine, whether
	bodies collide and when does it occured.
 */
#pragma once
#include "body.h"
#include "collisiontest.h"
#include "../maybe.h"


namespace sad
{

namespace p2d
{
typedef sad::Maybe<double> MaybeTime;
/*! A basic collision detector, used to determine, whether bodies
	collide and when does it ocurred.
 */
class CollisionDetector
{
  public:
	  /*! Tests, whether two bodies collide within specified limit
		  testing their movement in interval [0, limit] and returning
		  time if possible
		  \param[in] b1 first body
		  \param[in] b2 second body
		  \param[in] limit a time limit for finding a position of bodies
		  \return time of impact if possible
	   */
	  virtual sad::p2d::MaybeTime collides(sad::p2d::Body * b1, 
										   sad::p2d::Body * b2, 
									       double limit) = 0;
	  /*! Returns a sample  count, needed to fetch samples for detection
	   */
	  virtual int sampleCount() const;
	  virtual ~CollisionDetector();
};

}

}

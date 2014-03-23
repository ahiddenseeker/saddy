/*! \file line.h
	\author HiddenSeeker

	Defines a line as a collision shape
 */
#pragma once
#include "collisionshape.h"
#include "point.h"

namespace sad
{

namespace p2d
{
/*! Describes a line as collision shape
 */
class Line: public sad::p2d::CollisionShape
{
	SAD_OBJECT
protected:
	sad::p2d::Cutter2D m_c; //!< An inner cutter
public:
	inline Line() {}
	/*! Sets new cutter
		\param[in] c cutter
	 */
	inline void setCutter(const sad::p2d::Cutter2D & c) { m_c = c; }
	/*! Sets new cutter
		\param[in] p1 first point
		\param[in] p2 second point
	 */
	inline void setCutter(const sad::p2d::Point & p1, const sad::p2d::Point & p2) { setCutter(sad::p2d::Cutter2D(p1, p2)); }
	/*! Sets new cutter
		\param[in] x1  x of first point
		\param[in] y1  y of first point
		\param[in] x2  x of second point
		\param[in] y2  y of second point
	 */
	inline void setCutter(double x1, double y1, double x2, double y2)
	{
		setCutter(sad::p2d::Point(x1, y1), sad::p2d::Point(x2, y2));
	}
	/*! Returns a cutter
		\return cutter
	 */ 
	inline const sad::p2d::Cutter2D & cutter() const { return m_c; }
	/*! Returns new identical cuttes
		\param[in] count how many cutters we should create
		\return raw array of cutters
	 */
	sad::p2d::CollisionShape * clone(int count) const;
	/*! Returns a center of cutter
		\return center of cutter
	 */
	sad::p2d::Point center() const;
	/*! Rotates a lline around it's center
		\param[in] angle angle to rotate
	 */
	void rotate(double angle);
	/*! Moves a line by specified vector
		\param[in] d distance to move
	 */
	void move(const sad::p2d::Vector & d);
	/*! Converts a cutter to convex hull
		\return convex hull
	 */
	sad::p2d::ConvexHull toHull() const;
	/*! Projects cutter to axle
		\param[in] a axle
		\return cutter
	 */
	sad::p2d::Cutter1D project(const sad::p2d::Axle & a) const;
	/*! Returns a points from a line
		\return points
	 */
	sad::Vector<sad::p2d::Point> points() const;
	/*! Returns size of current type
		\return size of type in bytes
	 */
	virtual size_t sizeOfType() const;
	/*! Returns first point of line
		\return first point
	 */
	const sad::Point2D & p1() const { return m_c.p1();}
	/*! Returns second point of line
		\return second point
	 */
	const sad::Point2D & p2() const { return m_c.p2();}
	/*! Populates a vector two pooints, belonging to a border of bound
		\param[in] v vector
	 */
	virtual void populatePoints(sad::Vector<sad::p2d::Point> & v) const;
	/*! In any case, returns normal to a bound. A normal is returned as rotation
		of unit vector of line by 90 degrees. So if line will be like [p1, p2] and normal is like (n1, n2)
		the following cases will be valid:
		[ (0, 0), (1, 0) ] - (0, -1),
		[ (1, 0), (0, 0) ] - (0, 1),
		[ (0, 0), (0, 1) ] - (1, 0),
		[ (0, 1), (0, 0) ] - (-1, 0).
		\param[in] p point
		\param[out] n resulting normal
	 */
	virtual void normalToPointOnSurface(const sad::p2d::Point & p, sad::p2d::Vector & n) ;
	/*! Dumps object to string
		\return string
	 */
	virtual sad::String dump() const; 
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Line)

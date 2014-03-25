/*! \file walls.h
	\author HiddenSeeker

	Describes a bounding walls, that teleports collided player to other wall
	and destroy other objects
 */
#pragma once
#include "body.h"
#include "circle.h"
#include "line.h"
#include "bounds.h"

#include "../object.h"

namespace sad
{

namespace p2d
{
class Wall;
/*! Describes walls as in-game bounding container, when object
	within walls behaves normally, and when out of walls - it'll be destroyed
	or teleported (if this is player)
 */
class Walls
{
public:
	/*! A minimal definition for a wall
	 */
	typedef sad::Pair<p2d::BoundType, double> minimal_t;
	/*! Creates bounding walls with specified padding, which defines how far
	    walls are from edges of window. If we pass here 14, that the bounding
	    rectangle will be 14 points larger than screen bounds.
	    \param[in] padding
	 */
	Walls(double padding = 0);
	/*! Creates bounding walls with specified padding within window [0..width], [0..height].
		For example, if we pass 14 in padding we will get walls of [14..width-14], 
		[14..height-14]
		\param[in] width basic width of walls
		\param[in] height basic height of walls
		\param[in] padding a padding for walls
	 */
	Walls(double width, double height, double padding = 0);
	/*! Returns physical bodies of walls to add them into a game world
	 */
	const sad::Vector<p2d::Body *> bodies() const;
    /*! Destroys walls
	 */
	virtual ~Walls();
protected:
	/*! A non-reentrant constructor, which creates walls, populating lists of 
		walls and their bodies. Note, that padding must be already pre-set before call
		of this method.
		\param[in] width width of walls
		\param[in] height height of walls
	 */
	void makeWalls(double width, double height);

	sad::Vector<sad::p2d::Wall *> m_walls;  //!< A bounding walls
	sad::Vector<p2d::Body *> m_bodies; //!< A bodies

	
	double m_padding; //!< A padding for walls and bodies
};

/*! Describes a static non-moving wall
 */
class Wall: public sad::Object
{
SAD_OBJECT
public:
	/*! Creates an empty wall as object
	    \param[in] padding a padding between scene rectangle and wall positions
	 */
	Wall(double padding = 0.0);
	/*! Tries to teleport an object
	    \param[in] p body
	 */
	void tryTeleport(p2d::Body * p);
	/*! Returns a body
	    \return body
	 */
	inline p2d::Body * body() const { return m_body; }
	/*! Sets new body
		\param[in] b body
	 */
	inline void setBody(p2d::Body * b) { m_body = b; }
	/*! Returns body for opposite wall
		\return opposite body
	 */
	inline p2d::Body * oppositeBody() const { return m_body; }
	/*! Sets body for opposite wall
		\param[in] b opposite body
	   */
	inline void setOppositeBody(p2d::Body * b) {m_opposite_body = b;} 
	/*! Returns type of a wall
		\return type of wall
	 */
	p2d::BoundType  type() const;
protected:
	/*! A padding for opposite wall
	 */
	double m_padding;
	/*! A type of wall
	 */
	p2d::Body  * m_opposite_body;
	/*! A body
	 */
	p2d::Body * m_body;
};

}

}

DECLARE_TYPE_AS_SAD_OBJECT_ENUM(sad::p2d::Wall)

/*! \file world.h
	\author HiddenSeeker

	Describes a main physics simulation
 */
#pragma once

#include "movingsmile.h"

#include <extra/periodicalevent.h>
#include <scene.h>
#include <p2d/app/app.h>
#include <p2d/findcontactpoints.h>
#include <p2d/walls.h>
#include <p2d/bouncesolver.h>
#include <label.h>


class GridNode;
class Ball;
class Platform;
class UncoloredBullet;
class Shooter;
class ColoredBullet;
/*! Main class for application
 */
class World: public sad::p2d::app::App
{
protected:
	sad::p2d::FindContactPoints * m_find; //!<  A method for finding contact points
	sad::p2d::Walls * m_walls;   //!< A walls
	sad::p2d::BounceSolver * m_solver; //!< A solver for bouncing objects
	sad::p2d::app::Way          * m_way;    //!< A way for moving smilies

	int m_hit_count; //!< A hit count
	/*! This callback sets a friction for collision, so ball will lose speed
		after touching wall
		\param[in] ev event
	 */
	void onWallBall(const sad::p2d::CollisionEvent<Ball, sad::p2d::Wall> & ev);
	/*! This callback sets a friction for collision, so ball will lose speed
		after touching node
		\param[in] ev event
	 */
	void onBallNode(const sad::p2d::CollisionEvent<Ball, GridNode> & ev);
	/*! This callback increments a hit count, after uncolored bullet hits a shooter
		\param[in] ev event
	 */
	void incrementHitCount(const sad::p2d::CollisionEvent<UncoloredBullet, Shooter> & ev);
	/*! Erases both bullets
		\param[in] ev event
	 */
	void eraseBullets(const sad::p2d::CollisionEvent<ColoredBullet, UncoloredBullet> & ev);
	/*! A shooter's colored bullets will boost a ball, when they hit him. Also, they will
		be destroyed ar the moment
		\param[in] ev event
	 */
	void boostBall(const sad::p2d::CollisionEvent<ColoredBullet, Ball> & ev); 

	/*! Performs bouncing off both objects, using solver
		\param[in] ev event
	 */
	template<typename _O1, typename _O2>
	void performBounce(const sad::p2d::CollisionEvent<_O1, _O2> & ev)
	{
		m_solver->bounce(ev.m_object_1->body(), ev.m_object_2->body());
	}
	/*! Removes object with type _O1 from a world
		\param[in] ev event
	 */
	template<typename _O1, typename _O2>
	void removeFirst(const sad::p2d::CollisionEvent<_O1, _O2> & ev)
	{
		this->removeObject(ev.m_object_1);
	}
	/*! Toggles fullscreen		
	 */
	void toggleFullscreen(const sad::Event & );
public:

	World();

	~World();
	/*! Runs a simulation
	 */
	void run();
	/*! Quits a simulation
	 */
	void quit();
	/*! Returns count, how much times player is hit
	 */
	int hitCount();
};



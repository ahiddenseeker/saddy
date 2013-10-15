/*! \file startscreenrain.h
	\author HiddenSeeker

	Describes a rain in start screen
 */
#pragma once
#include <input.h>
#include <periodicalevent.h>


class Game;

/*! Descibes a rain of objects in start screen
 */ 
class StartScreenRain: public sad::PeriodicalEvent
{
 protected:
	 Game * m_game; //!< A game which rain is linked to

	 /*! Adds a new objects
	  */
	 virtual void perform();
 public:
	 /*! Creates new rain, linked to specified game
	  */
	 StartScreenRain(Game * g);
};



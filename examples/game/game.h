/*! \file game.h
	\author HiddenSeeker

	Describes a game state with all references and other stuff
 */
#pragma once
#include <extra/statemachine.h>
#include <extra/periodicalevent.h>
#include <scene.h>
#include <p2d/app/app.h>

/*! An enumerations for creating factory objects
 */
enum Objects
{
	O_BONUS = 0,
	O_ENEMY = 1,
	O_SHOOTINGENEMY = 2,
	O_SUPERSHOOTINGENEMY = 3
};

class Wall;
class Walls;
class GameObject;
class Player;
class PlayerBullet;
class EnemyBullet;
class Bonus;
class Enemy;
class ShootingEnemy;
class SuperShootingEnemy;

/*! Represents enumeration of game state data
 */
class GameState
{
 public:
	static const hst::string START;
	static const hst::string PLAYING;
};

/*! A main class of game
 */
class Game: public p2d::app::App
{
 private:
	int  m_highscore;		         //!< Defines a highest score, gained by player
	bool m_ispaused;		         //!< Whether game is paused
	fsm::Machine * m_machine;        //!< A state machine, which describes all transitions
	Player       * m_player;         //!< A link to in-game player
	TimePeriodicalTask * m_spawntask;//!< A task for spawn an enemies
	Walls        * m_walls;          //!< A new walls
	int            m_registered_supershooting_enemies_count; //!< Current count of super shooting enemies, active in game
 protected:
	/*! Create walls for game
	 */
    void createWalls();
	/*! A callback for leaving starting screen
	 */
	void leaveStartingScreen();
	/*! A callback for leaving playing screen
	 */
	void leavePlayingScreen();
	/*! A callback for entering a starting screen
	 */
	void enterStartingScreen();
	/*! A callback for entering a playing screen
	 */
	void enterPlayingScreen();
	/*! Moves to starting screen. Used to not break execution chain when
		called from enterStartingScreen from playing state
	 */
	void moveToStartingScreen();
	/*! Deletes an object if not player, when collision with wall
		\param[in] ev event
	 */
	void onWallCollision(const p2d::CollisionEvent<Wall, GameObject> & ev);
	/*! Increases score and hp of bonus and player
		\param[in] ev event
	 */
	void onBonusCollision(const p2d::CollisionEvent<Player, Bonus> & ev);
	/*! Increases score of player and decrements hp of enemy
		\param[in] ev event
	 */
	void onPlayerBulletEnemy(const p2d::CollisionEvent<PlayerBullet, Enemy> & ev);
	/*! Increases score of player and decrements hp of enemy
		\param[in] ev event
	 */
	void onPlayerBulletSEnemy(const p2d::CollisionEvent<PlayerBullet, ShootingEnemy> & ev);
	/*! Increases score of player and decrements hp of enemy
		\param[in] ev event
	 */
	void onPlayerBulletSuperEnemy(const p2d::CollisionEvent<PlayerBullet, SuperShootingEnemy> & ev);
	/*! Decreases player's hp and destroys bullet
		\param[in] ev event
	 */
	void onPlayerEnemyBullet(const p2d::CollisionEvent<Player, EnemyBullet> & ev);
	/*! Decreases player's hp and destroys enemy
		\param[in] ev event
	 */
	void onPlayerEnemy(const p2d::CollisionEvent<Player, Enemy> & ev);
	/*! Decreases player's hp and destroys shooting enemy
		\param[in] ev event
	 */
	void onPlayerShootingEnemy(const p2d::CollisionEvent<Player, ShootingEnemy> & ev);
	/*! Decreases player's hp and destroys super shooting enemy
		\param[in] ev event
	 */
	void onPlayerSuperShootingEnemy(const p2d::CollisionEvent<Player, SuperShootingEnemy> & ev);
 public:
	/*! Creates a new non-paused game with no highscore
	 */
	Game();
	/*! Tries to toggle fullscreen mode if not paused
	 */
	void tryToggleFullscreen();
	/*! Increases a score for player
	 */
	void increasePlayerScore(int delta);
	/*! Increases player's health by specified points
	 */
	void increasePlayerHealth(int by);
	/*! Decreases player's health by specified points. May end
		the game, when health goes below zero.
	 */
	void decreasePlayerHealth(int by);
	/*! Returns a highest score for player
		\return highscore
	 */
	int highscore() const;
	/*! Tests whether game is paused
		\return whether game is paused
	 */
	bool isPaused() const;
	/*! Tests whether game is paused
		\return whether game is paused
	 */
	bool isPlaying() const;
	/*! Changes game to paused or not
	 */ 
	void togglePaused();
	/*! Switches to playing mode
 	 */
	void startPlaying();
	
	/*! Returns ingame player
	 */
	Player * player();
	/*! Runs a game
	 */
	void run();
	/*! Returns current state of game
		\return current game state
	 */
	const hst::string & state();
	/*! Produces game object of specified type
	 */
	GameObject * produce(Objects type);
	~Game();
	/*! Sets a new highscore for game, sets in if it's largest that current
		score.
		\param[in] score 
	 */
	void trySetHighscore(int score);
	/*! Removes an object
		\param[in] o object
	 */
	void removeObject(p2d::app::Object * o);
};

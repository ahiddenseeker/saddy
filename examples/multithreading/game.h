/*! \file game.h

    Main game class
 */
#pragma once
#include <sadthread.h>

#include <functional>

#include <sprite2d.h>

#include <sadhash.h>

#include <hfsm/hfsmmachine.h>

#include <irrklang/singlesound.h>
#include <irrklang/engine.h>

#include "scenetransitionprocess.h"

namespace threads
{
class GameThread;
}

/*! Main game class
 */
class Game  // NOLINT(cppcoreguidelines-special-member-functions)
{
public:
    /*! A main menu state
     */
    enum MainMenuState
    {
        GMMS_PLAY     = 0,  //!< A new game is selected
        GMMS_OPTIONS  = 1,  //!< Options are selected
        GMMS_EXIT     = 2   //!< An exit state is selected
    };
    /*! Constructs game object
     */
    Game();
    /*! Destructs game object
     */
    ~Game();
    /*! Runs main game thread
     */
    void runMainGameThread();
    /*! Runs inventorty thread
     */
    void runInventoryThread();
    /*! Quits game
     */
    void quitGame();
    /*! Returns highest score for a game
     */
    int highscore() const;
    /*! Sets new highscore for a game
        \param[in] highscore a highscore
     */
    void setHighscore(int highscore);
    /*! Start starting state
     */
    void tryStartStartingState();

    /*! Transitions the game from current scene to the next one
        \param[in] load_new_data functon for loading elements for the new scene
        \param[in] on_loaded function, which will be called, when scene is loaded
        \param[in] actions_after_transition function with actions that will be done after loading data
    */
    void changeScene(std::function<void()> load_new_data, std::function<void()> on_loaded, std::function<void()> actions_after_transition);
    /*! Transitions the game from current scene to the next one
        \param[in] renderer renderer for animation
        \param[in] time time period for animation
        \param[in] dark if true - transition for bolder color, false - sprite will fade into nothing
        \return animation
    */
    sad::animations::Instance* setAnimationForScreenTransition(sad::Renderer &renderer, long time, bool dark);

    /*! Returns renderer for main thread
     */
    sad::Renderer* rendererForMainThread() const;
    /*! Returns renderer for inventory thread
     */
    sad::Renderer* rendererForInventoryThread() const;
private:
    /*! Disabled constructor
     */
    Game(const Game&);
    /*! Disabled copy operator
        \return self-reference
     */
    Game& operator=(const Game&);
    /*! A communication object for main thread
     */
    threads::GameThread* m_main_thread;
    /*! A communication object for inventory thread
     */
    threads::GameThread* m_inventory_thread;
    /*! Whether game is quitting
     */
    bool m_is_quitting;

    /*!  A main menu state for main menu
     */
    Game::MainMenuState  m_main_menu_state;
    /*! A mappping, that maps a state index from Game::MainMenuState
        to a label, where should be placed marker (player stuff)
     */
    sad::Hash<size_t, sad::String> m_main_menu_states_to_labels;

    /*! A state machine for searching data
     */
    sad::hfsm::Machine m_state_machine;
    /*! A state machine for paused state
     */
    sad::hfsm::Machine m_paused_state_machine;
    /*! A highestscore for game
     */
    int m_highscore;

    /*! A theme sound, that should be played
     */
    sad::irrklang::SingleSound m_theme;
    /*! A theme playing song
     */
    ::irrklang::ISound* m_theme_playing;

    /*! A transition process
     */
    SceneTransitionProcess* m_transition_process;
};

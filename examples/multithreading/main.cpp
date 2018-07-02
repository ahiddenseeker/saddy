/*! \mainpage Multithreading Example
    A simple example of two OpenGL windows running simultaneously
 */
/*! \file main.cpp
    
    A multithreading example is an example of two OpenGL windows in one process
    running simultaneously.
 */
#include <renderer.h>
#include <input/controls.h>
#include <keymouseconditions.h>
#include <input/handlers.h>
#include <font.h>
#include <orthographiccamera.h>
#include <sadthread.h>
#include <sprite2d.h>
#include <label.h>
#include "game/inventory.h"
#include "game/item.h"
#include <texturemappedfont.h>
#include <freetype/font.h>
#include <animations/animationstyping.h>
#include <animations/animationstexturecoordinatescontinuous.h>
#include <animations/animationscolor.h>
#include <animations/animationsparallel.h>
#include <animations/animationsinstance.h>

#include "3rdparty/tpunit++/tpunit++.hpp"
#include "game.h"

#ifdef WIN32
#include <windows.h>
#endif



/* This macro manupilation is to make console window hidden in MSVC window
 */
#ifdef WIN32
#ifndef MSVC_RELEASE
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char** argv)
#endif
#else
int main(int argc, char** argv)
#endif
{
    Game main_game;
    // Here we create two waitable threads
    sad::Thread a(&main_game, &Game::runMainGameThread);
    sad::Thread b(&main_game, &Game::runInventoryThread);

    // Run them
    a.run();
    b.run();

    // And wait
    a.wait();
    b.wait();
    return 0;

}


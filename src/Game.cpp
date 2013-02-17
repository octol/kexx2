//    Kexx2
//    Copyright (C) 2001, 2013 Jon Häggblad
//
//    This file is part of Kexx2.
//
//    Kexx2 is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    Kexx2 is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Kexx2.  If not, see <http://www.gnu.org/licenses/>.

#include <fstream>
#include <iostream>
#include "Game.h"
#include "SDLc/Screen.h"
#include "SDLc/Mixer.h"
#include "SDLc/Misc.h"
#include "SDLc/Timer.h"
#include "SDLc/Input.h"
#include "World.h"
#include "BuyScreen.h"
#include "Menu.h"
#include "Finished.h"
#include "GameOver.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Game::Game()
{
}

Game::~Game()
{
    delete environment;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Game::loadOptions()
{
#ifndef WIN32
    std::string settingsfile;
    std::string filename = getenv("HOME");
    filename = filename + "/.kexx2rc";

    std::ifstream file(filename.c_str());
    if (!file) {
        std::cout << "What is the path to the data dir? ";
        std::cout << "Example: [/home/user/kexx2/data/]" << std::endl;
        std::cin >> settingsfile;

        std::ofstream file2(filename.c_str());
        file2 << settingsfile;
        file2.close();
    } else {
        getline(file, settingsfile);
    }
    file.close();

    options.dataPath = settingsfile + "/";
#endif
#ifdef WIN32
    options.dataPath = "../data/";
#endif
    options.load(options.dataPath + "kexx2.cfg");
}

void Game::writeOptions()
{
    options.write(options.dataPath + "kexx2.cfg");
}

void Game::setupEnvironment(Screen& screen, Timer& timer, Mixer& mixer)
{
    // TODO: Replace with call to sdlc::Timer
    srand(SDL_GetTicks());

    std::string v = VERSION;
    // Should incorporate this function in sdlc::Screen
    SDL_WM_SetCaption(("Kexx 2 " + v).c_str(), NULL);

#ifdef WIN32
    int videoType;
    if (options.fullscreen())
        videoType = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
    else videoType = SDL_SWSURFACE;
#endif
#ifndef WIN32
    int videoType = SDL_SWSURFACE;
    if (options.fullscreen())
        videoType |= SDL_FULLSCREEN;
#endif

    screen.init(640, 480, 16, videoType);
    timer.init();
    mixer.init();

    // TODO: move this into sdlc::Screen
    SDL_ShowCursor(false);
    mainFont.load(options.dataPath + "fonts/font1.bmp");
    // TODO: Should incorporate this function in sdlc::Timer
    SDL_Delay(500);
}

void Game::start()
{
    // init game
    environment = new Menu(options);
}

void Game::runLogic(Timer& timer)
{
    // here we decide which Environment that should be used
    if (environment && environment->done()) {
        if (environment->getType() == ENV_MENU) {
            delete environment;
            environment = 0;

            currentLevel = 1;
            if (playerState.anyoneAlive())
                environment = new World(options, playerState, currentLevel);
            else done(true);
        } else if (environment->getType() == ENV_BUYSCREEN) {
            delete environment;
            environment = 0;

            environment = new World(options, playerState, currentLevel);
        } else if (environment->getType() == ENV_WORLD) {
            delete environment;
            environment = 0;
            currentLevel++;

            // game complete
            if (currentLevel > options.getHowManyLevels() && 
                    playerState.anyoneAlive()) {
                environment = new Finished(options, playerState);
            }
            // goto inbetween levels buyscreen
            else if (playerState.anyoneAlive()) {
                environment = new BuyScreen(options, playerState, currentLevel);
            }
            // game over
            else {
                environment = new GameOver();
            }
        } else {
            playerState.killall();
            delete environment;
            environment = 0;
            environment = new Menu(options);
        }
    }

    // abort in world
    extern Input* input;
    if (environment && environment->getType() == ENV_WORLD) {
        if (input->keyPressed(SDLK_ESCAPE, NO_AUTOFIRE)) {
            delete environment;
            playerState.killall();
            environment = new Menu(options);
        }
    }

    // used for testing
    if (input->keyPressed(SDLK_F2, NO_AUTOFIRE)) {
        delete environment;
        environment = 0;
    }
    if (input->keyPressed(SDLK_F3, NO_AUTOFIRE)) {
        delete environment;
        environment = new World(options, playerState, currentLevel = 1);
    }
    if (input->keyPressed(SDLK_F4, NO_AUTOFIRE)) {
        delete environment;
        environment = new BuyScreen(options, playerState, currentLevel);
    }

    if (environment)
        environment->runLogic(timer, playerState);
}

void Game::draw(Screen& screen)
{
    if (environment)
        environment->draw(screen, mainFont);
}

bool Game::done() const
{
    return m_done;
}

bool Game::done(bool value)
{
    return m_done = value;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

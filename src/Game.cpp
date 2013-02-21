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
// Member Functions
// -----------------------------------------------------------------------------
Game::~Game()
{
    delete game_state;
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
    game_state = new Menu(options);
}

void Game::runLogic(Timer& timer)
{
    // here we decide which Environment that should be used
    if (game_state && game_state->done()) {
        if (game_state->type() == ENV_MENU) {
            delete game_state;
            game_state = 0;

            current_level_ = 1;
            if (playerState.anyoneAlive()) {
                game_state = new World(options, playerState, current_level_);
            } else {
                set_done(true);
            }
        } else if (game_state->type() == ENV_BUYSCREEN) {
            delete game_state;
            game_state = 0;

            game_state = new World(options, playerState, current_level_);
        } else if (game_state->type() == ENV_WORLD) {
            delete game_state;
            game_state = 0;
            current_level_++;

            // game complete
            if (current_level_ > options.getHowManyLevels() && 
                    playerState.anyoneAlive()) {
                game_state = new Finished(options, playerState);
            }
            // goto inbetween levels buyscreen
            else if (playerState.anyoneAlive()) {
                game_state = new BuyScreen(options, playerState, current_level_);
            }
            // game over
            else {
                game_state = new GameOver();
            }
        } else {
            playerState.killall();
            delete game_state;
            game_state = 0;
            game_state = new Menu(options);
        }
    }

    // abort in world
    extern Input* input;
    if (game_state && game_state->type() == ENV_WORLD) {
        if (input->keyPressed(SDLK_ESCAPE, NO_AUTOFIRE)) {
            delete game_state;
            playerState.killall();
            game_state = new Menu(options);
        }
    }

    // used for testing
    if (input->keyPressed(SDLK_F2, NO_AUTOFIRE)) {
        delete game_state;
        game_state = 0;
    }
    if (input->keyPressed(SDLK_F3, NO_AUTOFIRE)) {
        delete game_state;
        game_state = new World(options, playerState, current_level_ = 1);
    }
    if (input->keyPressed(SDLK_F4, NO_AUTOFIRE)) {
        delete game_state;
        game_state = new BuyScreen(options, playerState, current_level_);
    }

    if (game_state)
        game_state->runLogic(timer, playerState);
}

void Game::draw(Screen& screen)
{
    if (game_state) {
        game_state->draw(screen, mainFont);
    }
}

bool Game::done() const
{
    return done_;
}

bool Game::set_done(bool value)
{
    return done_ = value;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

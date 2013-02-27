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
#include "Defines.h"

void Game::load_options()
{
    // TODO: Rewrite this to look in some standard locations first.
    // This requires that we store options in .kexx2rc instead of kexx2.cfg
    // TODO: Make this more robust to what the user enters.
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

    options.data_path = settingsfile + "/";
#endif
#ifdef WIN32
    options.data_path = "../data/";
#endif
    options.load(options.data_path + "kexx2.cfg");
}

void Game::write_options()
{
    options.write(options.data_path + "kexx2.cfg");
}

void Game::setup_environment(Screen& screen, Timer& timer, Mixer& mixer)
{
    srand(timer.ticks());
    screen.set_caption(("Kexx 2 " + std::string(VERSION)).c_str());

#ifdef WIN32
    int video_type = options.fullscreen() ?
                     SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN :
                     SDL_SWSURFACE;
#endif
#ifndef WIN32
    int video_type = options.fullscreen() ? SDL_SWSURFACE | SDL_FULLSCREEN
                     : SDL_SWSURFACE;
#endif

    screen.init(SCREEN_WIDTH, SCREEN_HEIGHT, 16, video_type);
    timer.init();
    mixer.init();

    screen.show_cursor(false);
    main_font_.load(options.data_path + "fonts/font1.bmp");
    timer.delay(500);
}

void Game::start()
{
    game_state_ = std::unique_ptr<Menu>(new Menu(options));
}

void Game::run_logic(Input& input, Timer& timer, Mixer& mixer)
{
    // here we decide which Game state that should be used
    if (game_state_ && game_state_->done()) {
        switch (game_state_->type()) {
        case ENV_MENU:
            current_level_ = 1;
            if (player_state.anyoneAlive())
                game_state_ = std::unique_ptr<World>
                    (new World(timer, options, player_state, current_level_));
            else
                set_done(true);
            break;

        case ENV_BUYSCREEN:
            game_state_ = std::unique_ptr<World>
                (new World(timer, options, player_state, current_level_));
            break;

        case ENV_WORLD:
            current_level_++;

            // game complete
            if (current_level_ > options.num_of_levels() &&
                    player_state.anyoneAlive()) {
                game_state_ = std::unique_ptr<Finished>
                    (new Finished(options, player_state));
            }
            // goto inbetween levels buyscreen
            else if (player_state.anyoneAlive()) {
                game_state_ = std::unique_ptr<BuyScreen>
                    (new BuyScreen(options, current_level_));
            }
            // game over
            else {
                game_state_ = std::unique_ptr<GameOver>(new GameOver());
            }
            break;

        default:
            player_state.killall();
            game_state_ = std::unique_ptr<Menu>(new Menu(options));
            break;
        }
    }

    // abort in world
    if (game_state_ && game_state_->type() == ENV_WORLD) {
        if (input.keyPressed(SDLK_ESCAPE, NO_AUTOFIRE)) {
            player_state.killall();
            game_state_ = std::unique_ptr<Menu>(new Menu(options));
        }
    }

#ifdef TESTING
    // used for testing
    if (input.keyPressed(SDLK_F2, NO_AUTOFIRE)) {
    }
    if (input.keyPressed(SDLK_F3, NO_AUTOFIRE)) {
        game_state_ = std::unique_ptr<World>
            (new World(timer, options, player_state, current_level_ = 1));
    }
    if (input.keyPressed(SDLK_F4, NO_AUTOFIRE)) {
        game_state_ = std::unique_ptr<BuyScreen>
            (new BuyScreen(options, current_level_));
    }
#endif

    if (game_state_)
        game_state_->run_logic(input, timer, mixer, player_state);
}

void Game::draw(Screen& screen)
{
    if (game_state_) {
        game_state_->draw(screen, main_font_);
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

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

#include "Game.h"

#include <fstream>
#include <iostream>

#include "config.h"

#include "SDLc/Screen.h"
#include "SDLc/Mixer.h"
#include "SDLc/Timer.h"
#include "SDLc/Input.h"

#include "World.h"
#include "BuyScreen.h"
#include "Menu.h"
#include "Finished.h"
#include "GameOver.h"
#include "Defines.h"

void Game::load_options(std::string data_path)
{
    options.load(CONFIG_FILE);

    // If data_path was specified on the commandline we use that one
    // instead.
    if (!data_path.empty())
        options.data_path = data_path;

    // If data_path still is empty, then use compile time path.
    if (options.data_path.empty()) 
        options.data_path = DATA_PATH;
}

void Game::write_options()
{
    options.write(CONFIG_FILE);
}

void Game::setup_environment(sdlc::Screen& screen, sdlc::Timer& timer, 
                             sdlc::Mixer& mixer)
{
    UNUSED(mixer);
#ifdef WIN32
    int video_type = options.fullscreen() 
                        ? SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN 
                        : SDL_SWSURFACE;
#endif
#ifndef WIN32
    int video_type = options.fullscreen() 
                        ? SDL_SWSURFACE | SDL_FULLSCREEN
                        : SDL_SWSURFACE;
#endif

    screen.open(SCREEN_WIDTH, SCREEN_HEIGHT, 16, video_type);

    srand(timer.ticks());
    screen.set_caption("Kexx 2 " + std::string(VERSION));
    screen.show_cursor(false);

    try {
        main_font_.load(options.data_path + "fonts/font1.bmp");
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << std::endl << std::endl;
        std::cerr << "If the game data is not found in the default location";
        std::cerr << " you need to specify the data path on the command line";
        std::cerr << " with --data." << std::endl;
        std::cerr << "If you are running Kexx2 without installing you probably want to run:" << std::endl;
        std::cerr << "  src/kexx2 --data \"data/\"" << std::endl << std::endl;
        throw;
    }

    timer.delay(500);
}

void Game::start(sdlc::Mixer& mixer)
{
    game_state_ = std::unique_ptr<Menu>(new Menu(mixer, options));
}

void Game::run_logic(sdlc::Input& input, sdlc::Timer& timer, sdlc::Mixer& mixer)
{
    // here we decide which Game state that should be used
    if (game_state_ && game_state_->done()) {
        switch (game_state_->type()) {
        case EnvironmentType::menu:
            current_level_ = 1;
            if (player_state.anyone_alive())
                game_state_ = std::unique_ptr<World>(new World(mixer, timer, options, player_state, current_level_));
            else
                set_done(true);
            break;

        case EnvironmentType::buyscreen:
            game_state_ = std::unique_ptr<World> (new World(mixer, timer, options, player_state, current_level_));
            break;

        case EnvironmentType::world:
            current_level_++;

            // game complete
            if (current_level_ > options.num_of_levels() && player_state.anyone_alive()) {
                game_state_ = std::unique_ptr<Finished>(new Finished(options, player_state));
            }
            // goto inbetween levels buyscreen
            else if (player_state.anyone_alive()) {
                game_state_ = std::unique_ptr<BuyScreen>(new BuyScreen(options, player_state, current_level_));
            }
            // game over
            else {
                game_state_ = std::unique_ptr<GameOver>(new GameOver());
            }
            break;

        default:
            player_state.kill_all();
            game_state_ = std::unique_ptr<Menu>(new Menu(mixer, options));
            break;
        }
    }

    // abort in world
    if (game_state_ && game_state_->type() == EnvironmentType::world) {
        if (input.key_pressed(SDLK_ESCAPE, sdlc::AutofireKeystate::off)) {
            player_state.kill_all();
            game_state_ = std::unique_ptr<Menu>(new Menu(mixer, options));
        }
    }

#ifdef TESTING
    // used for testing
    if (input.key_pressed(SDLK_F2, sdlc::AutofireKeystate::off)) {
    }
    if (input.key_pressed(SDLK_F3, sdlc::AutofireKeystate::off)) {
        game_state_ = std::unique_ptr<World>
            (new World(mixer, timer, options, player_state, current_level_ = 1));
    }
    if (input.key_pressed(SDLK_F4, sdlc::AutofireKeystate::off)) {
        game_state_ = std::unique_ptr<BuyScreen>
            (new BuyScreen(options, player_state, current_level_));
    }
#endif

    if (game_state_)
        game_state_->run_logic(input, timer, mixer, player_state);
}

void Game::draw(sdlc::Screen& screen)
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

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

#include <iostream>
#include "Menu.h"
#include "SDLc/Timer.h"
#include "SDLc/Screen.h"
#include "SDLc/Mixer.h"
#include "SDLc/Font.h"
#include "SDLc/Input.h"
#include "SDLc/Misc.h"
#include "PlayerState.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Menu::Menu(Options& options) : IGameState(ENV_MENU), options_(options)
{
    load_data();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Menu::load_data()
{
    logo_.load(options_.data_path + "gfx/Menulogo.png");
    bg_music_.load(options_.data_path + "music/bgmusic1.xm");
    bg_music_.play(-1);
    selector_.gfx.load(options_.data_path + "gfx/EnemySideways.png");

    for (auto& b : bg_data_) {
        b.alloc(640, 480);
        precalc_bg_surface(b);
    }
}

void Menu::run_logic(sdlc::Input& input, sdlc::Timer& timer, sdlc::Mixer& mixer, 
        PlayerState& player_state)
{
    // up/down
    if (input.keyPressed(SDLK_UP, sdlc::NO_AUTOFIRE))
        selector_.pos--;
    else if (input.keyPressed(SDLK_DOWN, sdlc::NO_AUTOFIRE))
        selector_.pos++;

    // limits
    if (selector_.pos < 0) 
        selector_.pos = 0;
    if (which_menu_ == MENU_ROOT && selector_.pos > 2)
        selector_.pos = 2;
    else if (which_menu_ == MENU_OPTIONS && selector_.pos > 3)
        selector_.pos = 3;

    // when return is pressed
    if (input.keyPressed(SDLK_RETURN, sdlc::NO_AUTOFIRE)) {
        if (which_menu_ == MENU_ROOT) {
            if (selector_.pos == 0) {
                player_state.kill_all();

                player_state.set_energy_max(1, 3);
                player_state.set_extra_weapon(1, "Rocket Weapon");
                player_state.set_extra_weapon_count(1, 10);

                if (options_.num_of_players() >= 2)
                    player_state.set_energy_max(2, 3);

                done_ = true;
            } else if (selector_.pos == 1) {
                which_menu_ = MENU_OPTIONS;
                selector_.pos = 0;
            } else if (selector_.pos == 2) {
                player_state.kill_all();
                which_menu_ = MENU_EXIT;
                exit_timer_ = timer.ticks();
                mixer.fade_out_music(3000);
            }
        } else if (which_menu_ == MENU_OPTIONS) {
            if (selector_.pos == 0) {
                options_.set_num_of_players(options_.num_of_players() + 1);
                if (options_.num_of_players() > NUM_OF_POSSIBLE_PLAYERS)
                    options_.set_num_of_players(1);
            } else if (selector_.pos == 1) {
                options_.set_fullscreen(!options_.fullscreen());

#ifdef WIN32
                int screen_type;
                if (options_.fullscreen())
                    screen_type = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
                else screen_type = SDL_SWSURFACE;

                // no reinit of screen here for win32, they need to restart
                // the game for that
#endif
#ifndef WIN32
                int screen_type = SDL_SWSURFACE;
                if (options_.fullscreen())
                    screen_type |= SDL_FULLSCREEN;

                // TODO: Remove extern Screen class.
                extern sdlc::Screen* screen;
                screen->init(640, 480, 16, screen_type);
#endif
            } else if (selector_.pos == 2)
                options_.set_fps_counter(!options_.fps_counter());
            else if (selector_.pos == 3) {
                which_menu_ = MENU_ROOT;
                selector_.pos = 1;
            }
        }
    }

    // exit screen
    if (which_menu_ == MENU_EXIT && SDL_GetTicks() - exit_timer_ > 3000)
        done_ = true;

    // set option strings
    players_ = std::to_string(options_.num_of_players());

    if (options_.fullscreen()) {
        display_ = "fullscreen";
    } else { 
        display_ = "windowed";
    }

    if (options_.fps_counter()) {
        fps_ = "yes";
    } else {
        fps_ = "no";
    }
}

void Menu::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    screen.blit(0, 0, bg_data_[rand() % 10]);

    if (which_menu_ != MENU_EXIT) {
        screen.blit(150, 60, logo_);
        screen.blit(230, 200 + selector_.pos * 30, selector_.gfx);
    }

    /////////
    if (which_menu_ == MENU_ROOT) {
        screen.print(270, 200, "start", font);
        screen.print(270, 230, "options", font);
        screen.print(270, 260, "exit", font);
    } else if (which_menu_ == MENU_OPTIONS) {
        screen.print(270, 200, "players: " + players_, font);
        screen.print(270, 230, "display: " + display_, font);
        screen.print(270, 260, "show fps: " + fps_, font);
        screen.print(270, 300, "return", font);

#ifdef WIN32
        if (selector_.pos == 1)
            screen.print(230, 465, "changes requires a restart", 0, 0, 255);
#endif
    } else if (which_menu_ == MENU_EXIT) {
        static bool tmpB = false;
        if (tmpB == false)
            sdl_logo_.load(options_.data_path + "gfx/SDLnow.png");
        tmpB = true;
        screen.blit(640 - sdl_logo_.getWidth(), 480 - sdl_logo_.getHeight(), sdl_logo_);

        int x = 170, y = 100;
        screen.print(x, y, "programming by:", font);
        screen.print(x + 40, y + 30, "jon haggblad", font);

        screen.print(x, y + 70, "additional work by:", font);
        screen.print(x + 40, y + 100, "birger hedman", font);
        screen.print(x + 40, y + 120, "petter ulfberg", font);

        screen.print(x, y + 160, "special thanks to:", font);
        screen.print(x + 40, y + 190, "jesper rojestal", font);
        screen.print(x + 40, y + 210, "john degerman", font);
        screen.print(x + 40, y + 230, "markus haggblad", font);
    } else {
        std::cout << "Menu::draw() undefined 'which_menu'" << std::endl;
    }

    std::string v = VERSION;
    screen.print(5, 455, "v " + v, font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Menu::precalc_bg_surface(sdlc::Surface& surface)
{
    int iy, ix;
    for (iy = 0; iy < 40; iy++)
        for (ix = 0; ix < 8; ix++)
            draw_group(ix * 80 + 5, iy * 12 + 2, surface);
}

void Menu::draw_group(int x, int y, sdlc::Surface& surface)
{
    int i;
    for (i = 0; i < 9; i++) {
        /*
          //char c = (rand()%127);
          char c = '0'+(rand()%127);
          string s;
          s = s + c;
          surface.print(x+(i*8), y, s, 100, 100, 100);
        */
        surface.print(x + (i * 8), y, std::to_string((rand() % 10)), 80, 80, 80);
    }
}

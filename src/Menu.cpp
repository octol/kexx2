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
#include "SDLc/Font.h"
#include "SDLc/Input.h"
#include "SDLc/Misc.h"
#include "PlayerState.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Menu::Menu(Options& options_) : IGameState(ENV_MENU)
{
    options = &options_;

    logo.load(options->data_path + "gfx/Menulogo.png");
    bgmusic.load(options->data_path + "music/bgmusic1.xm");
    bgmusic.play(-1);
    whichMenu = MENU_ROOT;
    selector.gfx.load(options->data_path + "gfx/EnemySideways.png");
    selector.pos = 0;
    exittimer = 0;

    int i;
    for (i = 0; i < 10; i++) {
        bgdata[i].alloc(640, 480);
        precalcbgsurface(bgdata[i]);
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------
void Menu::run_logic(Input& input, Timer& timer, PlayerState& player_state)
{
    // up/down
    if (input.keyPressed(SDLK_UP, NO_AUTOFIRE))
        selector.pos--;
    else if (input.keyPressed(SDLK_DOWN, NO_AUTOFIRE))
        selector.pos++;

    // limits
    if (selector.pos < 0) selector.pos = 0;
    if (whichMenu == MENU_ROOT && selector.pos > 2)
        selector.pos = 2;
    else if (whichMenu == MENU_OPTIONS && selector.pos > 3)
        selector.pos = 3;

    // when return is pressed
    if (input.keyPressed(SDLK_RETURN, NO_AUTOFIRE)) {
        if (whichMenu == MENU_ROOT) {
            if (selector.pos == 0) {
                player_state.killall();

                player_state.setEnergyMax(1, 3);
                //player_state.setExtraWeapon(1, WEAPON_EXTRA_ROCKET);
                player_state.setExtraWeapon(1, "Rocket Weapon");
                player_state.setExtraWeaponCount(1, 10);

                if (options->num_of_players() >= 2)
                    player_state.setEnergyMax(2, 3);

                done_ = true;
            } else if (selector.pos == 1) {
                whichMenu = MENU_OPTIONS;
                selector.pos = 0;
            } else if (selector.pos == 2) {
                player_state.killall();
                whichMenu = MENU_EXIT;
                exittimer = SDL_GetTicks();
                Mix_FadeOutMusic(3000);
            }
        } else if (whichMenu == MENU_OPTIONS) {
            if (selector.pos == 0) {
                options->set_num_of_players(options->num_of_players() + 1);
                if (options->num_of_players() > NUM_OF_POSSIBLE_PLAYERS)
                    options->set_num_of_players(1);
            } else if (selector.pos == 1) {
                options->set_fullscreen(!options->fullscreen());

#ifdef WIN32
                int type;
                if (options->fullscreen())
                    type = SDL_HWSURFACE | SDL_DOUBLEBUF | SDL_FULLSCREEN;
                else type = SDL_SWSURFACE;

                // no reinit of screen here for win32, they need to restart the game for that
#endif
#ifndef WIN32
                int type = SDL_SWSURFACE;
                if (options->fullscreen())
                    type |= SDL_FULLSCREEN;

                extern Screen* screen;
                screen->init(640, 480, 16, type);
#endif
            } else if (selector.pos == 2)
                options->set_fps_counter(!options->fps_counter());
            else if (selector.pos == 3) {
                whichMenu = MENU_ROOT;
                selector.pos = 1;
            }
        }
    }

    // exit screen
    if (whichMenu == MENU_EXIT && SDL_GetTicks() - exittimer > 3000)
        done_ = true;

    // set option strings
    players = std::to_string(options->num_of_players());

    if (options->fullscreen()) {
        display = "fullscreen";
    } else { 
        display = "windowed";
    }

    if (options->fps_counter()) {
        fps = "yes";
    } else {
        fps = "no";
    }
}

void Menu::draw(Screen& screen, Font& font)
{
    screen.blit(0, 0, bgdata[rand() % 10]);

    if (whichMenu != MENU_EXIT) {
        screen.blit(150, 60, logo);
        screen.blit(230, 200 + selector.pos * 30, selector.gfx);
    }

    /////////
    if (whichMenu == MENU_ROOT) {
        screen.print(270, 200, "start", font);
        screen.print(270, 230, "options", font);
        screen.print(270, 260, "exit", font);
    } else if (whichMenu == MENU_OPTIONS) {
        screen.print(270, 200, "players: " + players, font);;
        screen.print(270, 230, "display: " + display, font);
        screen.print(270, 260, "show fps: " + fps, font);
        screen.print(270, 300, "return", font);

#ifdef WIN32
        if (selector.pos == 1)
            screen.print(230, 465, "changes requires a restart", 0, 0, 255);
#endif
    } else if (whichMenu == MENU_EXIT) {
        static bool tmpB = false;
        if (tmpB == false)
            sdllogo.load(options->data_path + "gfx/SDLnow.png");
        tmpB = true;
        screen.blit(640 - sdllogo.getWidth(), 480 - sdllogo.getHeight(), sdllogo);

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
        std::cout << "Menu::draw() undefined 'whichMenu'" << std::endl;
    }

    std::string v = VERSION;
    screen.print(5, 455, "v " + v, font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Menu::precalcbgsurface(Surface& surface)
{
    int iy, ix;
    for (iy = 0; iy < 40; iy++)
        for (ix = 0; ix < 8; ix++)
            drawgroup(ix * 80 + 5, iy * 12 + 2, surface);
}

void Menu::drawgroup(int x, int y, Surface& surface)
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

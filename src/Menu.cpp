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
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Menu::Menu(Options& options_)
{
    options = &options_;
    env_type_ = ENV_MENU;

    logo.load(options->dataPath + "gfx/Menulogo.png");
    bgmusic.load(options->dataPath + "music/bgmusic1.xm");
    bgmusic.play(-1);
    whichMenu = MENU_ROOT;
    selector.gfx.load(options->dataPath + "gfx/EnemySideways.png");
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
void Menu::runLogic(Timer& timer, PlayerState& playerState)
{
    extern Input* input;

    // up/down
    if (input->keyPressed(SDLK_UP, NO_AUTOFIRE))
        selector.pos--;
    else if (input->keyPressed(SDLK_DOWN, NO_AUTOFIRE))
        selector.pos++;

    // limits
    if (selector.pos < 0) selector.pos = 0;
    if (whichMenu == MENU_ROOT && selector.pos > 2)
        selector.pos = 2;
    else if (whichMenu == MENU_OPTIONS && selector.pos > 3)
        selector.pos = 3;

    // when return is pressed
    if (input->keyPressed(SDLK_RETURN, NO_AUTOFIRE)) {
        if (whichMenu == MENU_ROOT) {
            if (selector.pos == 0) {
                playerState.killall();

                playerState.setEnergyMax(1, 3);
                //playerState.setExtraWeapon(1, WEAPON_EXTRA_ROCKET);
                playerState.setExtraWeapon(1, "Rocket Weapon");
                playerState.setExtraWeaponCount(1, 10);

                if (options->getHowManyPlayers() >= 2)
                    playerState.setEnergyMax(2, 3);

                done_ = true;
            } else if (selector.pos == 1) {
                whichMenu = MENU_OPTIONS;
                selector.pos = 0;
            } else if (selector.pos == 2) {
                playerState.killall();
                whichMenu = MENU_EXIT;
                exittimer = SDL_GetTicks();
                Mix_FadeOutMusic(3000);
            }
        } else if (whichMenu == MENU_OPTIONS) {
            if (selector.pos == 0) {
                options->setHowManyPlayers(options->getHowManyPlayers() + 1);
                if (options->getHowManyPlayers() > NUM_OF_POSSIBLE_PLAYERS)
                    options->setHowManyPlayers(1);
            } else if (selector.pos == 1) {
                options->fullscreen(!options->fullscreen());

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
                options->fpsCounter(!options->fpsCounter());
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
    players = std::to_string(options->getHowManyPlayers());

    if (options->fullscreen()) {
        display = "fullscreen";
    } else { 
        display = "windowed";
    }

    if (options->fpsCounter()) {
        fps = "yes";
    } else {
        fps = "no";
    }
}

void Menu::draw(Screen& screen, Font& mainFont)
{
    screen.blit(0, 0, bgdata[rand() % 10]);

    if (whichMenu != MENU_EXIT) {
        screen.blit(150, 60, logo);
        screen.blit(230, 200 + selector.pos * 30, selector.gfx);
    }

    /////////
    if (whichMenu == MENU_ROOT) {
        screen.print(270, 200, "start", mainFont);
        screen.print(270, 230, "options", mainFont);
        screen.print(270, 260, "exit", mainFont);
    } else if (whichMenu == MENU_OPTIONS) {
        screen.print(270, 200, "players: " + players, mainFont);;
        screen.print(270, 230, "display: " + display, mainFont);
        screen.print(270, 260, "show fps: " + fps, mainFont);
        screen.print(270, 300, "return", mainFont);

#ifdef WIN32
        if (selector.pos == 1)
            screen.print(230, 465, "changes requires a restart", 0, 0, 255);
#endif
    } else if (whichMenu == MENU_EXIT) {
        static bool tmpB = false;
        if (tmpB == false)
            sdllogo.load(options->dataPath + "gfx/SDLnow.png");
        tmpB = true;
        screen.blit(640 - sdllogo.getWidth(), 480 - sdllogo.getHeight(), sdllogo);

        int x = 170, y = 100;
        screen.print(x, y, "programming by:", mainFont);
        screen.print(x + 40, y + 30, "jon haggblad", mainFont);

        screen.print(x, y + 70, "additional work by:", mainFont);
        screen.print(x + 40, y + 100, "birger hedman", mainFont);
        screen.print(x + 40, y + 120, "petter ulfberg", mainFont);

        screen.print(x, y + 160, "special thanks to:", mainFont);
        screen.print(x + 40, y + 190, "jesper rojestal", mainFont);
        screen.print(x + 40, y + 210, "john degerman", mainFont);
        screen.print(x + 40, y + 230, "markus haggblad", mainFont);
    } else cout << "Menu::draw() undefined 'whichMenu'" << endl;

    string v = VERSION;
    screen.print(5, 455, "v " + v, mainFont);
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

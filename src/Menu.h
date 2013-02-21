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

#ifndef _KEXX2_MENU_H_
#define _KEXX2_MENU_H_

#include "IGameState.h"
#include "SDLc/Surface.h"
#include "SDLc/Music.h"

class Timer;
class Screen;
class Font;
class PlayerState;
class Options;

enum MENU_WHICH {
    MENU_ROOT,
    MENU_OPTIONS,
    MENU_EXIT
};

class Menu : public IGameState {
public:
    Menu(Options& options_);
    virtual ~Menu() {};

    void runLogic(Timer& timer, PlayerState& playerState);
    void draw(Screen& screen, Font& mainFont);
private:
    void precalcbgsurface(Surface& surface);
    void drawgroup(int x, int y, Surface& surface);

    Surface logo;
    Surface sdllogo;
    Surface bgdata[10];
    Music bgmusic;
    MENU_WHICH whichMenu;

    struct Selector {
        Surface gfx;
        int pos;
    } selector;

    Options* options;
    std::string players, display, fps;
    int exittimer;
};

#endif

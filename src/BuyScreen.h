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

#ifndef _KEXX_BUYSCREEN_H_
#define _KEXX_BUYSCREEN_H_

#include "Environment.h"
#include "SDLc/Surface.h"

class Options;
class PlayerState;

const int ROCKET_COST = 1000;

class BuyScreen : public Environment {
public:
    BuyScreen(Options& options, PlayerState& playerState_, int currentLevel_);
    virtual ~BuyScreen();

    void runLogic(Timer& Timer, PlayerState& playerState);
    void draw(Screen& Screen, Font& mainFont);

private:
    int currentLevel;
    int howManyPlayers;
    PlayerState* playerState;

    struct Selector {
        Surface gfx;
        int pos[2];
    } selectors;

    bool playerdone[2];
};

#endif

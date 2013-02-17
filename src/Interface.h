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

#ifndef _KEXX2_INTERFACE_H_
#define _KEXX2_INTERFACE_H_

#include "Defines.h"
#include "PlayerState.h"

class Font;
class Screen;

class Interface {
public:
    Interface();
    virtual ~Interface();

    void update(int numOfPlayers_, PlayerState& playerState);
    void draw(Font& font, Screen& screen);

private:
    void drawEnergy(int x, int y, int value, int maxValue, Font& font, Screen& screen);
    void drawScore(int x, int y, int value, Font& font, Screen& screen);
    void drawWeapons(int x, int y, PlayerState& ps, int player, Font& font, Screen& screen);

    //int score[NUM_OF_POSSIBLE_PLAYERS];
    //int energy[NUM_OF_POSSIBLE_PLAYERS];
    //int energyMax[NUM_OF_POSSIBLE_PLAYERS];
    PlayerState playerStateCopy;
    int numOfPlayers;
};

#endif

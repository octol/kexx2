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

#ifndef KEXX_BUYSCREEN_H
#define KEXX_BUYSCREEN_H

#include "SDLc/Surface.h"
#include "IGameState.h"

class Options;
class PlayerState;

const int ROCKET_COST = 1000;

class BuyScreen : public IGameState {
public:
    BuyScreen(Options& options, int current_level);
    virtual ~BuyScreen() {};

    void run_logic(Input& input, Timer& Timer, Mixer& mixer,
                   PlayerState& player_state) override;
    void draw(Screen& Screen, Font& font);

private:
    int currentLevel_;
    int howManyPlayers_;
    PlayerState* playerState_;

    struct Selector {
        Surface gfx;
        int pos[2];
    } selectors_;

    bool playerdone_[2];
};

#endif // KEXX2_BUYSCREEN_H

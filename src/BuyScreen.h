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

#include "SDLc.h"
#include "IGameState.h"

class Options;
class PlayerState;

const int ROCKET_COST = 1000;

class BuyScreen final : public IGameState {
public:
    BuyScreen(Options& options, int current_level);
    virtual ~BuyScreen() {};

    void run_logic(sdlc::Input& input, sdlc::Timer& timer, sdlc::Mixer& mixer,
                   PlayerState& player_state) override;
    void draw(sdlc::Screen& screen, sdlc::Font& font) override;

private:
    int current_level_;
    int how_many_players_;

    // TODO: move to smart pointer.
    PlayerState* player_state_;

    struct Selector {
        sdlc::Surface gfx;
        int pos[2] = { 0, 0 };
    } selectors_;

    bool player_done_[2] = { false, false };
};

#endif // KEXX2_BUYSCREEN_H

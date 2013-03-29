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

#include <vector>
#include <array>
#include "SDLc.h"
#include "IGameState.h"
#include "Defines.h"

class Options;
class PlayerState;

const int ROCKET_COST = 1000;

class BuyScreen final : public IGameState {
public:
    BuyScreen(Options&, PlayerState&, int current_level);

    void run_logic(sdlc::Input&, sdlc::Timer&, sdlc::Mixer&, PlayerState&) override;
    void draw(sdlc::Screen&, sdlc::Font&) override;

private:
    void press_enter(int i, PlayerState&);
    void draw_player_info(int i, sdlc::Screen& screen, sdlc::Font& font);
    void draw_extra_weapon(int i, sdlc::Screen&, sdlc::Font&);

    PlayerState& player_state_;

    int current_level_ = 0;
    const int num_of_players_ = 0;

    // If the players are finished in the buy screen
    std::array<bool,2> player_done_ = {{ false, false }};

    struct Selector {
        sdlc::Surface gfx;
        std::vector<int> pos;
    } selectors_;
};

#endif // KEXX2_BUYSCREEN_H

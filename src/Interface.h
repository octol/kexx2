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

#ifndef KEXX2_INTERFACE_H
#define KEXX2_INTERFACE_H

#include "SDLc.h"
#include "Defines.h"
#include "PlayerState.h"

class Interface final {
public:
    void update(int num_of_players, const PlayerState& player_state);
    void draw(sdlc::Font& font, sdlc::Screen& screen);

private:
    void draw_energy(int x, int y, int value, int max_value, 
                     sdlc::Font&, sdlc::Screen&);
    void draw_score(int x, int y, int value, 
                    sdlc::Font&, sdlc::Screen&);
    void draw_weapons(int x, int y, PlayerState& ps, int player, 
                      sdlc::Font&, sdlc::Screen&);

    // TODO: switch to reference for PlayerState?
    PlayerState player_state_;
    int num_of_players_ = 0;
};

#endif

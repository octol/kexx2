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

#ifndef KEXX2_MENU_H
#define KEXX2_MENU_H

#include "IGameState.h"
#include "SDLc.h"
#include <array>

class PlayerState;
class Options;

enum MenuWhich {
    MENU_ROOT,
    MENU_OPTIONS,
    MENU_EXIT
};

class Menu final : public IGameState {
public:
    Menu(Options& options);
    virtual ~Menu() {};

    void load_data();

    void run_logic(sdlc::Input& input, sdlc::Timer& timer, sdlc::Mixer& mixer, 
                   PlayerState& player_state) override;
    void draw(sdlc::Screen& screen, sdlc::Font& font) override;

private:
    void precalc_bg_surface(sdlc::Surface& surface);
    void draw_group(int x, int y, sdlc::Surface& surface);

    sdlc::Surface logo_;
    sdlc::Surface sdl_logo_;
    std::array<sdlc::Surface,10> bg_data_;
    sdlc::Music bg_music_;
    MenuWhich which_menu_ = MENU_ROOT;

    struct Selector {
        sdlc::Surface gfx;
        int pos = 0;
    } selector_;

    Options& options_;
    std::string players_;
    std::string display_;
    std::string fps_;
    int exit_timer_ = 0;
};

#endif // KEXX2_MENU_H

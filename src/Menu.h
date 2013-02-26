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
#include "SDLc/Surface.h"
#include "SDLc/Music.h"
#include <array>

class Timer;
class Screen;
class Font;
class PlayerState;
class Options;

enum MenuWhich {
    MENU_ROOT,
    MENU_OPTIONS,
    MENU_EXIT
};

class Menu : public IGameState {
public:
    Menu(Options& options);
    virtual ~Menu() {};

    void load_data();

    void run_logic(Input& input, Timer& timer, Mixer& mixer, 
                   PlayerState& player_state) override;
    void draw(Screen& screen, Font& font) override;

private:
    void precalcbgsurface(Surface& surface);
    void drawgroup(int x, int y, Surface& surface);

    Surface logo_;
    Surface sdl_logo_;
    std::array<Surface,10> bg_data_;
    Music bg_music_;
    MenuWhich which_menu_ = MENU_ROOT;

    struct Selector {
        Surface gfx;
        int pos = 0;
    } selector_;

    Options& options_;
    std::string players_;
    std::string display_;
    std::string fps_;
    int exit_timer_ = 0;
};

#endif // KEXX2_MENU_H

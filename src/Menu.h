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


class Menu final : public IGameState {
public:
    Menu(sdlc::Mixer&, Options&);
    virtual ~Menu() {};

    void load_data(sdlc::Mixer&);

    void run_logic(sdlc::Input&, sdlc::Timer&, sdlc::Mixer&, PlayerState&) override;
    void draw(sdlc::Screen&, sdlc::Font&) override;

private:
    void root_menu_logic(sdlc::Timer&, sdlc::Mixer&, PlayerState&);
    void options_menu_logic();

    void draw_root_screen(sdlc::Screen&, sdlc::Font&);
    void draw_options_screen(sdlc::Screen&, sdlc::Font&);
    void draw_exit_screen(sdlc::Screen&, sdlc::Font&);

    void precalc_bg_surface(sdlc::Surface&);
    void draw_group(int x, int y, sdlc::Surface&);

    sdlc::Surface logo_;
    sdlc::Surface sdl_logo_;
    std::array<sdlc::Surface,10> bg_data_;
    sdlc::Music bg_music_;

    enum class State { root, options, exit };

    State which_menu_ = State::root;

    struct Selector {
        sdlc::Surface gfx;
        int pos = 0;
    } selector_;

    Options& options_;
    std::string players_;
    std::string display_;
    std::string fps_;

    int exit_timer_ = 0;

    bool toggle_screen_ = false;
    int screen_type_ = 0;   // used to toogle fullscreen
};

#endif // KEXX2_MENU_H

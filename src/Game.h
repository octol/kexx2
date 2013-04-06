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

#ifndef KEXX2_GAME_H
#define KEXX2_GAME_H

#include <memory>

#include "SDLc.h"
#include "Options.h"
#include "PlayerState.h"
#include "IGameState.h"

class Game final {
public:
    // Initialisation functions.
    void load_options();
    void write_options();
    void setup_environment(sdlc::Screen&, sdlc::Timer&, sdlc::Mixer&);
    void close_environment(sdlc::Screen&, sdlc::Timer&, sdlc::Mixer&);
    void start();

    // Functions called in the game loop.
    void run_logic(sdlc::Input&, sdlc::Timer&, sdlc::Mixer&);
    void draw(sdlc::Screen& screen);

    bool done() const;
    bool set_done(bool value);

    Options options;
    PlayerState player_state;

private:
    std::unique_ptr<IGameState> game_state_;
    sdlc::Font main_font_;

    int current_level_ = 0;
    bool done_ = false;
};

#endif // KEXX2_GAME_H

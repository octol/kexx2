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

#ifndef _KEXX2_GAME_H_
#define _KEXX2_GAME_H_

#include "Options.h"
#include "PlayerState.h"
#include "SDLc/Font.h"

class Screen;
class Mixer;
class Timer;
class Environment;

class Game {
public:
    Game();
    Game(const Game& game) = delete;
    Game& operator = (const Game& game) = delete;
    virtual ~Game();

    // Initialisation functions.
    void loadOptions();
    void writeOptions();
    void setupEnvironment(Screen& screen, Timer& timer, Mixer& mixer);
    void start();

    // Functions called in the game loop.
    void runLogic(Timer& timer);
    void draw(Screen& screen);

    bool done() const;
    bool done(bool value);

    Options options;
    PlayerState playerState;

private:
    Environment* environment = nullptr;
    Font mainFont;

    int currentLevel = 0;
    bool m_done = false;
};

#endif

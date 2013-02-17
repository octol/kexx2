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

#include "SDLc/Screen.h"
#include "SDLc/Timer.h"
#include "SDLc/Input.h"
#include "SDLc/Mixer.h"
#include "SDLc/Misc.h"
#include "Game.h"

Screen* screen;
Timer* timer;
Input* input;
Mixer* mixer;

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    // System subsystems
    screen = new Screen;
    timer = new Timer;
    input = new Input;
    mixer = new Mixer;
    
    // Main game object
    Game* kexx2 = new Game;

    kexx2->loadOptions();
    kexx2->setupEnvironment(*screen, *timer, *mixer);
    kexx2->start();

    while (!kexx2->done()) {
        input->update();

        // Developer mode escape key.
        if (input->keyPressed(SDLK_F1, NO_AUTOFIRE)) {
            kexx2->done(true);
        }

        kexx2->runLogic(*timer);
        kexx2->draw(*screen);

        if (kexx2->options.fpsCounter()) {
            static int FPS = static_cast<int>(timer->getFPS() + 0.5f);
            static int ticks = timer->getTicks();
            if ((timer->getTicks() - ticks) > 500) {
                FPS = static_cast<int>(timer->getFPS() + 0.5f);
                ticks = timer->getTicks();
            }
            screen->print(5, 5, "FPS: " + std::to_string(FPS), 255, 255, 255);
        }

        screen->flipAll();
        screen->fillRect(0, 0, 640, 480, 0, 0, 0);
        timer->update();
    }

    kexx2->writeOptions();
    delete kexx2;
    delete mixer;
    delete input;
    delete timer;
    delete screen;
    return 0;
}


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
#include "Defines.h"

sdlc::Screen* screen;
sdlc::Timer* timer;
sdlc::Input* input;
sdlc::Mixer* mixer;

void print_fps_counter(sdlc::Screen&, sdlc::Timer&);

int main(int argc, char* argv[])
{
    UNUSED(argc);
    UNUSED(argv);

    // System subsystems
    screen = new sdlc::Screen;
    timer = new sdlc::Timer;
    input = new sdlc::Input;
    mixer = new sdlc::Mixer;
    
    auto kexx2 = std::unique_ptr<Game>(new Game);
    kexx2->load_options();
    kexx2->setup_environment(*screen, *timer, *mixer);
    kexx2->start();

    while (!kexx2->done()) {
        input->update();

#ifdef TESTING
        // Developer mode escape key.
        if (input->key_pressed(SDLK_F1, sdlc::NO_AUTOFIRE)) 
            kexx2->set_done(true);
#endif

        kexx2->run_logic(*input, *timer, *mixer);
        kexx2->draw(*screen);

        if (kexx2->options.fps_counter()) 
            print_fps_counter(*screen, *timer);

        screen->flip_all();
        screen->fill_rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0, 0);
        timer->update();
    }

    kexx2->write_options();
    kexx2.reset();

    delete mixer;
    delete input;
    delete timer;
    delete screen;
    return 0;
}

void print_fps_counter(sdlc::Screen& screen_, sdlc::Timer& timer_)
{
    static int FPS = static_cast<int>(timer_.fps() + 0.5f);
    static int ticks = timer_.ticks();
    if ((timer_.ticks() - ticks) > 500) {
        FPS = static_cast<int>(timer_.fps() + 0.5f);
        ticks = timer_.ticks();
    }
    screen_.print(5, 5, "FPS: " + std::to_string(FPS), 255, 255, 255);
}

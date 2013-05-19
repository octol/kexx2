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
#include "Game.h"
#include "Defines.h"

#include "config.h"

#include "boost/program_options.hpp"

// TODO: remove global system classes
sdlc::Screen* screen;
sdlc::Timer* timer;
sdlc::Input* input;
sdlc::Mixer* mixer;

void print_fps_counter(sdlc::Screen&, sdlc::Timer&);
std::string version_info();

int main(int argc, char* argv[])
{
    std::string data_path;

    // Parse command line options
    try {
        namespace po = boost::program_options;
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help,h", "produce help message")
            ("version,v", "print version number")
            ("data,d", po::value<std::string>(&data_path), "path to data resources");
        po::variables_map vm;

        try {
            po::store(po::parse_command_line(argc, argv, desc), vm);
            // --help
            if (vm.count("help")) {
                std::cout << desc << std::endl;
                return EXIT_SUCCESS;
            }
            // as this can throw, do it after -h
            po::notify(vm);    

        } catch (const boost::program_options::required_option& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        } catch (const boost::program_options::error& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return EXIT_FAILURE;
        }

        // --version
        if (vm.count("version")) {
            std::cout << version_info();
            return EXIT_SUCCESS;
        }

    } catch(const std::exception& e) {
        std::cerr << "Error parsing command line options." << std::endl;
        return EXIT_FAILURE;
    }

    // add a trailing slash if needed.
    if (data_path != "" && *(--end(data_path)) != '/') 
        data_path += "/";

    // System subsystems
    sdlc::init();
    screen = new sdlc::Screen;
    timer = new sdlc::Timer;
    input = new sdlc::Input;
    mixer = new sdlc::Mixer;
    
    auto kexx2 = std::unique_ptr<Game>(new Game);
    try {
        kexx2->load_options(data_path);
        kexx2->setup_environment(*screen, *timer, *mixer);
        kexx2->start(*mixer);
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize, quitting..." << std::endl;
        kexx2.reset();
        delete mixer;
        delete input;
        delete timer;
        delete screen;
        sdlc::quit();
        return EXIT_FAILURE;
    }

    while (!kexx2->done()) {
        input->update();

#ifdef TESTING
        // Developer mode escape key.
        if (input->key_pressed(SDLK_F1, sdlc::AutofireKeystate::off)) 
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
    sdlc::quit();
    return EXIT_SUCCESS;
}

void print_fps_counter(sdlc::Screen& screen_, sdlc::Timer& timer_)
{
    static int FPS = static_cast<int>(timer_.fps() + 0.5f);
    static int ticks = timer_.ticks();
    if ((timer_.ticks() - ticks) > 500) {
        FPS = static_cast<int>(timer_.fps() + 0.5f);
        ticks = timer_.ticks();
    }
    //screen_.print(5, 5, "FPS: " + std::to_string(FPS), 255, 255, 255);
    screen_.print(5, 5, "FPS: " + SSTR(FPS), 255, 255, 255);
}

std::string version_info() 
{
    std::string str(PACKAGE_STRING);
    str += "\nCopyright (C) Jon Haggblad 2001, 2013 <";
    str += std::string(PACKAGE_BUGREPORT) + ">\n";
    str += std::string(PACKAGE_URL) + "\n\n";
    str += "License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>\n";
    str += "This is free software; you are free to change and redistribute it.\n";
    str += "There is NO WARRANTY, to the extent permitted by law.\n";
    return str;
}


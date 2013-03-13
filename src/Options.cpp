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

#include "Options.h"

#include <fstream>
#include <iostream>
#include <vector>

void Options::load(std::string path)
{
    int file_players, file_fullscreen, file_fps_counter;

    std::ifstream options_file(path);
    if (!options_file) {
        std::cerr << "Error opening " << path << " for reading, ";
        std::cerr << "using default options!" << std::endl;
    } else {
        options_file.ignore(80, '=');
        options_file >> file_players;
        options_file.ignore(80, '=');
        options_file >> file_fullscreen;
        options_file.ignore(80, '=');
        options_file >> file_fps_counter;
    }
    options_file.close();

    set_num_of_players(file_players);
    set_fullscreen(file_fullscreen == 1);
    set_fps_counter(file_fps_counter == 1);
}

void Options::write(std::string path)
{
    // set up the information in variables
    int file_players = num_of_players();
    int file_fullscreen = fullscreen() ? 1 : 0;
    int file_fps_counter = fps_counter() ? 1 : 0; 

    // write the information to disk
    std::ofstream options_file(path.c_str());
    if (!options_file) {
        std::cerr << "Error writing to: " << path << std::endl;
    } else {
        options_file << "NumPlayers=" << file_players << "\n";
        options_file << "Fullscreen=" << file_fullscreen << "\n";
        options_file << "FPSCounter=" << file_fps_counter << "\n";
    }
    options_file.close();
}

int Options::set_num_of_players(int value)
{
    if (value < 0) {
        std::cerr << "Warning: Options::set_num_of_players() negative!" 
            << std::endl;
    }
    return num_of_players_ = value;
}

int Options::num_of_levels()
{
    std::vector<std::string> level_filename;
    std::ifstream levels_file(data_path + "levels/levels.cfg");

    std::string tmp_filename;
    while (!levels_file.eof()) {
        levels_file >> tmp_filename;
        level_filename.push_back(tmp_filename);
    }

    levels_file.close();
    return level_filename.size() - 1; // number of levels
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

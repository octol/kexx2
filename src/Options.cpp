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
#include <string>
#include <vector>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

void Options::load(std::string path)
{
    int players = 1;
    int fullscreen = 0;
    int fps_counter = 1;
    std::string line;

#ifdef WIN32
    // Since we (so far) don't store settings in Windows, always assue
    // fullscreen as default.
    fullscreen = 1;
#endif

    std::ifstream file(path);
    if (!file) {
        std::clog << "Note: " << path << " not found, using defaults." << std::endl;
    } else {
        while (std::getline(file, line)) {
            std::vector<std::string> strs;
            boost::split(strs, line, boost::is_any_of("\t ="));
            if (!(strs.size() == 2))
                std::cerr << "Error reading file format: " << path << std::endl;

            if (strs.at(0) == "players") {
                players = stoi(strs.at(1));
            } else if (strs.at(0) == "fullscreen") {
                fullscreen = stoi(strs.at(1));
            } else if (strs.at(0) == "fps_counter") {
                fps_counter = stoi(strs.at(1));
            } else if (strs.at(0) == "data_path") {
                data_path = strs.at(1);
            } else {
                std::cerr << "Uknown parameter in: " << path << std::endl;
            }
        }
    }
   
    set_num_of_players(players);
    set_fullscreen(fullscreen == 1);
    set_fps_counter(fps_counter == 1);
}

void Options::write(std::string path)
{
    // set up the information in variables
    int file_players = num_of_players();
    int file_fullscreen = fullscreen() ? 1 : 0;
    int file_fps_counter = fps_counter() ? 1 : 0; 

    // write the information to disk
    if (!boost::filesystem::exists(path)) {
        std::clog << "Creating new using default values: " << path << std::endl;
    }
    std::ofstream options_file(path.c_str());
    if (!options_file) {
        std::cerr << "Error writing to: " << path << std::endl;
    } else {
        options_file << "players=" << file_players << "\n";
        options_file << "fullscreen=" << file_fullscreen << "\n";
        options_file << "fps_counter=" << file_fps_counter << "\n";
        if (data_path != DATA_PATH) 
            options_file << "data_path=" << data_path << "\n";
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
    return int(level_filename.size()) - 1; // number of levels
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

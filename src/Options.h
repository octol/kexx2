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

#ifndef KEXX2_OPTIONS_H
#define KEXX2_OPTIONS_H

#include <string>
#include "Defines.h"

class Options final {
public:
    void load(std::string path);
    void write(std::string path);

    std::string data_path = "";

    bool fullscreen();
    bool set_fullscreen(bool value);

    bool fps_counter();
    bool set_fps_counter(bool value);

    int num_of_players();
    int set_num_of_players(int value);

    int num_of_levels();

private:
    bool fullscreen_ = false;
    bool fps_counter_ = false;
    int num_of_players_ = 0;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
bool Options::fullscreen() 
{
    return fullscreen_;
}

inline
bool Options::set_fullscreen(bool value) 
{
    return fullscreen_ = value;
}

inline
bool Options::fps_counter() 
{
    return fps_counter_;
}

inline
bool Options::set_fps_counter(bool value) 
{
    return fps_counter_ = value;
}

inline
int Options::num_of_players() 
{
    return num_of_players_;
}

#endif // KEXX2_OPTIONS_H

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

#ifndef KEXX2_DEFINES_H
#define KEXX2_DEFINES_H

#include <string>

#define UNUSED(x) ((void)x)
//#define TESTING

const std::string DATA_PATH = std::string(DATADIR) + "/kexx2/";
const std::string CONFIG_FILE = std::string(getenv("HOME")) + "/.kexx2.conf";
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float SCROLLING_SPEED = 50.0f; 
const int NUM_OF_POSSIBLE_PLAYERS = 2;

#endif // KEXX2_DEFINES_H

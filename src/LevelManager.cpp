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

#include "LevelManager.h"

#include <fstream>
#include <iostream>
#include <vector>
#include "ObjectManager.h"
#include "World.h"

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void LevelManager::load_level(std::string data_path, int level, 
                              ObjectManager& object_manager)
{
    // get level filenames first
    std::vector<std::string> levels;
    std::ifstream levels_file((data_path + "levels/levels.cfg").c_str());

    // load level files from level list file
    std::string level_file;
    while (levels_file) {
        levels_file >> level_file;
        levels.push_back(level_file);
    }

    parse_level(data_path + "levels/" + levels[level - 1], object_manager);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

bool LevelManager::parse_level(std::string level_name, 
                              ObjectManager& object_manager)
{
    std::string type;
    int x, y;

    // open file for reading
    std::ifstream level_file(level_name.c_str());
    if (!level_file) {
        std::cerr << "Error opening: " << level_name << std::endl;
        return false;
    } else {
        while (level_file) {
            level_file >> type;
            if (level_file.eof())
                break;

            ObjIndex obj = Object::parse_obj_index(type);

            level_file >> x;
            level_file >> y;

            object_manager.create_object(x, -y, 0.0f, 0.0f, obj, Owner::none);
        }
    }

    return true;
}


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
    // TODO: make this more elegant.
    // -------------------------
    // get level filenames first
    std::vector<std::string> level_filename;
    int i = 0;
    std::ifstream levels_file((data_path + "levels/levels.cfg").c_str());

    std::string tmp_filename;
    level_filename.clear();

    while (!levels_file.eof()) {
        levels_file >> tmp_filename;
        level_filename.push_back(tmp_filename);
        i++;

        if (levels_file.eof())
            break;
    }

    levels_file.close();
    // -------------------------

    internal_load_level(data_path + "levels/" + level_filename[level - 1], 
                        object_manager);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

bool LevelManager::internal_load_level(std::string level_name, 
                                       ObjectManager& object_manager)
{
    std::string type;
    int x, y;

    // open file for reading
    std::ifstream level_file(level_name.c_str());
    if (!level_file) {
        std::cerr << "Error opening " << level_name << "for reading!" << std::endl;
        return false;
    } else {
        while (!level_file.eof()) {
            level_file >> type;
            if (level_file.eof())
                break;

            level_file >> x;
            level_file >> y;

            ObjIndex object;

            if (type == "ENEMYSTD")      
                object = ENEMYSTD;
            else if (type == "ENEMYSIDEWAYS") 
                object = ENEMYSIDEWAYS;
            else if (type == "ENEMYRAMMER")   
                object = ENEMYRAMMER;
            else if (type == "ENEMYBONUS")    
                object = ENEMYBONUS;
            else if (type == "OBJECTBIGSHIP") 
                object = OBJECTBIGSHIP;

            // formations
            else if (type == "ENEMYSTD_V_FORMATION")
                object = ENEMYSTD_V_FORMATION;
            else if (type == "ENEMYSTD_3V_FORMATION")
                object = ENEMYSTD_3V_FORMATION;
            else if (type == "ENEMYSTD_DIAGONAL_FORMATION")
                object = ENEMYSTD_DIAGONAL_FORMATION;
            else if (type == "ENEMYSTD_MASSIVE_FORMATION")
                object = ENEMYSTD_MASSIVE_FORMATION;
            else if (type == "ENEMYSIDEWAYS_VLINE_FORMATION")
                object = ENEMYSIDEWAYS_VLINE_FORMATION;
            else if (type == "ENEMYSIDEWAYS_HLINE_FORMATION")
                object = ENEMYSIDEWAYS_HLINE_FORMATION;
            else if (type == "ENEMYSIDEWAYS_V_FORMATION")
                object = ENEMYSIDEWAYS_V_FORMATION;
            else if (type == "ENEMYSIDEWAYS_MASSIVE_FORMATION")
                object = ENEMYSIDEWAYS_MASSIVE_FORMATION;
            else if (type == "ENEMYRAMMER_VLINE_FORMATION")
                object = ENEMYRAMMER_VLINE_FORMATION;
            else if (type == "ENEMYRAMMER_DIAGONAL_FORMATION")
                object = ENEMYRAMMER_DIAGONAL_FORMATION;
            else if (type == "ENEMYRAMMER_FULLDIAGONAL_FORMATION")
                object = ENEMYRAMMER_FULLDIAGONAL_FORMATION;

            object_manager.create_object(x, -y, 0.0f, 0.0f, object, OWNER_NONE);

            // TODO: this should not be needed.
            //if (level_file.eof())
                //break;
        }
    }

    level_file.close();
    return true;
}


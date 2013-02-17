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
#include "ObjectManager.h"
#include "World.h"
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

LevelManager::LevelManager()
{
}

LevelManager::~LevelManager()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void LevelManager::loadLevel(std::string dataPath, int level, ObjectManager& objectManager)
{
    //////////////////////////////////////
    // get level filenames first
    std::vector<std::string> levelFilename;
    int i = 0;
    ifstream levelsFile((dataPath + "levels/levels.cfg").c_str());

    string tmpFilename;
    levelFilename.clear();

    while (!levelsFile.eof()) {
        levelsFile >> tmpFilename;
        levelFilename.push_back(tmpFilename);
        i++;

        if (levelsFile.eof())
            break;
    }

    levelsFile.close();
    ////////////////////////////////////

    internalLoadLevel(dataPath + "levels/" + levelFilename[level - 1], objectManager);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

bool LevelManager::internalLoadLevel(std::string levelName, ObjectManager& objectManager)
{
    string type;
    int x, y;

    // open file for reading
    ifstream levelFile(levelName.c_str());
    if (!levelFile) {
        cerr << "Error opening " << levelName << "for reading!" << endl;
        return false;
    } else {
        while (!levelFile.eof()) {
            levelFile >> type;
            if (levelFile.eof())
                break;

            levelFile >> x;
            levelFile >> y;

            ObjIndex object;

            if (type == "ENEMYSTD")      object = ENEMYSTD;
            else if (type == "ENEMYSIDEWAYS") object = ENEMYSIDEWAYS;
            else if (type == "ENEMYRAMMER")   object = ENEMYRAMMER;
            else if (type == "ENEMYBONUS")    object = ENEMYBONUS;
            else if (type == "OBJECTBIGSHIP") object = OBJECTBIGSHIP;

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

            objectManager.createObject(x, -y, 0.0f, 0.0f, \
                                       object, OWNER_NONE);

            if (levelFile.eof())
                break;
        }
    }

    levelFile.close();
    return true;
}
/*
int LevelManager::checkHowManyLevelsAvailable(string dataPath)
{
    int i = 0;
    ifstream levelsFile( (dataPath + "levels/levels.cfg").c_str() );

    string tmpFilename;
    levelFilename.clear();

    while(!levelsFile.eof())
    {
        levelsFile >> tmpFilename;
        levelFilename.push_back(tmpFilename);
//      cout << levelFilename[i] << endl;
        i++;

        if(levelsFile.eof())
            break;
    }

    levelsFile.close();
    levelIndexRead = true;
    return levelFilename.size()-1; // number of levels
}*/

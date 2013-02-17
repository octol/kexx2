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

#ifndef _KEXX2_LEVELMANAGER_H_
#define _KEXX2_LEVELMANAGER_H_

#include <string>
#include <vector>

class ObjectManager;
class State;

class LevelManager {
public:
    LevelManager();
    virtual ~LevelManager();

    void loadLevel(std::string dataPath, int level, ObjectManager& objectManager);

private:
    bool internalLoadLevel(std::string levelName, ObjectManager& objectManager);
    //std::vector<std::string> levelFilename;
    //bool levelIndexRead;
};

#endif

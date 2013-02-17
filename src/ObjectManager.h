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

#ifndef _KEXX2_OBJECTMANAGER_H_
#define _KEXX2_OBJECTMANAGER_H_

#include <list>
#include "SDLc/Surface.h"
#include "SDLc/Sound.h"
#include "Defines.h"

class Object;
class Screen;
class Timer;
class FxManager;
class PlayerState;

typedef std::list<Object*> ObjectList;

// main class
class ObjectManager {
public:
    ObjectManager();
    virtual ~ObjectManager();

    void loadData(std::string dataPath);

    void update(Timer& timer, FxManager& fxManager, float worldYPos_, PlayerState& playerState);
    void draw(Screen& screen);

    Object* createObject(int x, int y, float xVel, float yVel, ObjIndex object, Owner owner);
    Object* createObject(int x, int y, ObjIndex object, float vel, float angle, Owner owner);
    void createShips(PlayerState& playerState);
    int getHowManyEnemies();
    int getHowManyPlayersAlive();

    ObjectList list;                // object list
    Surface obj[ENEMYSTD_V_FORMATION];          // object graphics
    Sound snd[SND_SHOTROCKET + 1];          // object sound

private:
    void createFormation(int x, int y, float xVel, float yVel, enum ObjIndex object);

    // list maitenence functions
    void updatePlayerState(PlayerState& playerState);
    void flushList();
    void addFromQueue();
    void updateEnemyCount();

    ObjectList queue;
    int amountOfEnemiesInList;
    int playersAlive;
    float worldYPos;
};

#endif

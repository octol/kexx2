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

#ifndef _KEXX2_SHIP_H_
#define _KEXX2_SHIP_H_

#include "Object.h"
#include <list>
#include "PlayerState.h"
class Weapon;

const int BLINKING = 1;
const int INPUT_LOCKED = 2;

class Ship : public Object {
    //friend class Interface;
    friend class ObjectManager;
public:
    Ship(std::string n, int energy, int score, Surface& s, \
         Weapon* main, Weapon* extra, KeySet keySet_);
    virtual ~Ship();

    void think(ObjectManager& objectManager, FxManager& fxManager);
    void checkCollisions(ObjectManager& objectManager, FxManager& fxManager);
    void update(Timer& timer);
    void hurt(int value, ObjectManager& objectManager, FxManager& fxManager);
    void kill(ObjectManager& objectManager, FxManager& fxManager);

    void setMainWeapon(Weapon* w);
    void setExtraWeapon(Weapon* w);

private:
    void updateSmoketrail(ObjectManager& objectManager);
    void removeSmoketrail(ObjectManager& objectManager);

    void calculateHitImg();

    Weapon* mainWeapon;
    Weapon* extraWeapon;
    KeySet keySet;

    int invincible;
    bool levelcomplete;
    int blinkingTimer;
    int timesBlinked;

    Sound hitSnd;
};

#endif

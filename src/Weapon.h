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

#ifndef _KEXX2_WEAPON_H_
#define _KEXX2_WEAPON_H_

#include <list>
#include "SDLc/Surface.h"
#include "SDLc/Sound.h"
#include "Object.h"

class ObjectManager;

class Weapon {
    friend class ObjectManager;

public:
    Weapon();
    virtual ~Weapon();

    virtual void shoot(int x, int y, ObjectManager& objectManager) = 0;

    std::string name;
    int upgrade();
    int reset()         {
        return (level = 1);
    };
    int getCount()          {
        return count;
    };
    int getLevel()          {
        return level;
    };

protected:
    int setCount(int value);
    int setLevel(int value);
    unsigned getTimeWhenLastShot()  {
        return timeWhenLastShot;
    };
    unsigned setTimeWhenLastShot()  {
        return (timeWhenLastShot = SDL_GetTicks());
    };
    Owner getOwner()        {
        return owner;
    };
    Owner setOwner(Owner value) {
        return (owner = value);
    };

    sdlc::Sound shotSnd;

private:
    int count;
    int level;
    unsigned timeWhenLastShot;
    Owner owner;
};

#endif

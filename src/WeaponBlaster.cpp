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

#include "WeaponBlaster.h"
#include "Shot.h"
#include "ObjectManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

WeaponBlaster::WeaponBlaster(Sound& sound, Owner owner)
{
    shotSnd.link(&sound);
    shotSnd.setChannel((int)SND_W_BLASTER);
    setOwner(owner);

    setLevel(1);
    name = "Blaster Weapon";
}

WeaponBlaster::~WeaponBlaster()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void WeaponBlaster::shoot(int x, int y, ObjectManager& objectManager)
{
    if (SDL_GetTicks() > getTimeWhenLastShot() + 150) {
        shotSnd.play(0);
        if (getLevel() == 1) {
            objectManager.createObject(x, y, 0, -500.0f/*-0.5f*/, \
                                       SHOTBLASTER, getOwner());
        }
        if (getLevel() == 2) {
            objectManager.createObject(x - 7, y + 7, 0, -500.0f, \
                                       SHOTBLASTER, getOwner());
            objectManager.createObject(x + 7, y + 7, 0, -500.0f, \
                                       SHOTBLASTER, getOwner());
        }
        if (getLevel() == 3) {
            objectManager.createObject(x - 10, y + 7, -10.0f/*-0.01f*/, -500.0f, \
                                       SHOTBLASTER, getOwner());
            objectManager.createObject(x, y, 0, -500.0f, \
                                       SHOTBLASTERBIG, getOwner());
            objectManager.createObject(x + 10, y + 7,  10.0f/*0.01f*/, -500.0f, \
                                       SHOTBLASTER, getOwner());
        }
        if (getLevel() == 4) {
            objectManager.createObject(x - 14, y + 7, -50.0f, -500.0f, \
                                       SHOTBLASTER, getOwner());
            objectManager.createObject(x + 14, y + 7,  50.0f, -500.0f, \
                                       SHOTBLASTER, getOwner());
            objectManager.createObject(x - 10, y, 0, -500.0f, \
                                       SHOTBLASTERBIG, getOwner());
            objectManager.createObject(x + 10, y, 0, -500.0f, \
                                       SHOTBLASTERBIG, getOwner());
        }
        setTimeWhenLastShot();
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

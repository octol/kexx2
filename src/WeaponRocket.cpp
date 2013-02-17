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

#include "WeaponRocket.h"
#include "Defines.h"
#include "ShotRocket.h"
#include "ObjectManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

WeaponRocket::WeaponRocket(Sound& sound, Owner owner)
{
    shotSnd.link(&sound);
    shotSnd.setChannel((int)SND_W_ROCKET);
    setOwner(owner);

    setLevel(1);
    name = "Rocket Weapon";
}

WeaponRocket::~WeaponRocket()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void WeaponRocket::shoot(int x, int y, ObjectManager& objectManager)
{
    if (SDL_GetTicks() > getTimeWhenLastShot() + 750) {
        shotSnd.play(0);
        objectManager.createObject(x, y, 0, 0, SHOTROCKET, getOwner());
        setCount(getCount() - 1);
        setTimeWhenLastShot();
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

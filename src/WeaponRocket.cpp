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

WeaponRocket::WeaponRocket(sdlc::Sound& sound, Owner owner) : Weapon(sound)
{
    // TODO: redo how these are set.
    //shot_snd_.link(&sound);
    shot_snd_.set_channel((int)SND_W_ROCKET);
    set_owner(owner);

    set_level(1);
    name = "Rocket Weapon";
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void WeaponRocket::shoot(int x, int y, ObjectManager& object_manager)
{
    // TODO: use Timer class instead of raw SDL call.
    if (SDL_GetTicks() > time_when_last_shot() + 750) {
        shot_snd_.play(0);
        object_manager.create_object(x, y, 0, 0, SHOTROCKET, owner());
        set_count(count() - 1);
        set_time_when_last_shot();
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------


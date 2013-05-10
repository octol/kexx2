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

WeaponBlaster::WeaponBlaster(sdlc::Sound& sound, Owner owner) : Weapon(sound)
{
    //shot_snd_.link(&sound);
    shot_snd_.set_channel((int)SoundChannel::w_blaster);
    set_owner(owner);

    set_level(1);
    name = "Blaster Weapon";
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void WeaponBlaster::shoot(int x, int y, ObjectManager& object_manager)
{
    // TODO: use Timer class instead of raw SDL call.
    if (SDL_GetTicks() > time_when_last_shot() + 150) {
        shot_snd_.play(0);
        switch (level()) {
        case 1:
            object_manager.create_object(x, y, 0, -500.0f, 
                    ObjIndex::shotblaster, owner());
            break;
        case 2:
            object_manager.create_object(x - 7, y + 7, 0, -500.0f, 
                    ObjIndex::shotblaster, owner());
            object_manager.create_object(x + 7, y + 7, 0, -500.0f, 
                    ObjIndex::shotblaster, owner());
            break;
        case 3:
            object_manager.create_object(x - 10, y + 7, -10.0f, -500.0f, 
                    ObjIndex::shotblaster, owner());
            object_manager.create_object(x, y, 0, -500.0f, 
                    ObjIndex::shotblasterbig, owner());
            object_manager.create_object(x + 10, y + 7,  10.0f, -500.0f, 
                    ObjIndex::shotblaster, owner());
            break;
        case 4:
            object_manager.create_object(x - 14, y + 7, -50.0f, -500.0f, 
                    ObjIndex::shotblaster, owner());
            object_manager.create_object(x + 14, y + 7,  50.0f, -500.0f, 
                    ObjIndex::shotblaster, owner());
            object_manager.create_object(x - 10, y, 0, -500.0f, 
                    ObjIndex::shotblasterbig, owner());
            object_manager.create_object(x + 10, y, 0, -500.0f, 
                    ObjIndex::shotblasterbig, owner());
            break;
        default:
            break;
        }
        set_time_when_last_shot();
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

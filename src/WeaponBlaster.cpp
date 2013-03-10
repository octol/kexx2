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

WeaponBlaster::WeaponBlaster(sdlc::Sound& sound, Owner owner)
{
    shot_snd_.link(&sound);
    shot_snd_.setChannel((int)SND_W_BLASTER);
    set_owner(owner);

    set_level(1);
    name = "Blaster Weapon";
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void WeaponBlaster::shoot(int x, int y, ObjectManager& object_manager)
{
    if (SDL_GetTicks() > time_when_last_shot() + 150) {
        shot_snd_.play(0);
        if (level() == 1) {
            object_manager.create_object(x, y, 0, -500.0f, 
                    SHOTBLASTER, owner());
        }
        if (level() == 2) {
            object_manager.create_object(x - 7, y + 7, 0, -500.0f, 
                    SHOTBLASTER, owner());
            object_manager.create_object(x + 7, y + 7, 0, -500.0f, 
                    SHOTBLASTER, owner());
        }
        if (level() == 3) {
            object_manager.create_object(x - 10, y + 7, -10.0f, -500.0f, 
                    SHOTBLASTER, owner());
            object_manager.create_object(x, y, 0, -500.0f, 
                    SHOTBLASTERBIG, owner());
            object_manager.create_object(x + 10, y + 7,  10.0f, -500.0f, 
                    SHOTBLASTER, owner());
        }
        if (level() == 4) {
            object_manager.create_object(x - 14, y + 7, -50.0f, -500.0f, 
                    SHOTBLASTER, owner());
            object_manager.create_object(x + 14, y + 7,  50.0f, -500.0f, 
                    SHOTBLASTER, owner());
            object_manager.create_object(x - 10, y, 0, -500.0f, 
                    SHOTBLASTERBIG, owner());
            object_manager.create_object(x + 10, y, 0, -500.0f, 
                    SHOTBLASTERBIG, owner());
        }
        set_time_when_last_shot();
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

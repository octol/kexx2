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

#ifndef KEXX2_WEAPONBLASTER_H
#define KEXX2_WEAPONBLASTER_H

#include "SDLc.h"
#include "Weapon.h"
#include "Defines.h"

class WeaponBlaster final : public Weapon {
public:
    WeaponBlaster(sdlc::Sound& sound, Owner owner);

    void shoot(int x, int y, ObjectManager& object_manager) override;
};

#endif

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

#include "EnemyBonus.h"
#include "FxManager.h"
#include "ObjectManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemyBonus::EnemyBonus(std::string name, int energy, 
                       sdlc::Surface& s, sdlc::Surface& hit_s)
    : Object(name, energy, s, hit_s, ObjType::enemy)
{
    set_score(0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemyBonus::activate(ObjectManager& object_manager)
{
    Object::activate(object_manager);
    set_y_vel(120.0f);
}

void EnemyBonus::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    set_energy(0);
    int cx = (int)(x() + (float)width() / 2.0f);
    int cy = (int)(y() + (float)height() / 2.0f);

    fx_manager.explode_normal(cx, cy);

    int type = (rand() % 1) + (int)ObjIndex::bonusblaster;
    object_manager.create_object(cx, cy, 0, 0, (ObjIndex)type, Owner::none);
}


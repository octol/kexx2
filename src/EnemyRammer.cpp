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

#include "EnemyRammer.h"
#include "ObjectManager.h"
#include "FxManager.h"
#include <vector>

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemyRammer::EnemyRammer(std::string name, int energy, int score, sdlc::Surface& s)
    : Object(name, energy, s, OBJ_ENEMY)
{
    set_score(score);
    calculate_hit_img();
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemyRammer::activate(ObjectManager& object_manager)
{
    Object::activate(object_manager);
    //std::vector<Object*> player_vector;
    std::vector<std::shared_ptr<IObject>> player_vector;

    // Fetch the player ship objects
    for (auto obj : object_manager.list) {
        if (obj->type() == OBJ_PLAYER && obj->energy() > 0)
            player_vector.push_back(obj);
    }

    if (!player_vector.empty()) {
        int pos = rand() % player_vector.size();
        int px = (int)player_vector[pos]->x();
        int py = (int)player_vector[pos]->y();

        float dx = px - x();
        float dy = py - y();

        set_x_vel(((dx / 2) * 1.3f));
        set_y_vel(50.0f + (dy / 2));
    }

}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

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

EnemyRammer::EnemyRammer(std::string name, int energy, int score, Surface& s)
    : Object(name, energy, s, OBJ_ENEMY)
{
    set_score(score);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemyRammer::activate(ObjectManager& object_manager)
{
    std::vector<Object*> player_vector;

    // TODO: range based for loop
    ObjectList::iterator i = object_manager.list.begin();
    for (; i != object_manager.list.end(); i++) {
        Object* current = *i;
        if (current->type() == OBJ_PLAYER && current->energy() > 0)
            player_vector.push_back(current);
    }

    if (!player_vector.empty()) {
        int pos = rand() % player_vector.size();
        int px = (int)player_vector[pos]->getX();
        int py = (int)player_vector[pos]->getY();

        float dX = px - getX();
        float dY = py - getY();

        setXVel(((dX / 2) * 1.3f /** 0.001f*/));
        setYVel(50.0f + (dY / 2) /** 0.001f*/);
    }

}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

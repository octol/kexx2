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

#include "Shot.h"
#include <iostream>
#include "ObjectManager.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Shot::Shot() : Object("Generic Shot", 1, OBJ_SHOT)
{
}

Shot::Shot(std::string n, int energy, Surface& s, Owner owner)
   : Object(n, energy, s, OBJ_SHOT)
{
   set_owner(owner);
}

Shot::~Shot()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Shot::check_collisions(ObjectManager& object_manager, FxManager& fx_manager)
{
    if (owner() >= OWNER_PLAYER1 && owner() <= OWNER_PLAYER1 + NUM_OF_POSSIBLE_PLAYERS - 1) {
        // check collision  friendly fire <-> enemies
        ObjectList::iterator i;

        // start by getting pointer to the player
        // TODO: better name for this.
        Object* owner_tmp = 0;
        for (i = object_manager.list.begin(); i != object_manager.list.end(); i++) {
            Object* current = *i;
            if (owner() == OWNER_PLAYER1 && current->name == "Player 1")
                owner_tmp = current;
            else if (owner() == OWNER_PLAYER2 && current->name == "Player 2")
                owner_tmp = current;
        }

        for (i = object_manager.list.begin(); i != object_manager.list.end(); i++) {
            Object* current = *i;

            SDL_Rect tmp1 = getReducedRect();
            SDL_Rect tmp2 = current->getRect();
            if (current->type() == OBJ_ENEMY && current->energy() && \
                    overlap(tmp1, tmp2)) {
                current->hurt(energy(), object_manager, fx_manager);
                if (!current->energy() && owner_tmp)
                    owner_tmp->adjust_score(current->score());

                // kill itself
                kill(object_manager, fx_manager);
            } else if (current->type() == OBJ_SHOT) // optimization
                break;
        }
    } else {
        // check collision  player <-> enemy fire
        ObjectList::iterator i = object_manager.list.begin();
        for (; i != object_manager.list.end(); i++) {
            Object* current = *i;

            SDL_Rect tmp1 = getReducedRect();
            SDL_Rect tmp2 = current->getRect();
            if (current->type() == OBJ_PLAYER && current->energy() && \
                    overlap(tmp1, tmp2)) {
                current->hurt(energy(), object_manager, fx_manager);

                // kill itself
                kill(object_manager, fx_manager);
            } else if (current->type() == OBJ_SHOT) // optimization
                break;
        }
    }
}

void Shot::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    set_energy(0);
    fx_manager.playhitSnd();

    float angle = 270;
    //if(getYVel() > 0)
    //angle = 90.0f;
    //fx_manager.explodeTiny(getX()+getWidth()/2, getY()+getHeight()/2, 0.1f, angle);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

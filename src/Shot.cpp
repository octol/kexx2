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
#include <cassert>
#include <iostream>
#include <vector>
#include <algorithm>
#include "ObjectManager.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Shot::Shot() : Object("Generic Shot", 1, OBJ_SHOT)
{
}

Shot::Shot(std::string n, int _energy, sdlc::Surface& s, Owner _owner)
   : Object(n, _energy, s, OBJ_SHOT)
{
    set_owner(_owner);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Shot::check_collisions(ObjectManager& object_manager, 
                            FxManager& fx_manager)
{
    if (owner() >= OWNER_PLAYER1 
            && owner() <= OWNER_PLAYER1 + NUM_OF_POSSIBLE_PLAYERS - 1) {

        // 1) Check collision:  friendly fire <-> enemies 
        
        // start by getting pointer to the player
        std::shared_ptr<Object> owner_player = *std::find_if(
            begin(object_manager.list), 
            end(object_manager.list), 
            [this](std::shared_ptr<Object> o) {
                return Object::parse_owner(o->name()) == this->owner();
            });

        for (auto& object : object_manager.list) {
            SDL_Rect shot_rect = reduced_rect();
            SDL_Rect object_rect = object->rect();

            if (object->type() == OBJ_ENEMY && object->energy() && 
                    sdlc::overlap(shot_rect, object_rect)) {
                object->hurt(energy(), object_manager, fx_manager);
                if (!object->energy() && owner_player)
                    owner_player->adjust_score(object->score());

                // let the shot kill itself
                kill(object_manager, fx_manager);
            } else if (object->type() == OBJ_SHOT) // optimization
                break;
        }
    } else {
        
        // 2) Check collision  player <-> enemy fire 
  
        for (auto& current : object_manager.list) {
            SDL_Rect shot_rect = reduced_rect();
            SDL_Rect object_rect = current->rect();

            if (current->type() == OBJ_PLAYER && current->energy() && 
                    sdlc::overlap(shot_rect, object_rect)) {
                current->hurt(energy(), object_manager, fx_manager);

                // left the shot kill itself
                kill(object_manager, fx_manager);
            } else if (current->type() == OBJ_SHOT) // optimization
                break;
        }
    }
}

void Shot::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    UNUSED(object_manager);
    set_energy(0);
    fx_manager.play_hit_snd();
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

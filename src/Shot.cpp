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
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Shot::Shot()
{
    name = "Generic Shot";
    setEnergy(setEnergyMax(1));

    // inherited data
    setType(OBJ_SHOT);
}

Shot::Shot(std::string name_, int energy, Surface& s, Owner owner)
{
    name = name_;
    setEnergy(setEnergyMax(energy));
    link(s.data);

    // inherited data
    setOwner(owner);
    setType(OBJ_SHOT);
}

Shot::~Shot()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Shot::checkCollisions(ObjectManager& objectManager, FxManager& fxManager)
{
    if (getOwner() >= OWNER_PLAYER1 && getOwner() <= OWNER_PLAYER1 + NUM_OF_POSSIBLE_PLAYERS - 1) {
        // check collision  friendly fire <-> enemies
        ObjectList::iterator i;

        // start by getting pointer to the player
        Object* owner = 0;
        for (i = objectManager.list.begin(); i != objectManager.list.end(); i++) {
            Object* current = *i;
            if (getOwner() == OWNER_PLAYER1 && current->name == "Player 1")
                owner = current;
            else if (getOwner() == OWNER_PLAYER2 && current->name == "Player 2")
                owner = current;
        }

        for (i = objectManager.list.begin(); i != objectManager.list.end(); i++) {
            Object* current = *i;

            SDL_Rect tmp1 = getReducedRect();
            SDL_Rect tmp2 = current->getRect();
            if (current->getType() == OBJ_ENEMY && current->getEnergy() && \
                    overlap(tmp1, tmp2)) {
                current->hurt(getEnergy(), objectManager, fxManager);
                if (!current->getEnergy() && owner)
                    owner->adjustScore(current->getScore());

                // kill itself
                kill(objectManager, fxManager);
            } else if (current->getType() == OBJ_SHOT) // optimization
                break;
        }
    } else {
        // check collision  player <-> enemy fire
        ObjectList::iterator i = objectManager.list.begin();
        for (; i != objectManager.list.end(); i++) {
            Object* current = *i;

            SDL_Rect tmp1 = getReducedRect();
            SDL_Rect tmp2 = current->getRect();
            if (current->getType() == OBJ_PLAYER && current->getEnergy() && \
                    overlap(tmp1, tmp2)) {
                current->hurt(getEnergy(), objectManager, fxManager);

                // kill itself
                kill(objectManager, fxManager);
            } else if (current->getType() == OBJ_SHOT) // optimization
                break;
        }
    }
}

void Shot::kill(ObjectManager& objectManager, FxManager& fxManager)
{
    setEnergy(0);
    fxManager.playhitSnd();

    float angle = 270;
    //if(getYVel() > 0)
    //angle = 90.0f;
    //fxManager.explodeTiny(getX()+getWidth()/2, getY()+getHeight()/2, 0.1f, angle);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

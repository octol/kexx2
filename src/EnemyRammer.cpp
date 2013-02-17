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
#include <vector>
#include "FxManager.h"
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemyRammer::EnemyRammer(std::string name_, int energy, int score, Surface& s)
{
    setType(OBJ_ENEMY);

    name = name_;
    setEnergy(setEnergyMax(energy));
    setScore(score);
    link(s.data);
    calculateHitImg();
}

EnemyRammer::~EnemyRammer()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemyRammer::activate(ObjectManager& objectManager)
{
    vector<Object*> playerVector;

    ObjectList::iterator i = objectManager.list.begin();
    for (; i != objectManager.list.end(); i++) {
        Object* current = *i;
        if (current->getType() == OBJ_PLAYER && current->getEnergy() > 0)
            playerVector.push_back(current);
    }

    if (!playerVector.empty()) {
        int pos = rand() % playerVector.size();
        int px = (int)playerVector[pos]->getX();
        int py = (int)playerVector[pos]->getY();

        float dX = px - getX();
        float dY = py - getY();

        setXVel(((dX / 2) * 1.3f /** 0.001f*/));
        setYVel(50.0f + (dY / 2) /** 0.001f*/);
    }

}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

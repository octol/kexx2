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

EnemyBonus::EnemyBonus(std::string name_, int energy, int score, Surface& s)
{
    setType(OBJ_ENEMY);

    name = name_;
    setEnergy(setEnergyMax(energy));
    setScore(score);
    link(s.data);
    calculateHitImg();
}

EnemyBonus::~EnemyBonus()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemyBonus::activate(ObjectManager& objectManager)
{
    setYVel(120.0f/*0.12f*/);
}

void EnemyBonus::kill(ObjectManager& objectManager, FxManager& fxManager)
{
    setEnergy(0);
    fxManager.explodeNormal((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2));

    int type = (rand() % 1) + BONUSBLASTER;
    objectManager.createObject((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2), 0, 0/*0.1f*/, (ObjIndex)type, OWNER_NONE);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

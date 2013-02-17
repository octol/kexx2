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

#include "EnemySideways.h"
#include "SDLc/Timer.h"
#include "ObjectManager.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemySideways::EnemySideways(std::string name_, int energy, int score, Surface& s)
{
    setType(OBJ_ENEMY);

    name = name_;
    setEnergy(setEnergyMax(energy));
    setScore(score);
    link(s.data);
    calculateHitImg();

    left = false;
    timeWhenLastShot = 0;
}

EnemySideways::~EnemySideways()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemySideways::activate(ObjectManager& objectManager)
{
    setXVel(-100.0f/*-0.1f*/);
    setYVel(80.0f/*0.08f*/);
    timeWhenLastShot = SDL_GetTicks() - (rand() % 2000);
}

void EnemySideways::think(ObjectManager& objectManager, FxManager& fxManager)
{
    extern Timer* timer;
    if (active()) {
        if (left) {
            //cout << "hej" << endl;
            setXVel(getXVel() - (100.0f /*0.0001f*/ * timer->getFrametime()));
            if (getXVel() < -100.0f/*-0.1f*/)
                left = false;
        } else {
            //cout << "nej" << endl;
            setXVel(getXVel() + (100.0f /*0.0001f*/ * timer->getFrametime()));
            if (getXVel() > 100.0f/*0.1f*/)
                left = true;
        }

        // shooting
        if (SDL_GetTicks() - timeWhenLastShot > 2000) {
            objectManager.createObject((int)(getX() + getWidth() / 2), (int)(getY() + getHeight()), 0, /*0.2f*/200.0f, SHOTENEMYSTD, OWNER_ENEMY);
            timeWhenLastShot = SDL_GetTicks();
        }
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

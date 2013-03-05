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

#include "SDLc.h"
#include "EnemySideways.h"
#include "ObjectManager.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemySideways::EnemySideways(std::string name, int energy, int score, sdlc::Surface& s)
    : Object(name, energy, s, OBJ_ENEMY),
      left_(false), time_when_last_shot_(0)
{
    set_score(score);
}

EnemySideways::~EnemySideways()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemySideways::activate(ObjectManager& object_manager)
{
    setXVel(-100.0f/*-0.1f*/);
    setYVel(80.0f/*0.08f*/);
    // TODO: user Timer object passed down.
    extern sdlc::Timer* timer;
    time_when_last_shot_ = timer->ticks() - (rand() % 2000);
}

void EnemySideways::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    // TODO: remove extern
    extern sdlc::Timer* timer;
    if (active()) {
        if (left_) {
            setXVel(getXVel() - (100.0f /*0.0001f*/ * timer->frame_time()));
            if (getXVel() < -100.0f/*-0.1f*/)
                left_ = false;
        } else {
            setXVel(getXVel() + (100.0f /*0.0001f*/ * timer->frame_time()));
            if (getXVel() > 100.0f/*0.1f*/)
                left_ = true;
        }

        // shooting
        if (timer->ticks() - time_when_last_shot_ > 2000) {
            object_manager.createObject((int)(getX() + getWidth() / 2), 
                    (int)(getY() + getHeight()), 
                    0, /*0.2f*/200.0f, SHOTENEMYSTD, OWNER_ENEMY);
            time_when_last_shot_ = timer->ticks();
        }
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

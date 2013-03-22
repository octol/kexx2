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
      left_(false), 
      time_when_last_shot_(0)
{
    set_score(score);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void EnemySideways::activate(ObjectManager& object_manager)
{
    UNUSED(object_manager);

    // TODO: Remove extern Timer class.
    extern sdlc::Timer* timer;

    set_x_vel(-100.0f);
    set_y_vel(80.0f);

    time_when_last_shot_ = timer->ticks() - (rand() % 2000);
}

void EnemySideways::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    UNUSED(fx_manager);

    // TODO: Remove extern Timer class.
    extern sdlc::Timer* timer;

    if (active()) {
        if (left_) {
            set_x_vel(x_vel() - (100.0f * timer->frame_time()));
            if (x_vel() < -100.0f)
                left_ = false;
        } else {
            set_x_vel(x_vel() + (100.0f * timer->frame_time()));
            if (x_vel() > 100.0f)
                left_ = true;
        }

        // shooting
        if (timer->ticks() - time_when_last_shot_ > 2000) {
            int cx = x() + width() / 2;
            int cy = y() + height() / 2;
            object_manager.create_object(cx, cy, 0, 200.0f, SHOTENEMYSTD, 
                                         OWNER_ENEMY);
            time_when_last_shot_ = timer->ticks();
        }
    }
}


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

#include "ShotRocket.h"
#include "SDLc/Timer.h"
#include "FxManager.h"
#include "ObjectManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

ShotRocket::ShotRocket(std::string _name, int _energy, sdlc::Surface& s, 
        Owner _owner)
    : Shot(_name, _energy, s, _owner),
      // TODO: remove raw SDL call.
      time_when_shot_(SDL_GetTicks())       
{
    Object::init_animation(50, 2, 0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ShotRocket::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    // TODO: remove extern Timer class.
    extern sdlc::Timer* timer;
    set_y_vel(y_vel() - 500.0f * timer->frame_time());

    fx_manager.smokepuff((int)(x() + (float)width() / 2.0f), 
                         (int)(y() + (float)height() / 2.0f));

    // TODO: remove raw SDL call.
    if (SDL_GetTicks() - time_when_shot_ > 1000)
        kill(object_manager, fx_manager);
}

void ShotRocket::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    set_energy(0);
    fx_manager.explode_normal((int)(x() + (float)width() / 2.0f), 
                              (int)(y() + (float)height() / 2.0f));

    for (int i = 0; i < EXPLOSION_FRAGMENTS; i++) {
        float vel = 500.0f + (float)(rand() % 10 - 5) * 5.0f;
        float angle = (float)(rand() % 360);
        float center_x = x() + (float)width() / 2.0f;
        float center_y = y() + (float)height() / 2.0f;
        object_manager.create_object((int)center_x, (int)center_y, 
                SHOTBOMBFRAGMENT, vel, angle, owner());
    }
}


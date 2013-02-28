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

ShotRocket::ShotRocket(std::string name, int energy, Surface& s, Owner owner)
    : Shot(name, energy, s, owner),
      time_when_shot_(SDL_GetTicks())
{
    initAnimation(50, 2, 0);
}

ShotRocket::~ShotRocket()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ShotRocket::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    extern Timer* timer;
    //setXVel( getXVel() * 0.007f * Timer->getFrametime());
    setYVel(getYVel() - 500.0f /*0.0005*/*timer->frame_time());

    fx_manager.smokepuff((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2));

    if (SDL_GetTicks() - time_when_shot_ > 1000)
        kill(object_manager, fx_manager);
}

void ShotRocket::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    set_energy(0);
    fx_manager.explodeNormal((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2));

    // TODO: remove magic number
    for (int i = 0; i < 64; i++) {
        float vel = 500.0f/*0.5f*/ + (rand() % 10 - 5) * 5.0f/*0.005*/;
        float angle = rand() % 360;
        float x = getX() + getWidth() / 2;
        float y = getY() + getHeight() / 2;
        object_manager.createObject((int)x, (int)y, SHOTBOMBFRAGMENT, vel, angle, owner());
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

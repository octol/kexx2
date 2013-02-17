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

#include "Object.h"
#include "FxManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Object::Object()
{
    name = "Generic Object";
    type = OBJ_UNDEFINED;
    m_active = false;
    energy = 0;
    energyMax = 0;

    activationYVelSet = false;
    activationYVel = 0;

    score = 0;
    owner = OWNER_NONE;
    hitTimer = 0;
}

Object::Object(std::string n, int energy, int score, Surface& s, ObjType t, float initYVel)
{
    name = n;
    setEnergy(setEnergyMax(energy));
    setScore(score);
    link(s.data);
    type = t;
    setActivationYVel(initYVel);

    m_active = false;
    owner = OWNER_NONE;

    calculateHitImg();
    hitTimer = 0;
}

Object::~Object()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Object::activate(ObjectManager& objectManager)
{
    if (activationYVelSet)
        setYVel(activationYVel);
}

void Object::think(ObjectManager& objectManager, FxManager& fxManager)
{
}

void Object::checkCollisions(ObjectManager& objectManager, FxManager& fxManager)
{
}

void Object::update(Timer& timer)
{
    Sprite::update(timer);
    if (hitTimer && SDL_GetTicks() - hitTimer > 20) {
        hitTimer = 0;
        // flip SDL_Surface's
        SDL_Surface* tmp = data;
        data = hitImg.data;
        hitImg.data = tmp;
    }
}

void Object::hurt(int value, ObjectManager& objectManager, FxManager& fxManager)
{
    if (hitTimer == 0) {
        hitTimer = SDL_GetTicks();

        // flip SDL_Surface's
        SDL_Surface* tmp = data;
        data = hitImg.data;
        hitImg.data = tmp;
    }

    adjustEnergy(-value);
    if (getEnergy() <= 0)
        kill(objectManager, fxManager);
}

void Object::kill(ObjectManager& objectManager, FxManager& fxManager)
{
    setEnergy(0);
    fxManager.explodeNormal((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2));
}

float Object::setActivationYVel(float value)
{
    activationYVelSet = true;
    return (activationYVel = value);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Object::calculateHitImg()
{
    hitImg.data = SDL_DisplayFormat(data);
    //hitImg.alloc(data->w, data->h);
    hitImg.lock();
    int ix, iy;
    for (iy = 0; iy < hitImg.data->h; iy++) {
        for (ix = 0; ix < hitImg.data->w; ix++) {

            bool transparent = false;
            Uint8 r = 0, g = 0, b = 0, a = 255;
            getPix(ix, iy, &r, &g, &b);
            if (r >= 248 && g == 0 && b >= 248)
                transparent = true;
            else if (a != 255)
                transparent = true;

            if (!transparent)
                hitImg.setPix(ix, iy, 255, 255, 255);
        }
    }
    hitImg.unlock();
}

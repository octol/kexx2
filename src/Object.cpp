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
#include "SDLc/Timer.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

// TODO: better chaining of the constructors.

Object::Object()
{
    name = "Generic Object";
}

Object::Object(std::string n, int energy, ObjType t) 
    : Object(), type_(t)
{
    name = n;
    set_energy(set_energy_max(energy));
}

Object::Object(std::string n, int energy, Surface& s, ObjType t)
    : Object(n, energy, t),
{
    link(s.data);
    calculate_hit_img();
}

Object::Object(std::string n, int energy, int score, Surface& s, 
               ObjType t, float init_y_vel)
    : Object(n, energy, s, t)
{
    set_score(score);

    // note that this also sets: activation_y_vel_set_ = true
    set_activation_y_vel(init_y_vel);
}

Object::~Object()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Object::activate(ObjectManager& object_manager)
{
    if (activation_y_vel_set_)
        setYVel(activation_y_vel_);
}

void Object::think(ObjectManager& object_manager, FxManager& fx_manager)
{
}

void Object::check_collisions(ObjectManager& object_manager, FxManager& fx_manager)
{
}

void Object::update(Timer& timer)
{
    Sprite::update(timer);
    //if (hit_timer && SDL_GetTicks() - hit_timer > 20) {
    if (hit_timer && timer.ticks() - hit_timer > 20) {
        hit_timer = 0;
        // flip SDL_Surface's
        // TODO: use basesurface/surface from sdlc instead.
        SDL_Surface* tmp = data;
        data = hit_img.data;
        hit_img.data = tmp;
    }
}

void Object::hurt(int value, ObjectManager& object_manager, FxManager& fx_manager)
{
    if (hit_timer == 0) {
        // TODO: replace with Timer object.
        hit_timer = SDL_GetTicks();

        // flip SDL_Surface's
        // TODO: use basesurface/surface from sdlc instead.
        SDL_Surface* tmp = data;
        data = hit_img.data;
        hit_img.data = tmp;
    }

    adjust_energy(-value);
    if (energy() <= 0)
        kill(object_manager, fx_manager);
}

void Object::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    set_energy(0);
    fx_manager.explodeNormal((int)(getX() + getWidth() / 2), (int)(getY() + getHeight() / 2));
}

float Object::set_activation_y_vel(float value)
{
    activation_y_vel_set_ = true;
    return activation_y_vel_ = value;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Object::calculate_hit_img()
{
    hit_img.data = SDL_DisplayFormat(data);
    //hitImg.alloc(data->w, data->h);
    hit_img.lock();
    int ix, iy;
    for (iy = 0; iy < hit_img.data->h; iy++) {
        for (ix = 0; ix < hit_img.data->w; ix++) {

            bool transparent = false;
            Uint8 r = 0, g = 0, b = 0, a = 255;
            getPix(ix, iy, &r, &g, &b);
            if (r >= 248 && g == 0 && b >= 248)
                transparent = true;
            else if (a != 255)
                transparent = true;

            if (!transparent)
                hit_img.setPix(ix, iy, 255, 255, 255);
        }
    }
    hit_img.unlock();
}

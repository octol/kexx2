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

#include <cassert>
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
    : type_(t)
{
    name = n;
    set_energy(set_energy_max(energy));
}

Object::Object(std::string n, int energy, Surface& s, ObjType t)
    : Object(n, energy, t)
{
    link(s.data);
    calculate_hit_img();
}

Object::Object(std::string n, int energy, int score, Surface& s, 
               ObjType t, float init_y_vel)
    : Object(n, energy, s, t)
{
    set_score(score);
    set_activation_y_vel(init_y_vel);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Object::activate(ObjectManager& object_manager)
{
    UNUSED(object_manager);

    if (activation_y_vel_set_)
        set_y_vel(activation_y_vel_);

    set_active(true);
}

void Object::think(ObjectManager& object_manager, FxManager& fx_manager)
{
    UNUSED(object_manager);
    UNUSED(fx_manager);
}

void Object::check_collisions(ObjectManager& object_manager, 
        FxManager& fx_manager)
{
    UNUSED(object_manager);
    UNUSED(fx_manager);
}

void Object::update(sdlc::Timer& timer)
{
    Sprite::update(timer);
    if (hit_timer && timer.ticks() - hit_timer > 20) {
        hit_timer = 0;
        // flip SDL_Surface's
        // TODO: Use BaseSurface/Surface from sdlc instead.
        SDL_Surface* tmp = data;
        data = hit_img.data;
        hit_img.data = tmp;
    }
}

void Object::hurt(int value, ObjectManager& object_manager, 
        FxManager& fx_manager)
{
    if (hit_timer == 0) {
        // TODO: replace with Timer object.
        hit_timer = SDL_GetTicks();

        // flip SDL_Surface's
        // TODO: Use BaseSurface/Surface from sdlc instead.
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
    UNUSED(object_manager);

    set_energy(0);
    fx_manager.explode_normal((int)(x() + width() / 2), 
                              (int)(y() + height() / 2));
}

float Object::set_activation_y_vel(float value)
{
    activation_y_vel_set_ = true;
    return activation_y_vel_ = value;
}

ObjIndex Object::parse_obj_index(std::string type)
{
    ObjIndex object = NONE; 

    if (type == "ENEMYSTD")      
        object = ENEMYSTD;
    else if (type == "ENEMYSIDEWAYS") 
        object = ENEMYSIDEWAYS;
    else if (type == "ENEMYRAMMER")   
        object = ENEMYRAMMER;
    else if (type == "ENEMYBONUS")    
        object = ENEMYBONUS;
    else if (type == "OBJECTBIGSHIP") 
        object = OBJECTBIGSHIP;

    // formations
    else if (type == "ENEMYSTD_V_FORMATION")
        object = ENEMYSTD_V_FORMATION;
    else if (type == "ENEMYSTD_3V_FORMATION")
        object = ENEMYSTD_3V_FORMATION;
    else if (type == "ENEMYSTD_DIAGONAL_FORMATION")
        object = ENEMYSTD_DIAGONAL_FORMATION;
    else if (type == "ENEMYSTD_MASSIVE_FORMATION")
        object = ENEMYSTD_MASSIVE_FORMATION;
    else if (type == "ENEMYSIDEWAYS_VLINE_FORMATION")
        object = ENEMYSIDEWAYS_VLINE_FORMATION;
    else if (type == "ENEMYSIDEWAYS_HLINE_FORMATION")
        object = ENEMYSIDEWAYS_HLINE_FORMATION;
    else if (type == "ENEMYSIDEWAYS_V_FORMATION")
        object = ENEMYSIDEWAYS_V_FORMATION;
    else if (type == "ENEMYSIDEWAYS_MASSIVE_FORMATION")
        object = ENEMYSIDEWAYS_MASSIVE_FORMATION;
    else if (type == "ENEMYRAMMER_VLINE_FORMATION")
        object = ENEMYRAMMER_VLINE_FORMATION;
    else if (type == "ENEMYRAMMER_DIAGONAL_FORMATION")
        object = ENEMYRAMMER_DIAGONAL_FORMATION;
    else if (type == "ENEMYRAMMER_FULLDIAGONAL_FORMATION")
        object = ENEMYRAMMER_FULLDIAGONAL_FORMATION;

    assert(object != NONE);

    return object;
}

Owner Object::parse_owner(std::string player)
{
    Owner owner = OWNER_NONE;
    if (player == "Player 1")
        owner = OWNER_PLAYER1;
    else if (player == "Player 2")
        owner = OWNER_PLAYER2;

    return owner;
}

// -----------------------------------------------------------------------------
// Static Functions
// -----------------------------------------------------------------------------

// TODO: remove this function
bool Object::compare_type(const std::shared_ptr<Object>& o1, 
                          const std::shared_ptr<Object>& o2)
{
    return o1->type() < o2->type();
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Object::calculate_hit_img()
{
    // TODO: replace raw SDL call.
    // TODO: need to implement copy constructor for Surface.
    hit_img.data = SDL_DisplayFormat(data);

    hit_img.lock();
    for (int iy = 0; iy < hit_img.data->h; iy++) {
        for (int ix = 0; ix < hit_img.data->w; ix++) {
            uint8_t r = 0, g = 0, b = 0, a = 255;
            get_pix(ix, iy, &r, &g, &b);
            if (false == ((r >= 248 && g == 0 && b >= 248) || (a != 255)))
                hit_img.set_pix(ix, iy, 255, 255, 255);
        }
    }
    hit_img.unlock();
}


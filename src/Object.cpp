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

Object::Object()
{
}

Object::Object(std::string n, int energy, ObjType t) 
    : name_(n), type_(t), energy_(energy), energy_max_(energy) 
{
}

Object::Object(std::string n, int energy, sdlc::Surface& s, ObjType t)
    : Object(n, energy, 0, s, s, t)
{
}

Object::Object(std::string n, int energy, 
               sdlc::Surface& s, sdlc::Surface& hit_s, ObjType t)
    : Object(n, energy, 0, s, hit_s, t)
{
}

Object::Object(std::string n, int energy, int score, 
               sdlc::Surface& s, ObjType t, float init_y_vel)
    : Object(n, energy, score, s, s, t, init_y_vel)
{
}

Object::Object(std::string n, int energy, int score, 
               sdlc::Surface& s, sdlc::Surface& hit_s, ObjType t, 
               float init_y_vel)
    : Object(n, energy, score, s, hit_s, t)
{
    set_activation_y_vel(init_y_vel);
}

Object::Object(std::string n, int energy, int score, 
               sdlc::Surface& s, ObjType t)
    : Object(n, energy, score, s, s, t)
{
}

Object::Object(std::string n, int energy, int score, 
               sdlc::Surface& s, sdlc::Surface& hit_s, ObjType t)
    : sprite_(s), 
      hit_img_(hit_s),
      name_(n), 
      type_(t),
      energy_(energy), 
      energy_max_(energy), 
      score_(score)
{
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
    sprite_.update(timer);

    // keep other sprites in sync
    hit_img_.set_pos(sprite_.x(), sprite_.y());
    hit_img_.set_vel(sprite_.x_vel(), sprite_.y_vel());

    if (hit_timer && timer.ticks() - hit_timer > 20) {
        hit_timer = 0;
    }
}

void Object::hurt(int value, ObjectManager& object_manager, 
        FxManager& fx_manager)
{
    if (hit_timer == 0) {
        // TODO: replace with Timer object.
        hit_timer = SDL_GetTicks();
    }

    adjust_energy(-value);
    if (energy() <= 0)
        kill(object_manager, fx_manager);
}

void Object::kill(ObjectManager& object_manager, FxManager& fx_manager)
{
    UNUSED(object_manager);

    set_energy(0);
    fx_manager.explode_normal((int)(x() + (float)width() / 2.0f), 
                              (int)(y() + (float)height() / 2.0f));
}

float Object::set_activation_y_vel(float value)
{
    activation_y_vel_set_ = true;
    return activation_y_vel_ = value;
}

float Object::x() 
{
    return sprite_.x(); 
}

float Object::y() 
{
    return sprite_.y(); 
}

float Object::set_x(float value) 
{
    return sprite_.set_x(value); 
}

float Object::set_y(float value) 
{
    return sprite_.set_y(value); 
}

void Object::set_pos(float x, float y) 
{
    sprite_.set_pos(x,y); 
}

float Object::x_vel() 
{
    return sprite_.x_vel(); 
}

float Object::y_vel() 
{
    return sprite_.y_vel(); 
}

float Object::set_x_vel(float value) 
{
    return sprite_.set_x_vel(value); 
}

float Object::set_y_vel(float value) 
{
    return sprite_.set_y_vel(value); 
}

void Object::set_vel(float x, float y) 
{
    sprite_.set_vel(x,y); 
}

int Object::width() 
{
    return sprite_.width(); 
}

int Object::height() 
{
    return sprite_.height(); 
}

bool Object::locked_to_screen() const 
{
    return sprite_.locked_to_screen(); 
}

bool Object::set_locked_to_screen(bool value) 
{
    return sprite_.set_locked_to_screen(value); 
}

void Object::init_animation(int speed, int frames, int iterations)
{
    sprite_.init_animation(speed, frames, iterations);
    hit_img_.init_animation(speed, frames, iterations);
}

void Object::set_current_anim_frame(int num) 
{
    sprite_.set_current_anim_frame(num);
}

SDL_Rect Object::rect() const 
{
    return sprite_.rect(); 
}

SDL_Rect Object::reduced_rect() const 
{
    return sprite_.reduced_rect(); 
}

std::string Object::name() const
{
    return name_;
}

std::string Object::set_name(const std::string& n)
{
    return name_ = n;
}

const sdlc::Sprite& Object::sprite() const 
{
    if (hit_timer)
        return hit_img_;
    return sprite_;
}

// -----------------------------------------------------------------------------
// Static Functions
// -----------------------------------------------------------------------------

bool Object::compare_type(const std::shared_ptr<IObject>& o1, 
                          const std::shared_ptr<IObject>& o2)
{
    return o1->type() < o2->type();
}

ObjIndex Object::parse_obj_index(std::string type)
{
    ObjIndex object = ObjIndex::none; 

    if (type == "ENEMYSTD")      
        object = ObjIndex::enemystd;
    else if (type == "ENEMYSIDEWAYS") 
        object = ObjIndex::enemysideways;
    else if (type == "ENEMYRAMMER")   
        object = ObjIndex::enemyrammer;
    else if (type == "ENEMYBONUS")    
        object = ObjIndex::enemybonus;
    else if (type == "OBJECTBIGSHIP") 
        object = ObjIndex::objectbigship;

    // formations
    else if (type == "ENEMYSTD_V_FORMATION")
        object = ObjIndex::enemystd_v_formation;
    else if (type == "ENEMYSTD_3V_FORMATION")
        object = ObjIndex::enemystd_3v_formation;
    else if (type == "ENEMYSTD_DIAGONAL_FORMATION")
        object = ObjIndex::enemystd_diagonal_formation;
    else if (type == "ENEMYSTD_MASSIVE_FORMATION")
        object = ObjIndex::enemystd_massive_formation;
    else if (type == "ENEMYSIDEWAYS_VLINE_FORMATION")
        object = ObjIndex::enemysideways_vline_formation;
    else if (type == "ENEMYSIDEWAYS_HLINE_FORMATION")
        object = ObjIndex::enemysideways_hline_formation;
    else if (type == "ENEMYSIDEWAYS_V_FORMATION")
        object = ObjIndex::enemysideways_v_formation;
    else if (type == "ENEMYSIDEWAYS_MASSIVE_FORMATION")
        object = ObjIndex::enemysideways_massive_formation;
    else if (type == "ENEMYRAMMER_VLINE_FORMATION")
        object = ObjIndex::enemyrammer_vline_formation;
    else if (type == "ENEMYRAMMER_DIAGONAL_FORMATION")
        object = ObjIndex::enemyrammer_diagonal_formation;
    else if (type == "ENEMYRAMMER_FULLDIAGONAL_FORMATION")
        object = ObjIndex::enemyrammer_fulldiagonal_formation;
    else throw std::invalid_argument("Object::parse_obj_index(" + type + ")");

    return object;
}

Owner Object::parse_owner(int player)
{
    return static_cast<Owner>((int)Owner::player1 + player - 1);
}

Owner Object::parse_owner(std::string player)
{
    Owner owner = Owner::none;
    if (player == "Player 1")
        owner = Owner::player1;
    else if (player == "Player 2")
        owner = Owner::player2;
    else throw std::invalid_argument("Object::parse_owner(" + player + ")");

    return owner;
}

sdlc::Surface Object::create_hit_img(const sdlc::Surface& surface)
{
    sdlc::Surface hit_surface(surface);
    hit_surface.make_unique();
    hit_surface.lock();
    for (int iy = 0; iy < surface.height(); ++iy) {
        for (int ix = 0; ix < surface.width(); ++ix) {
            uint8_t r = 0, g = 0, b = 0, a = 255;
            surface.get_pix(ix, iy, &r, &g, &b);
            if (false == ((r >= 248 && g == 0 && b >= 248) || (a != 255)))
                hit_surface.set_pix(ix, iy, 255, 255, 255);
        }
    }    
    hit_surface.unlock();
    return hit_surface;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------



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

#ifndef KEXX2_OBJECT_H
#define KEXX2_OBJECT_H

#include "SDLc/Sprite.h"
#include <list>
#include "Defines.h"

class ObjectManager;
class FxManager;

class Object : public sdlc::Sprite {
public:
    Object();
    Object(std::string n, int energy, ObjType t);
    Object(std::string n, int energy, sdlc::Surface& s, ObjType t);
    Object(std::string n, int energy, int score, sdlc::Surface& s, ObjType t, float init_y_vel);
    virtual ~Object();

    virtual void activate(ObjectManager& object_manager);
    virtual void think(ObjectManager& object_manager, FxManager& fx_manager);
    virtual void check_collisions(ObjectManager& object_manager, FxManager& fx_manager);
    virtual void update(sdlc::Timer& timer);
    virtual void hurt(int value, ObjectManager& object_manager, FxManager& fx_manager);
    virtual void kill(ObjectManager& object_manager, FxManager& fx_manager);

    ObjType type();
    ObjType set_type(ObjType value);
    int energy();
    int set_energy(int value);
    int adjust_energy(int e);
    int energy_max();
    int set_energy_max(int value);

    bool active();
    bool active(bool value);

    float set_activation_y_vel(float value);

    int score();
    int set_score(int value);
    int adjust_score(int value);

    // TODO:
    // protected:
    Owner owner();
    Owner set_owner(Owner value);

protected:
    virtual void calculate_hit_img();
    Surface hit_img;
    int hit_timer = 0;

private:
    ObjType type_ = OBJ_UNDEFINED;
    bool active_ = false;
    int energy_ = 0;
    int energy_max_ = 0;

    bool activation_y_vel_set_ = false;
    float activation_y_vel_ = 0;

    int score_ = 0;
    Owner owner_ = OWNER_NONE;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
ObjType Object::type()       
{
    return type_;
}

inline
ObjType Object::set_type(ObjType value)  
{
    return type_ = value;
}

inline
int Object::energy()         
{
    return energy_;
}

inline
int Object::set_energy(int value)
{
    if (value < 0) 
        value = 0;
    if (energy_max() != 0 && value > energy_max())
        value = energy_max();

    return energy_ = value;
}

inline
int Object::adjust_energy(int e)         
{
    return set_energy(energy() + e);
}

inline
int Object::energy_max()          
{
    return energy_max_;
}

inline
int Object::set_energy_max(int value)     
{
    return energy_max_ = value;
}

inline
bool Object::active()           
{
    return active_;
}

inline
bool Object::active(bool value)     
{
    return active_ = value;
}

inline
int Object::score()          
{
    return score_;
}

inline
int Object::set_score(int value)
{
    if (value < 0) 
        value = 0;
    return score_ = value;
}

inline
int Object::adjust_score(int value)  
{
    return set_score(score() + value);
}

// TODO: why protected?
//protected:
inline
Owner Object::owner()        
{
    return owner_;
}

inline
Owner Object::set_owner(Owner value) 
{
    return owner_ = value;
}
#endif // KEXX2_OBJECT_H

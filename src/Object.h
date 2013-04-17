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

#include <list>
#include <memory>
#include "SDLc/Sprite.h"
#include "Defines.h"
#include "IObject.h"

class ObjectManager;
class FxManager;

class Object : public IObject {
public:
    Object();
    Object(std::string n, int energy, ObjType);
    Object(std::string n, int energy, sdlc::Surface&, ObjType);
    Object(std::string n, int energy, int score, sdlc::Surface&, ObjType, 
           float init_y_vel);
    Object(std::string n, int energy, int score, sdlc::Surface&, ObjType);
    virtual ~Object() {};

    // Main functions
    virtual void activate(ObjectManager&) override;
    virtual void think(ObjectManager&, FxManager&) override;
    virtual void check_collisions(ObjectManager&, FxManager&) override;
    virtual void update(sdlc::Timer&) override;
    virtual void hurt(int value, ObjectManager&, FxManager&) override;
    virtual void kill(ObjectManager&, FxManager&) override;

    // State data
    virtual float x() override;
    virtual float y() override;
    virtual float set_x(float value) override;
    virtual float set_y(float value) override;
    virtual void set_pos(float x, float y) override;
    virtual float x_vel() override;
    virtual float y_vel() override;
    virtual float set_x_vel(float value) override;
    virtual float set_y_vel(float value) override;
    virtual void  set_vel(float x, float y) override;

    virtual int width() override;
    virtual int height() override;

    virtual bool locked_to_screen() const override;
    virtual bool set_locked_to_screen(bool value) override;

    virtual void init_animation(int speed, int frames, int iterations) override;
    virtual void set_current_anim_frame(int num) override;   // set current frame

    virtual SDL_Rect rect() const override;
    virtual SDL_Rect reduced_rect() const override;

    virtual ObjType type() const override;
    virtual ObjType set_type(ObjType value) override;
    virtual int energy() const override;
    virtual int set_energy(int value) override;
    virtual int adjust_energy(int e) override;
    virtual int energy_max() const override;
    virtual int set_energy_max(int value) override;

    // Some objects toggle between active and passive state.
    virtual bool active() const override;
    virtual bool set_active(bool value) override;
    virtual float set_activation_y_vel(float value) override;

    virtual int score() const override;
    virtual int set_score(int value) override;
    virtual int adjust_score(int value) override;

    virtual Owner owner() const override;
    virtual Owner set_owner(Owner value) override;

    virtual std::string name() const override;
    virtual std::string set_name(const std::string&) override;

    virtual const sdlc::Sprite& sprite() const override;

    // Static functions
    static bool compare_type(const std::shared_ptr<IObject>& o1, 
                             const std::shared_ptr<IObject>& o2);

    static ObjIndex parse_obj_index(std::string type);
    static Owner parse_owner(std::string type);

protected:
    virtual void calculate_hit_img();

    // The sprite object maintains position and velocity state.
    sdlc::Sprite sprite_;

    // the graphics shown when being hit.
    sdlc::Surface hit_img_;
    int hit_timer = 0;

private:
    std::string name_ = "Generic object";
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
ObjType Object::type() const
{
    return type_;
}

inline
ObjType Object::set_type(ObjType value)  
{
    return type_ = value;
}

inline
int Object::energy() const
{
    return energy_;
}

inline
int Object::set_energy(int value)
{
    value = std::max(value, 0);
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
int Object::energy_max() const
{
    return energy_max_;
}

inline
int Object::set_energy_max(int value)     
{
    return energy_max_ = value;
}

inline
bool Object::active() const
{
    return active_;
}

inline
bool Object::set_active(bool value)     
{
    return active_ = value;
}

inline
int Object::score() const
{
    return score_;
}

inline
int Object::set_score(int value)
{
    return score_ = std::max(value, 0);
}

inline
int Object::adjust_score(int value)  
{
    return set_score(score() + value);
}

//protected:
inline
Owner Object::owner() const
{
    return owner_;
}

inline
Owner Object::set_owner(Owner value) 
{
    return owner_ = value;
}

#endif // KEXX2_OBJECT_H

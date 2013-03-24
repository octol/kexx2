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
#include "SDLc/Sprite.h"
#include "Defines.h"

class ObjectManager;
class FxManager;

enum Owner { 
    OWNER_NONE,
    OWNER_PLAYER1,
    OWNER_PLAYER2,
    OWNER_ENEMY 
};

enum ObjType { 
    OBJ_UNDEFINED, 
    OBJ_PASSIVE,
    OBJ_ENEMY,
    OBJ_BONUS,
    OBJ_PLAYER,
    OBJ_PLAYERPASSIVE,
    OBJ_SHOT 
};

enum ObjIndex {
    NONE,
    PLAYER1,
    PLAYER2,
    ENEMYSTD,
    ENEMYSIDEWAYS,
    ENEMYRAMMER,
    ENEMYBONUS,
    OBJECTBIGSHIP,
    BONUSBLASTER,
    BONUSROCKET,

    // shots
    SHOTBLASTER,
    SHOTBLASTERBIG,
    SHOTROCKET,
    SHOTBOMBFRAGMENT,
    SHOTENEMYSTD,

    // misc
    SMOKETRAIL,

    // formations
    ENEMYSTD_V_FORMATION,
    ENEMYSTD_3V_FORMATION,
    ENEMYSTD_DIAGONAL_FORMATION,
    ENEMYSTD_MASSIVE_FORMATION,

    ENEMYSIDEWAYS_VLINE_FORMATION,
    ENEMYSIDEWAYS_HLINE_FORMATION,
    ENEMYSIDEWAYS_V_FORMATION,
    ENEMYSIDEWAYS_MASSIVE_FORMATION,

    ENEMYRAMMER_VLINE_FORMATION,
    ENEMYRAMMER_DIAGONAL_FORMATION,
    ENEMYRAMMER_FULLDIAGONAL_FORMATION
};

enum ObjSnd {
    SND_SHOTBLASTER,
    SND_SHOTROCKET
};

enum SoundChannel { 
    SND_DIV,
    SND_W_BLASTER,
    SND_W_ROCKET,
    SND_EXPL_SMALL,
    SND_EXPL_BIG,
    SND_EXPL_PLAYER,
    SND_ALARM 
};

// TODO: change this to composition?

class Object : public sdlc::Sprite {
public:
    Object();
    Object(std::string n, int energy, ObjType t);
    Object(std::string n, int energy, sdlc::Surface& s, ObjType t);
    Object(std::string n, int energy, int score, sdlc::Surface& s, 
           ObjType t, float init_y_vel);
    virtual ~Object() {};

    virtual void activate(ObjectManager& object_manager);
    virtual void think(ObjectManager& object_manager, FxManager& fx_manager);
    virtual void check_collisions(ObjectManager& object_manager, FxManager& fx_manager);
    virtual void update(sdlc::Timer& timer);
    virtual void hurt(int value, ObjectManager& object_manager, FxManager& fx_manager);
    virtual void kill(ObjectManager& object_manager, FxManager& fx_manager);

    ObjType type() const;
    ObjType set_type(ObjType value);
    int energy() const;
    int set_energy(int value);
    int adjust_energy(int e);
    int energy_max() const;
    int set_energy_max(int value);

    bool active() const;
    // TODO: these should be private/protected?
    bool active(bool value); // DEPRECATED
    bool set_active(bool value);

    float set_activation_y_vel(float value);

    int score() const;
    int set_score(int value);
    int adjust_score(int value);

    static ObjIndex parse_obj_index(std::string type);
    static Owner parse_owner(std::string type);

    // TODO: Why did I mark this protected?
    // protected:
    Owner owner() const;
    Owner set_owner(Owner value);

    static bool compare_type(const Object* o1, const Object* o2);


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
bool Object::active(bool value)     
{
    return active_ = value;
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

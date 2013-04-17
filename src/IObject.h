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

#ifndef KEXX2_IOBJECT_H
#define KEXX2_IOBJECT_H

#include "SDLc/Timer.h"

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

class ObjectManager;
class FxManager;

// -----------------------------------------------------------------------------
// Defines an interface for all game object.
// -----------------------------------------------------------------------------

class IObject {
public:
    virtual ~IObject() {};

    virtual void activate(ObjectManager&) = 0;
    virtual void think(ObjectManager&, FxManager&) = 0;
    virtual void check_collisions(ObjectManager&, FxManager&) = 0;
    virtual void update(sdlc::Timer&) = 0;
    virtual void hurt(int value, ObjectManager&, FxManager&) = 0;
    virtual void kill(ObjectManager&, FxManager&) = 0;

    virtual float x() = 0;
    virtual float y() = 0;
    virtual float set_x(float value) = 0;
    virtual float set_y(float value) = 0;
    virtual void set_pos(float x, float y) = 0;
    virtual float x_vel() = 0;
    virtual float y_vel() = 0;
    virtual float set_x_vel(float value) = 0;
    virtual float set_y_vel(float value) = 0;
    virtual void  set_vel(float x, float y) = 0;

    virtual int width() = 0;
    virtual int height() = 0;

    virtual bool locked_to_screen() const = 0;
    virtual bool set_locked_to_screen(bool value) = 0;

    virtual void init_animation(int speed, int frames, int iterations) = 0;
    virtual void set_current_anim_frame(int num) = 0;   // set current frame

    virtual SDL_Rect rect() const = 0;
    virtual SDL_Rect reduced_rect() const = 0;

    virtual ObjType type() const = 0;
    virtual ObjType set_type(ObjType value) = 0;
    virtual int energy() const = 0;
    virtual int set_energy(int value) = 0;
    virtual int adjust_energy(int e) = 0;
    virtual int energy_max() const = 0;
    virtual int set_energy_max(int value) = 0;

    // Some objects toggle between active and passive state.
    virtual bool active() const = 0;
    virtual bool set_active(bool value) = 0;
    virtual float set_activation_y_vel(float value) = 0;

    virtual int score() const = 0;
    virtual int set_score(int value) = 0;
    virtual int adjust_score(int value) = 0;

    virtual Owner owner() const = 0;
    virtual Owner set_owner(Owner value) = 0;

    virtual std::string name() const = 0;
    virtual std::string set_name(const std::string&) = 0;

    virtual const sdlc::Sprite& sprite() const = 0;
};

#endif // KEXX2_IOBJECT_H

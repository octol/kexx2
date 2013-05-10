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

#ifndef _KEXX2_WEAPON_H_
#define _KEXX2_WEAPON_H_

#include <list>
#include "SDLc/Surface.h"
#include "SDLc/Sound.h"
#include "Object.h"

class ObjectManager;

class Weapon {
    friend class ObjectManager;
public:
    Weapon(sdlc::Sound& sound);
    virtual ~Weapon() = default;

    virtual void shoot(int x, int y, ObjectManager& objectManager) = 0;

    std::string name = "Generic Weapon";
    int upgrade();
    int reset();
    int count();
    int level();

protected:
    int set_count(int value);
    int set_level(int value);
    unsigned time_when_last_shot();
    unsigned set_time_when_last_shot();
    Owner owner();
    Owner set_owner(Owner value);

    sdlc::Sound shot_snd_;

private:
    int count_ = 0;
    int level_ = 0;
    unsigned time_when_last_shot_ = 0;
    Owner owner_ = Owner::none;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
int Weapon::reset()         
{
    return level_ = 1;
}

inline
int Weapon::count()          
{
    return count_;
}

inline
int Weapon::level()          
{
    return level_;
}

inline
unsigned Weapon::time_when_last_shot()  
{
    return time_when_last_shot_;
}

inline
unsigned Weapon::set_time_when_last_shot()  
{
    // TODO: replace with call to timer object.
    return time_when_last_shot_ = SDL_GetTicks();
}

inline
Owner Weapon::owner()        
{
    return owner_;
}

inline
Owner Weapon::set_owner(Owner value) 
{
    return owner_ = value;
}

#endif

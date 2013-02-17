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

#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "SDLc/Sprite.h"
#include <list>
#include "Defines.h"

class ObjectManager;
class FxManager;

class Object : public Sprite {
public:
    Object();
    Object(std::string n, int energy, int score, Surface& s, ObjType t, float initYVel);
    virtual ~Object();

    virtual void activate(ObjectManager& objectManager);
    virtual void think(ObjectManager& objectManager, FxManager& fxManager);
    virtual void checkCollisions(ObjectManager& objectManager, \
                                 FxManager& fxManager);
    virtual void update(Timer& timer);
    virtual void hurt(int value, ObjectManager& objectManager, \
                      FxManager& fxManager);
    virtual void kill(ObjectManager& objectManager, FxManager& fxManager);

    ObjType getType()       {
        return type;
    };
    ObjType setType(ObjType value)  {
        return (type = value);
    };
    int getEnergy()         {
        return energy;
    };
    inline int setEnergy(int value);
    int adjustEnergy(int e)         {
        return setEnergy(getEnergy() + e);
    };
    int getEnergyMax()          {
        return energyMax;
    };
    int setEnergyMax(int value)     {
        return (energyMax = value);
    };

    bool active()           {
        return m_active;
    };
    bool active(bool value)     {
        return (m_active = value);
    };

    float setActivationYVel(float value);

    int getScore()          {
        return score;
    };
    inline int setScore(int value);
    int adjustScore(int value)  {
        return setScore(getScore() + value);
    };

    //protected:
    Owner getOwner()        {
        return owner;
    };
    Owner setOwner(Owner value) {
        return (owner = value);
    };

protected:
    virtual void calculateHitImg();
    Surface hitImg;
    int hitTimer;

private:
    ObjType type;
    bool m_active;
    int energy;
    int energyMax;

    bool activationYVelSet;
    float activationYVel;

    int score;
    Owner owner;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

int Object::setEnergy(int value)
{
    if (value < 0) value = 0;
    if (energyMax != 0 && value > energyMax) value = energyMax;

    return (energy = value);
}

int Object::setScore(int value)
{
    if (value < 0) value = 0;
    return (score = value);
}

#endif

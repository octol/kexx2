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

#include "Ship.h"
#include "ObjectManager.h"
#include "SDLc/Input.h"
#include "SDLc/Timer.h"
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "FxManager.h"
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Ship::Ship(std::string n, int energy, int score, Surface& s, \
           Weapon* main, Weapon* extra, KeySet keySet_)
{
    setType(OBJ_PLAYER);

    name = n;

    int tmp = n[n.length() - 1] - (int)'0';
    setOwner((Owner)(OWNER_PLAYER1 + tmp - 1));

    link(s.data);
    calculateHitImg();

    setEnergy(setEnergyMax(energy));
    setScore(score);
    lockedToScreen(false);
    active(true);

    mainWeapon = main;
    extraWeapon = extra;
    keySet = keySet_;

    invincible = INPUT_LOCKED;
    levelcomplete = false;
    blinkingTimer = 0;
    timesBlinked = 0;
}

Ship::~Ship()
{
    if (mainWeapon)
        delete mainWeapon;
    if (extraWeapon)
        delete extraWeapon;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Ship::think(ObjectManager& objectManager, FxManager& fxManager)
{
    extern Input* input;

    // reset movement velocity (if not input locked)
    if (invincible != INPUT_LOCKED) {
        setXVel(0);
        setYVel(0);

        // check keyboard
        if (input->keyPressed(keySet.left, AUTOFIRE))
            setXVel(-100.0f/*-0.1f*/);
        else if (input->keyPressed(keySet.right, AUTOFIRE))
            setXVel(100.0f);
        if (input->keyPressed(keySet.up, AUTOFIRE))
            setYVel(-100.0f);
        else if (input->keyPressed(keySet.down, AUTOFIRE))
            setYVel(100.0f);

        if (input->keyPressed(keySet.fireMain, AUTOFIRE) && mainWeapon)
            mainWeapon->shoot((int)(getX() + (getWidth() / 2)), (int)(getY() + 10), objectManager);
        if (input->keyPressed(keySet.fireExtra, NO_AUTOFIRE) && extraWeapon)
            extraWeapon->shoot((int)(getX() + (getWidth() / 2)), (int)(getY() + 10), objectManager);
    } else {
        // do some scripted movement when entering/leaving a level
        if (!levelcomplete) {
            extern Timer* timer;
            setYVel(getYVel() + (130.0f * timer->frame_time()));

            if (getYVel() > -10.0f /*-0.0100f*/) {
                setYVel(0);
                invincible = 0;
                lockedToScreen(true);
            }
        } else if (levelcomplete) {
            if (lockedToScreen()) {
                setXVel(0);
                setYVel(0);
                lockedToScreen(false);
            }

            extern Timer* timer;
            setYVel(getYVel() - (130.0f * timer->frame_time()));
        }
    }

    // blinking when hit
    if (invincible == BLINKING && SDL_GetTicks() - blinkingTimer > 50) {
        timesBlinked++;
        blinkingTimer = SDL_GetTicks();

        // flip SDL_Surface's
        SDL_Surface* tmp = data;
        data = hitImg.data;
        hitImg.data = tmp;
    }

    // check if empty clip
    if (extraWeapon && extraWeapon->getCount() < 1) {
        delete extraWeapon;
        extraWeapon = 0;
    }

    // smoke trail
    updateSmoketrail(objectManager);

    // if level complete
    if (objectManager.getHowManyEnemies() == 0) {
        invincible = INPUT_LOCKED;
        levelcomplete = true;
    }
}

void Ship::checkCollisions(ObjectManager& objectManager, FxManager& fxManager)
{
    // check collisions  player <-> objects
    ObjectList::iterator i = objectManager.list.begin();
    for (; i != objectManager.list.end(); i++) {
        Object* current = *i;

        SDL_Rect tmp1 = getReducedRect();
        SDL_Rect tmp2 = current->getReducedRect();
        if (current->getType() == OBJ_ENEMY && current->getEnergy() && \
                overlap(tmp1, tmp2) && !invincible) {
            // kill both objects
            kill(objectManager, fxManager);
            current->kill(objectManager, fxManager);
        } else if (current->getType() == OBJ_BONUS && current->getEnergy() && \
                   overlap(tmp1, tmp2)) {
            // blaster
            if (current->name == "Blaster Bonus" && \
                    mainWeapon->name == "Blaster Weapon")
                mainWeapon->upgrade();
            else if (current->name == "Blaster Bonus")
                setMainWeapon(new WeaponBlaster(objectManager.snd[SND_SHOTBLASTER], getOwner()));

            current->setEnergy(0); // want mute removal
        }
    }
}

void Ship::update(Timer& timer)
{
    Sprite::update(timer);
    if (hitTimer > 0 && timesBlinked > 100) {
        hitTimer = 0;
        timesBlinked = 0;
        blinkingTimer = 0;
        invincible = 0;
    }
}

void Ship::hurt(int value, ObjectManager& objectManager, FxManager& fxManager)
{
    if (!invincible && hitTimer == 0) {
        Object::hurt(value, objectManager, fxManager);
        if (getEnergy())
            fxManager.playPlayerHitSnd();

        invincible = BLINKING;
        blinkingTimer = SDL_GetTicks();
    }
}

void Ship::kill(ObjectManager& objectManager, FxManager& fxManager)
{
    if (!invincible) {
        Object::kill(objectManager, fxManager);
        removeSmoketrail(objectManager);
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Ship::setMainWeapon(Weapon* w)
{
    if (mainWeapon)
        delete mainWeapon;
    mainWeapon = w;
}

void Ship::setExtraWeapon(Weapon* w)
{
    if (extraWeapon)
        delete extraWeapon;
    extraWeapon = w;
}

void Ship::updateSmoketrail(ObjectManager& objectManager)
{
    bool smokefound = false;
    ObjectList::iterator i = objectManager.list.begin();
    for (; i != objectManager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->getOwner() == getOwner()) {
            smokefound = true;
            break;
        }
    }
    if (!smokefound) {
        float x = getX() + 60;
        float y = getY() + 60;
        objectManager.createObject((int)x, (int)y, 0, 0, SMOKETRAIL, getOwner());
        objectManager.createObject((int)(x + 40), (int)y, 0, 0, SMOKETRAIL, getOwner());
    }

    int current = 1;
    i = objectManager.list.begin();
    for (; i != objectManager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->getOwner() == getOwner()) {
            if (current == 1) {
                obj->setPos(getX() + 13, getY() + getHeight() - 2);
                obj->setVel(getXVel(), getYVel());

                extern Timer* timer;
                obj->update(*timer);
                obj->setVel(0, 0);
                current++;
            } else if (current == 2) {
                obj->setPos(getX() + 24, getY() + getHeight() - 2);
                obj->setVel(getXVel(), getYVel());

                extern Timer* timer;
                obj->update(*timer);
                obj->setVel(0, 0);
                break;
            }
        }
    }
}

void Ship::removeSmoketrail(ObjectManager& objectManager)
{
    int current = 1;
    ObjectList::iterator i = objectManager.list.begin();
    for (; i != objectManager.list.end(); i++) {
        Object* obj = *i;
        if (obj->name == "Smoketrail" && obj->getOwner() == getOwner()) {
            if (current == 1)
                obj->setEnergy(0);
            else if (current == 2) {
                obj->setEnergy(0);
                break;
            }
        }
    }
}

void Ship::calculateHitImg()
{
    hitImg.data = SDL_DisplayFormat(data);
    hitImg.fillRect(0, 0, hitImg.data->w, hitImg.data->h, 255, 0, 255);
}

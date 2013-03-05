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

#include "ObjectManager.h"
#include "SDLc/Screen.h"
#include "Ship.h"
#include "EnemySideways.h"
#include "EnemyRammer.h"
#include "EnemyBonus.h"
#include "ShotRocket.h"
#include <iostream>
#include <list>
#include <cmath>
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "PlayerState.h"
#include "SDLc/Misc.h"
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

ObjectManager::ObjectManager()
{
    amountOfEnemiesInList = 0;
    playersAlive = 0;
    worldYPos = 0;
}

ObjectManager::~ObjectManager()
{
    // clean up objectList
    ObjectList::iterator i = list.begin();
    for (; i != list.end(); i++) {
        Object* current = *i;
        delete current;
    }
    // clean up queue
    i = queue.begin();
    for (; i != queue.end(); i++) {
        Object* current = *i;
        delete current;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ObjectManager::loadData(std::string dataPath)
{
    // load objects
    obj[PLAYER1].load(dataPath + "gfx/Ship1.png");
    obj[PLAYER2].load(dataPath + "gfx/Ship2.png");
    obj[ENEMYSTD].load(dataPath + "gfx/EnemyStd.png");
    obj[ENEMYSIDEWAYS].load(dataPath + "gfx/EnemySideways.png");
    obj[ENEMYRAMMER].load(dataPath + "gfx/EnemyRammer.png");
    obj[ENEMYBONUS].load(dataPath + "gfx/EnemyBonus.png");
    obj[OBJECTBIGSHIP].load(dataPath + "gfx/ObjectBigship.png");
    obj[BONUSBLASTER].load(dataPath + "gfx/BonusBlaster.png");
    obj[BONUSROCKET].load(dataPath + "gfx/BonusRocket.png");

    // shots (for the weapons)
    obj[SHOTBLASTER].load(dataPath + "gfx/ShotBlaster.png");
    obj[SHOTBLASTERBIG].load(dataPath + "gfx/ShotBlasterBig.png");
    obj[SHOTROCKET].load(dataPath + "gfx/ShotRocket.png");
    obj[SHOTBOMBFRAGMENT].alloc(2, 2);
    obj[SHOTBOMBFRAGMENT].fillRect(0, 0, 2, 2, 100, 100, 255);
    obj[SHOTENEMYSTD].load(dataPath + "gfx/ShotStd2.png");
    snd[SND_SHOTBLASTER].load(dataPath + "soundfx/shoot1.wav");
    snd[SND_SHOTROCKET].load(dataPath + "soundfx/rocketshot.wav");

    // misc
    obj[SMOKETRAIL].load(dataPath + "gfx/Flame.png");
}

void ObjectManager::update(sdlc::Timer& timer, FxManager& fxManager, float worldYPos_, PlayerState& playerState)
{
    worldYPos = worldYPos_;

    ObjectList::iterator iterator = list.begin();
    while (iterator != list.end()) {
        Object* current = *iterator;
        iterator++;

        if (current->energy()) {
            // main functions
            current->think(*this, fxManager);
            current->update(timer);
            current->check_collisions(*this, fxManager);

            // kill objects who move outside their allowed area
            if (current->getY() > 480) {
                ObjType t = current->type();
                if (t == OBJ_ENEMY || t == OBJ_PASSIVE || \
                        t == OBJ_BONUS || t == OBJ_SHOT)
                    current->set_energy(0);
            } else if (current->type() == OBJ_SHOT) {
                if (current->getY() < 0 - current->getHeight() || \
                        current->getX() > 640 || \
                        current->getX() < 0 - current->getWidth())
                    current->set_energy(0);
            }
            // debug
            if (current->type() == OBJ_UNDEFINED)
                cout << "warning: object with undefined " << \
                     "type detected!" << endl;
            if (current->name == "Generic Object")
                cout << "warning: object with undefined " << \
                     "name detected!" << endl;
        }
    }

    // run maitenence functions
    updatePlayerState(playerState);
    flushList();        // flush dead objects
    addFromQueue();     // add the new objects
    updateEnemyCount(); // update/count how many enemies
}

void ObjectManager::draw(sdlc::Screen& screen)
{
    ObjectList::iterator i = list.begin();
    for (; i != list.end(); i++) {
        Object* current = *i;
        screen.blit(*current);
    }
}

Object* ObjectManager::createObject(int x, int y, float xVel, float yVel, \
                                    ObjIndex object, Owner owner)
{
    Object* tmp = 0;

    if (object >= ENEMYSTD_V_FORMATION)
        createFormation(x, y, xVel, yVel, object);

    else {
        // enemies
        if (object == ENEMYSTD)
            tmp = new Object("Standard Enemy", 5, 52, obj[ENEMYSTD], OBJ_ENEMY, SCROLLING_SPEED);
        else if (object == ENEMYSIDEWAYS)
            tmp = new EnemySideways("Sideways Enemy", 3, 71, obj[ENEMYSIDEWAYS]);
        else if (object == ENEMYRAMMER)
            tmp = new EnemyRammer("Rammer Enemy", 2, 43, obj[ENEMYRAMMER]);
        else if (object == ENEMYBONUS) {
            tmp = new EnemyBonus("Bonus Enemy", 10, 200, obj[ENEMYBONUS]);
            tmp->initAnimation(50, 2, 0);
        }

        // passive objects
        else if (object == OBJECTBIGSHIP) {
            tmp = new Object("Object Bigship", 5, 0, obj[OBJECTBIGSHIP], OBJ_PASSIVE,  10.0f);
            tmp->initAnimation(40, 2, 0);
        }

        // bonuses
        else if (object == BONUSBLASTER) {
            tmp = new Object("Blaster Bonus", 1, 0, obj[BONUSBLASTER], OBJ_BONUS,  20.0f);
            tmp->initAnimation(40, 3, 0);
        } else if (object == BONUSROCKET) {
            tmp = new Object("Rocket Bonus", 1, 0, obj[BONUSROCKET], OBJ_BONUS,  20.0f);
            tmp->initAnimation(40, 3, 0);
        }

        // shots
        else if (object == SHOTBLASTER)
            tmp = new Shot("Blaster Shot", 1, obj[SHOTBLASTER], owner);
        else if (object == SHOTBLASTERBIG)
            tmp = new Shot("Big Blaster Shot", 1, obj[SHOTBLASTERBIG], owner);
        else if (object == SHOTROCKET)
            tmp = new ShotRocket("Rocket Shot", 10, obj[SHOTROCKET], owner);
        else if (object == SHOTBOMBFRAGMENT)
            tmp = new Shot("Bomb Fragment Shot", 4, obj[SHOTBOMBFRAGMENT], owner);
        else if (object == SHOTENEMYSTD)
            tmp = new Shot("Standard Enemy Shot", 1, obj[SHOTENEMYSTD], owner); // kolla

        // misc
        else if (object == SMOKETRAIL) {
            tmp = new Object("Smoketrail", 1, 0, obj[SMOKETRAIL], OBJ_PLAYERPASSIVE, 0);
            tmp->set_owner(owner);
            tmp->initAnimation(40, 2, 0);
        }


        else cout << "undefined object passed to ObjectManager::createObject()" << endl;

        tmp->setX(x - tmp->getWidth() / 2);
        tmp->setY(y - tmp->getHeight() / 2 + worldYPos);
        tmp->setXVel(xVel);
        tmp->setYVel(yVel);
        queue.push_back(tmp);
    }
    return tmp;
}

Object* ObjectManager::createObject(int x, int y, ObjIndex object, \
                                    float vel, float angle, Owner owner)
{
    angle = angle * (3.1415927f / 180.0f);

    float xVel = cos(angle) * vel;
    float yVel = -sin(angle) * vel;
    //if(xVel < 0.0000001) xVel = 0;
    //if(yVel < 0.0000001) yVel = 0;
    return (createObject(x, y, xVel, yVel, object, owner));
}

void ObjectManager::createShips(PlayerState& playerState)
{
    int i, j;
    for (i = 1; i <= NUM_OF_POSSIBLE_PLAYERS; i++) {
        if (playerState.energy_max(i)) {
            Weapon* w1 = 0;
            Weapon* w2 = 0;
            int count = 0;

            //if(playerState.main_weapon(i) == WEAPON_MAIN_BLASTER)
            if (playerState.main_weapon(i) == "Blaster Weapon")
                w1 = new WeaponBlaster(snd[SND_SHOTBLASTER], (Owner)(OWNER_PLAYER1 + i - 1));
            for (j = 1; j < playerState.main_weapon_level(i); j++)
                w1->upgrade();

            //if(playerState.extra_weapon(i) == WEAPON_EXTRA_ROCKET)
            if (playerState.extra_weapon(i) == "Rocket Weapon") {
                w2 = new WeaponRocket(snd[SND_SHOTROCKET], (Owner)(OWNER_PLAYER1 + i - 1));
                w2->setCount(playerState.extra_weapon_count(i));
            }
            string name = "Player " + std::to_string(i);
            KeySet keyset;
            if (i == 1) {
                keyset.up = SDLK_UP;
                keyset.left = SDLK_LEFT;
                keyset.down = SDLK_DOWN;
                keyset.right = SDLK_RIGHT;
                keyset.fire_main = SDLK_RCTRL;
                keyset.fire_extra = SDLK_RSHIFT;
            } else if (i == 2) {
                keyset.up = SDLK_w;
                keyset.left = SDLK_a;
                keyset.down = SDLK_s;
                keyset.right = SDLK_d;
                keyset.fire_main = SDLK_LSHIFT;
                keyset.fire_extra = SDLK_GREATER;
            } else cout << "ObjectManager::createShips() keys not set!" << endl;

            playerState.set_keyset(i, keyset);

            int e = playerState.energy_max(i);
            playerState.set_energy(i, e);
            int s = playerState.score(i);
            Object* tmp = new Ship(name, e, s, obj[PLAYER1 + i - 1], w1, w2, keyset);

            tmp->lockedToScreen(false);
            tmp->setX(213 * i - tmp->getWidth());
            //tmp->setY(400 - tmp->getHeight());
            tmp->setY(480);
            tmp->setVel(0, -200.0f/*-0.200f*/);
            queue.push_back(tmp);
        }
    }
}

int ObjectManager::getHowManyEnemies()
{
    return amountOfEnemiesInList;
}
int ObjectManager::getHowManyPlayersAlive()
{
    return playersAlive;
}
// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void ObjectManager::createFormation(int x, int y, float xVel, float yVel, enum ObjIndex object)
{
    if (object == ENEMYSTD_V_FORMATION) {
        createObject(x - 80, y - 40, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(x - 40, y - 20, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(x   , y   , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(x + 40, y - 20, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(x + 80, y - 40, xVel, yVel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_3V_FORMATION) {
        createObject(300 - 80, y - 40, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 - 40, y - 20, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300   , y   , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 40, y - 20, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 80, y - 40, xVel, yVel, ENEMYSTD, OWNER_NONE);

        createObject(300 - 260, y - 140, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 - 220, y - 120, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 - 180, y - 100, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 - 140, y - 120, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 - 100, y - 140, xVel, yVel, ENEMYSTD, OWNER_NONE);

        createObject(300 + 100, y - 140, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 140, y - 120, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 180, y - 100, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 220, y - 120, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300 + 260, y - 140, xVel, yVel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_DIAGONAL_FORMATION) {
        createObject(0  , y    , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(50 , y - 10 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(100, y - 20 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(150, y - 30 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(200, y - 40 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(250, y - 50 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(300, y - 60 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(350, y - 70 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(400, y - 80 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(450, y - 90 , xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(500, y - 100, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(550, y - 110, xVel, yVel, ENEMYSTD, OWNER_NONE);
        createObject(600, y - 120, xVel, yVel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_MASSIVE_FORMATION) {
        int i;
        for (i = 0; i < 20; i++)
            createObject((rand() % 560) + 40 , y - (rand() % 200), xVel, yVel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_VLINE_FORMATION) {
        createObject(x   , y   , xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 5 , y - 20, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 10, y - 40, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 15, y - 60, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 20, y - 80, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_HLINE_FORMATION) {
        createObject(x - 160, y, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x - 80, y, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x   , y, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 80, y, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 160, y, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_V_FORMATION) {
        createObject(x - 80, y - 40, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x - 40, y - 20, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x   , y   , xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 40, y - 20, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
        createObject(x + 80, y - 40, xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_MASSIVE_FORMATION) {
        int i;
        for (i = 0; i < 20; i++)
            createObject((rand() % 560) + 40 , y - (rand() % 200), xVel, yVel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYRAMMER_VLINE_FORMATION) {
        createObject(x, y   , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x, y - 20, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x, y - 40, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x, y - 60, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x, y - 80, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
    } else if (object == ENEMYRAMMER_DIAGONAL_FORMATION) {
        createObject(x - 80, y   , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x - 40, y - 20, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x   , y - 40, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x + 40, y - 60, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(x + 80, y - 80, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
    } else if (object == ENEMYRAMMER_FULLDIAGONAL_FORMATION) {
        createObject(10 , y - 120, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(110, y - 100, xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(210, y - 80 , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(310, y - 60 , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(410, y - 40 , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(510, y - 20 , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
        createObject(610, y    , xVel, yVel, ENEMYRAMMER, OWNER_NONE);
    }
}

void ObjectManager::updatePlayerState(PlayerState& playerState)
{
    ObjectList::iterator i = list.begin();
    for (; i != list.end(); i++) {
        Object* obj = *i;

        if (obj->type() == OBJ_PLAYER) {
            int which = obj->name[obj->name.length() - 1] - (int)'0';

            playerState.set_energy(which, obj->energy());
            playerState.set_energy_max(which, obj->energy_max());
            playerState.set_score(which, obj->score());

            Ship* tmp;
            tmp = (Ship*)obj;
            if (tmp->main_weapon_) {
                playerState.set_main_weapon(which, tmp->main_weapon_->name);
                playerState.set_main_weapon_level(which, tmp->main_weapon_->getLevel());
            }
            if (tmp->extra_weapon_) {
                playerState.set_extra_weapon(which, tmp->extra_weapon_->name);
                playerState.set_extra_weapon_count(which, tmp->extra_weapon_->getCount());
            } else playerState.set_extra_weapon(which, "none");

            /*if(tmp->mainWeapon && tmp->mainWeapon->name == "Blaster Weapon")
            playerState.set_main_weapon(which, WEAPON_MAIN_BLASTER);
            if(tmp->extraWeapon && tmp->extraWeapon->name == "Rocket Weapon")
            playerState.set_extra_weapon(which, WEAPON_EXTRA_ROCKET);
            */
            //cout << tmp->mainWeapon->name;
            //cout << "\t" << tmp->extraWeapon->name << endl;
            //playerState.set_main_weapon_level();
            //playerState.set_extra_weapon();

            if (obj->energy() == 0)
                playerState.set_energy_max(which, 0);
        }
    }
}

void ObjectManager::flushList()
{
    ObjectList::iterator i = list.begin();
    while (i != list.end()) {
        Object* current = *i;
        i++;
        if (current->energy() <= 0) {
            delete current;
            list.remove(current);
            //break;
        }
    }
}

void ObjectManager::addFromQueue()
{
    ObjectList::iterator i = queue.begin();
    while (i != queue.end()) {
        Object* current = *i;
        i++;

        if (worldYPos < current->getY() + current->getHeight()) {
            current->setY(current->getY() - worldYPos);

            current->active(true);
            current->activate(*this);

            if (list.empty() || current->type() == OBJ_SHOT)
                list.push_back(current);
            else {
                unsigned j;
                ObjectList::iterator iterator = list.begin();
                for (j = 0; j < list.size(); j++) {
                    Object* currentList = *iterator;
                    if (current->type() < currentList->type()) {
                        list.insert(iterator, current);
                        j = list.size(); // break;
                    }

                    // when we reach the end of the list
                    else if (j == list.size() - 1) {
                        list.push_back(current);
                        j = list.size(); // break;
                    }
                    iterator++;
                }
            }
            queue.remove(current);
        }
    }
}

void ObjectManager::updateEnemyCount()
{
    amountOfEnemiesInList = 0;
    playersAlive = 0;
    ObjectList::iterator iterator;
    for (iterator = list.begin(); iterator != list.end(); iterator++) {
        Object* current = *iterator;
        ObjType t = current->type();
        if (t == OBJ_ENEMY || t == OBJ_BONUS)
            amountOfEnemiesInList++;
        else if (t == OBJ_PLAYER)
            playersAlive++;
    }
    for (iterator = queue.begin(); iterator != queue.end(); iterator++) {
        Object* current = *iterator;
        ObjType t = current->type();
        if (t == OBJ_ENEMY || t == OBJ_BONUS)
            amountOfEnemiesInList++;
        else if (t == OBJ_PLAYER)
            playersAlive++;
    }
}

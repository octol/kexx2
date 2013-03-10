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

#include <cmath>
#include <list>
#include <iostream>

#include "SDLc.h"

#include "Ship.h"
#include "EnemySideways.h"
#include "EnemyRammer.h"
#include "EnemyBonus.h"
#include "ShotRocket.h"
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

ObjectManager::~ObjectManager()
{
    // TODO: range based for loops.
    // clean up objectList
    for (auto i = list.begin(); i != list.end(); ++i) {
        Object* current = *i;
        delete current;
    }
    // TODO: range based for loops.
    // clean up queue
    for (auto i = queue.begin(); i != queue.end(); ++i) {
        Object* current = *i;
        delete current;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ObjectManager::load_data(std::string data_path)
{
    // load objects
    obj[PLAYER1].load(data_path + "gfx/Ship1.png");
    obj[PLAYER2].load(data_path + "gfx/Ship2.png");
    obj[ENEMYSTD].load(data_path + "gfx/EnemyStd.png");
    obj[ENEMYSIDEWAYS].load(data_path + "gfx/EnemySideways.png");
    obj[ENEMYRAMMER].load(data_path + "gfx/EnemyRammer.png");
    obj[ENEMYBONUS].load(data_path + "gfx/EnemyBonus.png");
    obj[OBJECTBIGSHIP].load(data_path + "gfx/ObjectBigship.png");
    obj[BONUSBLASTER].load(data_path + "gfx/BonusBlaster.png");
    obj[BONUSROCKET].load(data_path + "gfx/BonusRocket.png");

    // shots (for the weapons)
    obj[SHOTBLASTER].load(data_path + "gfx/ShotBlaster.png");
    obj[SHOTBLASTERBIG].load(data_path + "gfx/ShotBlasterBig.png");
    obj[SHOTROCKET].load(data_path + "gfx/ShotRocket.png");
    obj[SHOTBOMBFRAGMENT].alloc(2, 2);
    obj[SHOTBOMBFRAGMENT].fillRect(0, 0, 2, 2, 100, 100, 255);
    obj[SHOTENEMYSTD].load(data_path + "gfx/ShotStd2.png");
    snd[SND_SHOTBLASTER].load(data_path + "soundfx/shoot1.wav");
    snd[SND_SHOTROCKET].load(data_path + "soundfx/rocketshot.wav");

    // misc
    obj[SMOKETRAIL].load(data_path + "gfx/Flame.png");
}

void ObjectManager::update(sdlc::Timer& timer, FxManager& fx_manager, 
                           float world_y_pos, PlayerState& player_state)
{
    world_y_pos_ = world_y_pos;

    // TODO: nicer loop
    ObjectList::iterator iterator = list.begin();
    while (iterator != list.end()) {
        Object* current = *iterator;
        ++iterator;

        if (current->energy()) {
            // main functions
            current->think(*this, fx_manager);
            current->update(timer);
            current->check_collisions(*this, fx_manager);

            // kill objects who move outside their allowed area
            if (current->getY() > 480) {
                ObjType t = current->type();
                if (t == OBJ_ENEMY || t == OBJ_PASSIVE 
                        || t == OBJ_BONUS || t == OBJ_SHOT) {
                    current->set_energy(0);
                }
            } else if (current->type() == OBJ_SHOT) {
                if (current->getY() < 0 - current->getHeight() 
                        || current->getX() > 640 
                        || current->getX() < 0 - current->getWidth()) {
                    current->set_energy(0);
                }
            }
            // debug
            if (current->type() == OBJ_UNDEFINED) {
                std::cout << "warning: object with undefined " 
                    << "type detected!" << std::endl;
            }
            if (current->name == "Generic Object") {
                std::cout << "warning: object with undefined " 
                    << "name detected!" << std::endl;
            }
        }
    }

    // run maintenance functions
    update_player_state(player_state);
    flush_list();               // flush dead objects
    add_from_queue();           // add the new objects
    update_enemy_count();       // update/count how many enemies
}

void ObjectManager::draw(sdlc::Screen& screen)
{
    //ObjectList::iterator i = list.begin();
    // TODO: range based loop
    for (auto i = list.begin(); i != list.end(); i++) {
        Object* current = *i;
        screen.blit(*current);
    }
}

Object* ObjectManager::create_object(int x, int y, float x_vel, float y_vel, 
                                     ObjIndex object, Owner owner)
{
    // TODO: Redo to use a Factory pattern instead.
    
    Object* tmp = 0;

    if (object >= ENEMYSTD_V_FORMATION)
        create_formation(x, y, x_vel, y_vel, object);

    else {
        // enemies
        if (object == ENEMYSTD)
            tmp = new Object("Standard Enemy", 5, 52, 
                    obj[ENEMYSTD], OBJ_ENEMY, SCROLLING_SPEED);
        else if (object == ENEMYSIDEWAYS)
            tmp = new EnemySideways("Sideways Enemy", 3, 71, 
                    obj[ENEMYSIDEWAYS]);
        else if (object == ENEMYRAMMER)
            tmp = new EnemyRammer("Rammer Enemy", 2, 43, obj[ENEMYRAMMER]);
        else if (object == ENEMYBONUS) {
            tmp = new EnemyBonus("Bonus Enemy", 10, 200, obj[ENEMYBONUS]);
            tmp->initAnimation(50, 2, 0);
        }

        // passive objects
        else if (object == OBJECTBIGSHIP) {
            tmp = new Object("Object Bigship", 5, 0, obj[OBJECTBIGSHIP], 
                    OBJ_PASSIVE,  10.0f);
            tmp->initAnimation(40, 2, 0);
        }

        // bonuses
        else if (object == BONUSBLASTER) {
            tmp = new Object("Blaster Bonus", 1, 0, obj[BONUSBLASTER], 
                    OBJ_BONUS,  20.0f);
            tmp->initAnimation(40, 3, 0);
        } else if (object == BONUSROCKET) {
            tmp = new Object("Rocket Bonus", 1, 0, obj[BONUSROCKET], 
                    OBJ_BONUS,  20.0f);
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
            tmp = new Shot("Bomb Fragment Shot", 4, obj[SHOTBOMBFRAGMENT], 
                    owner);
        else if (object == SHOTENEMYSTD)
            // TODO: check this (why?)
            tmp = new Shot("Standard Enemy Shot", 1, obj[SHOTENEMYSTD], owner);

        // misc
        else if (object == SMOKETRAIL) {
            tmp = new Object("Smoketrail", 1, 0, obj[SMOKETRAIL], 
                    OBJ_PLAYERPASSIVE, 0);
            tmp->set_owner(owner);
            tmp->initAnimation(40, 2, 0);
        }

        else {
            std::cout << "ObjectManager::create_object(): undefined object." 
                << std::endl;
        }

        tmp->setX(x - tmp->getWidth() / 2);
        tmp->setY(y - tmp->getHeight() / 2 + world_y_pos_);
        tmp->setXVel(x_vel);
        tmp->setYVel(y_vel);
        queue.push_back(tmp);
    }
    return tmp;
}

Object* ObjectManager::create_object(int x, int y, ObjIndex object, 
                                     float vel, float angle, Owner owner)
{
    angle = angle * (3.1415927f / 180.0f);

    float x_vel = cos(angle) * vel;
    float y_vel = -sin(angle) * vel;
    return (create_object(x, y, x_vel, y_vel, object, owner));
}

void ObjectManager::create_ships(PlayerState& player_state)
{
    int i, j;
    for (i = 1; i <= NUM_OF_POSSIBLE_PLAYERS; i++) {
        if (player_state.energy_max(i)) {
            Weapon* w1 = 0;
            Weapon* w2 = 0;
            int count = 0;

            if (player_state.main_weapon(i) == "Blaster Weapon")
                // TODO: remove C style cast.
                w1 = new WeaponBlaster(snd[SND_SHOTBLASTER], 
                        (Owner)(OWNER_PLAYER1 + i - 1));
            for (j = 1; j < player_state.main_weapon_level(i); j++)
                w1->upgrade();

            if (player_state.extra_weapon(i) == "Rocket Weapon") {
                // TODO: remove C style cast
                w2 = new WeaponRocket(snd[SND_SHOTROCKET], 
                        (Owner)(OWNER_PLAYER1 + i - 1));
                w2->set_count(player_state.extra_weapon_count(i));
            }
            std::string name = "Player " + std::to_string(i);
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
            } else {
                std::cout << "ObjectManager::createShips() keys not set!" 
                    << std::endl;
            }

            player_state.set_keyset(i, keyset);

            int e = player_state.energy_max(i);
            player_state.set_energy(i, e);
            int s = player_state.score(i);
            Object* tmp = new Ship(name, e, s, obj[PLAYER1 + i - 1], w1, w2, 
                    keyset);

            tmp->lockedToScreen(false);
            tmp->setX(213 * i - tmp->getWidth());
            tmp->setY(480);
            tmp->setVel(0, -200.0f);
            queue.push_back(tmp);
        }
    }
}

int ObjectManager::num_of_enemies()
{
    return amount_of_enemies_in_list_;
}

int ObjectManager::num_of_players_alive()
{
    return players_alive_;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void ObjectManager::create_formation(int x, int y, float x_vel, float y_vel, 
                                     enum ObjIndex object)
{
    if (object == ENEMYSTD_V_FORMATION) {
        create_object(x - 80, y - 40, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(x - 40, y - 20, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(x     , y     , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(x + 40, y - 20, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(x + 80, y - 40, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_3V_FORMATION) {
        create_object(300 - 80, y - 40, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 - 40, y - 20, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300     , y     , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 40, y - 20, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 80, y - 40, x_vel, y_vel, ENEMYSTD, OWNER_NONE);

        create_object(300 - 260, y - 140, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 - 220, y - 120, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 - 180, y - 100, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 - 140, y - 120, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 - 100, y - 140, x_vel, y_vel, ENEMYSTD, OWNER_NONE);

        create_object(300 + 100, y - 140, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 140, y - 120, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 180, y - 100, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 220, y - 120, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300 + 260, y - 140, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_DIAGONAL_FORMATION) {
        create_object(0  , y    , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(50 , y - 10 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(100, y - 20 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(150, y - 30 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(200, y - 40 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(250, y - 50 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(300, y - 60 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(350, y - 70 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(400, y - 80 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(450, y - 90 , x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(500, y - 100, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(550, y - 110, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
        create_object(600, y - 120, x_vel, y_vel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSTD_MASSIVE_FORMATION) {
        int i;
        for (i = 0; i < 20; i++)
            create_object((rand() % 560) + 40 , y - (rand() % 200), x_vel, 
                    y_vel, ENEMYSTD, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_VLINE_FORMATION) {
        create_object(x     , y     , x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 5 , y - 20, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 10, y - 40, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 15, y - 60, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 20, y - 80, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_HLINE_FORMATION) {
        create_object(x - 160, y, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x - 80 , y, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x      , y, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 80 , y, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 160, y, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_V_FORMATION) {
        create_object(x - 80, y - 40, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x - 40, y - 20, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x     , y     , x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 40, y - 20, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
        create_object(x + 80, y - 40, x_vel, y_vel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYSIDEWAYS_MASSIVE_FORMATION) {
        int i;
        for (i = 0; i < 20; i++)
            create_object((rand() % 560) + 40 , y - (rand() % 200), x_vel, 
                    y_vel, ENEMYSIDEWAYS, OWNER_NONE);
    } else if (object == ENEMYRAMMER_VLINE_FORMATION) {
        create_object(x, y     , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x, y - 20, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x, y - 40, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x, y - 60, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x, y - 80, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
    } else if (object == ENEMYRAMMER_DIAGONAL_FORMATION) {
        create_object(x - 80, y   , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x - 40, y - 20, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x     , y - 40, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x + 40, y - 60, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(x + 80, y - 80, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
    } else if (object == ENEMYRAMMER_FULLDIAGONAL_FORMATION) {
        create_object(10 , y - 120, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(110, y - 100, x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(210, y - 80 , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(310, y - 60 , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(410, y - 40 , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(510, y - 20 , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
        create_object(610, y      , x_vel, y_vel, ENEMYRAMMER, OWNER_NONE);
    }
}

void ObjectManager::update_player_state(PlayerState& player_state)
{
    // TODO: range based loop
    ObjectList::iterator i = list.begin();
    for (; i != list.end(); i++) {
        Object* obj = *i;

        if (obj->type() == OBJ_PLAYER) {
            int which = obj->name[obj->name.length() - 1] - (int)'0';

            player_state.set_energy(which, obj->energy());
            player_state.set_energy_max(which, obj->energy_max());
            player_state.set_score(which, obj->score());

            Ship* tmp;
            tmp = (Ship*)obj;
            if (tmp->main_weapon_) {
                player_state.set_main_weapon(which, tmp->main_weapon_->name);
                player_state.set_main_weapon_level(which, 
                        tmp->main_weapon_->level());
            }
            if (tmp->extra_weapon_) {
                player_state.set_extra_weapon(which, tmp->extra_weapon_->name);
                player_state.set_extra_weapon_count(which, 
                        tmp->extra_weapon_->count());
            } else player_state.set_extra_weapon(which, "none");

            if (obj->energy() == 0)
                player_state.set_energy_max(which, 0);
        }
    }
}

void ObjectManager::flush_list()
{
    // TODO: range based loop.
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

void ObjectManager::add_from_queue()
{
    // TODO: range based loop
    ObjectList::iterator i = queue.begin();
    while (i != queue.end()) {
        Object* current = *i;
        i++;

        if (world_y_pos_ < current->getY() + current->getHeight()) {
            current->setY(current->getY() - world_y_pos_);

            current->active(true);
            current->activate(*this);

            if (list.empty() || current->type() == OBJ_SHOT)
                list.push_back(current);
            else {
                unsigned j;
                // TODO: range based loop
                ObjectList::iterator iterator = list.begin();
                for (j = 0; j < list.size(); j++) {
                    Object* current_list = *iterator;
                    if (current->type() < current_list->type()) {
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

void ObjectManager::update_enemy_count()
{
    amount_of_enemies_in_list_ = 0;
    players_alive_ = 0;

    // TODO: range based loop
    ObjectList::iterator iterator;
    for (iterator = list.begin(); iterator != list.end(); iterator++) {
        Object* current = *iterator;
        ObjType t = current->type();
        if (t == OBJ_ENEMY || t == OBJ_BONUS)
            amount_of_enemies_in_list_++;
        else if (t == OBJ_PLAYER)
            players_alive_++;
    }
    // TODO: range based loop
    for (iterator = queue.begin(); iterator != queue.end(); iterator++) {
        Object* current = *iterator;
        ObjType t = current->type();
        if (t == OBJ_ENEMY || t == OBJ_BONUS)
            amount_of_enemies_in_list_++;
        else if (t == OBJ_PLAYER)
            players_alive_++;
    }
}

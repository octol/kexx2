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

#ifndef KEXX2_OBJECTMANAGER_H
#define KEXX2_OBJECTMANAGER_H

#include <list>
#include <memory>
#include "SDLc.h"
#include "Defines.h"
#include "Object.h"

class FxManager;
class PlayerState;
class Weapon;

typedef std::list<Object*> ObjectList;

class ObjectManager final {
public:
    ~ObjectManager();

    void load_data(std::string data_path);

    void update(sdlc::Timer& timer, FxManager& fx_manager, float world_y_pos, 
                PlayerState& player_state);
    void draw(sdlc::Screen& screen);

    Object* create_object(int x, int y, float x_vel, float y_vel, 
                          ObjIndex object, Owner owner);
    Object* create_object(int x, int y, ObjIndex object, float vel, 
                          float angle, Owner owner);
    void create_ships(PlayerState& player_state);

    int num_of_enemies();
    int num_of_players_alive();

    // TODO: replace C arrays
    ObjectList list;                                // object list
    sdlc::Surface obj[ENEMYSTD_V_FORMATION];        // object graphics
    sdlc::Sound snd[SND_SHOTROCKET + 1];            // object sound

private:
    Object* allocate_object(ObjIndex object, Owner owner);
    void create_formation(int x, int y, float x_vel, float y_vel, 
                          enum ObjIndex object);

    std::unique_ptr<Weapon> create_main_weapon(int player, 
                                               PlayerState player_state);
    std::unique_ptr<Weapon> create_extra_weapon(int player, 
                                                PlayerState player_state);

    // list maintenance functions
    void update_player_state(PlayerState& player_state);
    void flush_list();
    void add_from_queue();
    void update_enemy_count();

    ObjectList queue;
    int amount_of_enemies_in_list_ = 0;
    int players_alive_ = 0;
    float world_y_pos_ = 0;
};

#endif

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

#ifndef _KEXX2_PLAYERSTATE_H_
#define _KEXX2_PLAYERSTATE_H_

#include "Weapon.h"
#include "Defines.h"

struct KeySet {
    KeySet();
    int left;
    int right;
    int up;
    int down;
    int fireMain;
    int fireExtra;
};

class PlayerState {
public:
    PlayerState();
    virtual ~PlayerState();

    int getEnergy(int player);
    int setEnergy(int player, int value);

    int getEnergyMax(int player);
    int setEnergyMax(int player, int value);

    int getScore(int player);
    int setScore(int player, int score);

    int getMainWeaponLevel(int player);
    int setMainWeaponLevel(int player, int level);

    int getExtraWeaponCount(int player);
    int setExtraWeaponCount(int player, int count);

    std::string getMainWeapon(int player);
    std::string setMainWeapon(int player, std::string s);
    std::string getExtraWeapon(int player);
    std::string setExtraWeapon(int player, std::string s);

    KeySet getKeySet(int player);
    KeySet setKeySet(int player, KeySet keySet_);

    bool anyoneAlive();
    void killall();

private:
    struct Player_ {
        int energy;
        int energyMax;
        int score;
        int mainWeaponLevel;
        int extraWeaponCount;
        std::string mainWeapon;
        std::string extraWeapon;
        KeySet keySet;
    } player_[NUM_OF_POSSIBLE_PLAYERS];
};

#endif

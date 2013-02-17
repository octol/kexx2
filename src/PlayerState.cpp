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

#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

KeySet::KeySet()
{
    left = right = up = down = fireMain = fireExtra = 0;
}

PlayerState::PlayerState()
{
    int i;
    for (i = 0; i < NUM_OF_POSSIBLE_PLAYERS; i++) {
        player_[i].energy = 0;
        player_[i].energyMax = 0;
        player_[i].score = 0;
        player_[i].mainWeaponLevel = 1;
        player_[i].extraWeaponCount = 0;
        player_[i].mainWeapon = "Blaster Weapon";
        player_[i].extraWeapon = "none";
        //player_[i].mainWeapon = WEAPON_MAIN_BLASTER;
        //player_[i].extraWeapon = WEAPON_NONE;
    }
}

PlayerState::~PlayerState()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

int PlayerState::getEnergy(int player)
{
    return player_[player - 1].energy;
}

int PlayerState::setEnergy(int player, int value)
{
    return (player_[player - 1].energy = value);
}

int PlayerState::getEnergyMax(int player)
{
    return player_[player - 1].energyMax;
}

int PlayerState::setEnergyMax(int player, int value)
{
    return (player_[player - 1].energyMax = value);
}

int PlayerState::getScore(int player)
{
    return player_[player - 1].score;
}

int PlayerState::setScore(int player, int score)
{
    return (player_[player - 1].score = score);
}

int PlayerState::getMainWeaponLevel(int player)
{
    return player_[player - 1].mainWeaponLevel;
}

int PlayerState::setMainWeaponLevel(int player, int level)
{
    return (player_[player - 1].mainWeaponLevel = level);
}

int PlayerState::getExtraWeaponCount(int player)
{
    return player_[player - 1].extraWeaponCount;
}

int PlayerState::setExtraWeaponCount(int player, int count)
{
    return (player_[player - 1].extraWeaponCount = count);
}

std::string PlayerState::getMainWeapon(int player)
{
    return player_[player - 1].mainWeapon;
}

std::string PlayerState::setMainWeapon(int player, std::string s)
{
    return (player_[player - 1].mainWeapon = s);
}

std::string PlayerState::getExtraWeapon(int player)
{
    return player_[player - 1].extraWeapon;
}

std::string PlayerState::setExtraWeapon(int player, std::string s)
{
    return (player_[player - 1].extraWeapon = s);
}

KeySet PlayerState::getKeySet(int player)
{
    return player_[player - 1].keySet;
}

KeySet PlayerState::setKeySet(int player, KeySet keySet_)
{
    return (player_[player - 1].keySet = keySet_);
}

bool PlayerState::anyoneAlive()
{
    bool a = false;
    int i;
    for (i = 0; i < NUM_OF_POSSIBLE_PLAYERS; i++) {
        if (player_[i].energyMax)
            a = true;
    }
    return a;
}

void PlayerState::killall()
{
    int i;
    for (i = 0; i < NUM_OF_POSSIBLE_PLAYERS; i++) {
        player_[i].energyMax = 0;
        player_[i].energy = 0;
        player_[i].score = 0;
        player_[i].mainWeaponLevel = 1;
        player_[i].extraWeaponCount = 0;
        player_[i].mainWeapon = "Blaster Weapon";
        player_[i].extraWeapon = "none";

        /*int energy;
        int energyMax;
        int score;
        int mainWeaponLevel;
        int extraWeaponCount;
        std::string mainWeapon;
        std::string extraWeapon;
        KeySet keySet;*/
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

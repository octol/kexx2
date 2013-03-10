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

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

int PlayerState::energy(int player)
{
    return player_[player - 1].energy;
}

int PlayerState::set_energy(int player, int value)
{
    return (player_[player - 1].energy = value);
}

int PlayerState::energy_max(int player)
{
    return player_[player - 1].energy_max;
}

int PlayerState::set_energy_max(int player, int value)
{
    return (player_[player - 1].energy_max = value);
}

int PlayerState::score(int player)
{
    return player_[player - 1].score;
}

int PlayerState::set_score(int player, int score)
{
    return (player_[player - 1].score = score);
}

int PlayerState::main_weapon_level(int player)
{
    return player_[player - 1].main_weapon_level;
}

int PlayerState::set_main_weapon_level(int player, int level)
{
    return (player_[player - 1].main_weapon_level = level);
}

int PlayerState::extra_weapon_count(int player)
{
    return player_[player - 1].extra_weapon_count;
}

int PlayerState::set_extra_weapon_count(int player, int count)
{
    return (player_[player - 1].extra_weapon_count = count);
}

std::string PlayerState::main_weapon(int player)
{
    return player_[player - 1].main_weapon;
}

std::string PlayerState::set_main_weapon(int player, std::string s)
{
    return (player_[player - 1].main_weapon = s);
}

std::string PlayerState::extra_weapon(int player)
{
    return player_[player - 1].extra_weapon;
}

std::string PlayerState::set_extra_weapon(int player, std::string s)
{
    return (player_[player - 1].extra_weapon = s);
}

KeySet PlayerState::keyset(int player)
{
    return player_[player - 1].keyset;
}

KeySet PlayerState::set_keyset(int player, KeySet keyset_)
{
    return (player_[player - 1].keyset = keyset_);
}

bool PlayerState::anyone_alive()
{
    bool a = false;
    int i;
    for (i = 0; i < NUM_OF_POSSIBLE_PLAYERS; i++) {
        if (player_[i].energy_max)
            a = true;
    }
    return a;
}

void PlayerState::kill_all()
{
    int i;
    for (i = 0; i < NUM_OF_POSSIBLE_PLAYERS; i++) {
        player_[i].energy_max = 0;
        player_[i].energy = 0;
        player_[i].score = 0;
        player_[i].main_weapon_level = 1;
        player_[i].extra_weapon_count = 0;
        player_[i].main_weapon = "Blaster Weapon";
        player_[i].extra_weapon = "none";

        /*int energy;
        int energy_max;
        int score;
        int main_weapon_level;
        int extra_weapon_count;
        std::string main_weapon;
        std::string extra_weapon;
        KeySet keyset;*/
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

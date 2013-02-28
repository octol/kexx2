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

#include "Interface.h"
#include "SDLc/Font.h"
#include "SDLc/Screen.h"
#include "SDLc/Misc.h"
#include "Object.h"
#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Interface::update(int num_of_players, const PlayerState& player_state)
{
    num_of_players_ = num_of_players;
    player_state_ = player_state;
}

void Interface::draw(Font& font, Screen& screen)
{
    // player 1
    int e = player_state_.energy(1);
    int eMax = player_state_.energy_max(1);
    int s = player_state_.score(1);

    if (!e)
        screen.print(20, 440, "dead", font);
    else 
        draw_energy(20, 440, e, eMax, font, screen);

    draw_score(20, 20, s, font, screen);
    draw_weapons(20, 40, player_state_, 1, font, screen);

    // player 2
    e = player_state_.energy(2);
    eMax = player_state_.energy_max(2);
    s = player_state_.score(2);
    if (num_of_players_ >= 2) {
        if (!e)
            screen.print(560, 440, "dead", font);
        else 
            draw_energy(560, 440, e, eMax, font, screen);

        draw_score(400, 20, s, font, screen);
        draw_weapons(400, 40, player_state_, 2, font, screen);
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Interface::draw_energy(int x, int y, int value, int max_value, Font& font, Screen& screen)
{
    int w = 20 * max_value;
    int h = 20;
    int multi = (w / max_value);

    screen.fillRect(x - 1, y - 1, w + 2, h + 2, 150, 150, 150);
    screen.fillRect(x, y, w, h, 0, 0, 0);
    if (value > 0)
        screen.fillRect(x + 1, y + 1, multi * (max_value - (max_value - value)) - 1, \
                        h - 1, 100, 100, 100);
}

void Interface::draw_score(int x, int y, int value, Font& font, Screen& screen)
{
    screen.print(x, y, "score: " + std::to_string(value), font);
}

void Interface::draw_weapons(int x, int y, PlayerState& ps, int player, Font& font, Screen& screen)
{
    int length1 = (ps.main_weapon(player)).length();
    std::string text1 = (ps.main_weapon(player)).substr(0, length1 - 7);
    int length2 = (ps.extra_weapon(player)).length();
    std::string text2 = (ps.extra_weapon(player)).substr(0, length2 - 7);

    screen.print(x, y, text1 + " level " + std::to_string(ps.main_weapon_level(player)), font);
    if (ps.extra_weapon(player) != "none")
        screen.print(x, y + 20, text2 + " count: " + std::to_string(ps.extra_weapon_count(player)), font);
}

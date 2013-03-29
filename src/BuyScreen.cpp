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

#include "BuyScreen.h"

#include <iostream>
#include <string>
#include <array>

#include "SDLc/Screen.h"
#include "SDLc/Font.h"
#include "SDLc/Misc.h"
#include "SDLc/Input.h"

#include "Options.h"
#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

BuyScreen::BuyScreen(Options& options, PlayerState& player_state, int current_level)
    : IGameState(ENV_BUYSCREEN),
      player_state_(player_state),
      current_level_(current_level),
      num_of_players_(options.num_of_players())
{
    for (int i = 0; i < NUM_OF_POSSIBLE_PLAYERS; ++i) 
        selectors_.pos.push_back(0);

    selectors_.gfx.load(options.data_path + "gfx/EnemyRammer.png");
    
    // This layout can really only handle 2 players
    if (num_of_players_ > 2)
        std::cout << "Warning: too many players" << std::endl;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void BuyScreen::run_logic(sdlc::Input& input, sdlc::Timer& timer, 
                          sdlc::Mixer& mixer, PlayerState& player_state)
{
    UNUSED(timer);
    UNUSED(mixer);

    // TODO: for now we do not read the keys from player_state.
    std::vector<KeySet> keys = { KeySet(1), KeySet(2) };

    for (size_t i = 0; i < keys.size(); i++) {
        if (player_state.energy_max(i + 1)) {

            // Move selector up/down
            if (input.key_pressed(keys.at(i).down, sdlc::NO_AUTOFIRE)) {
                selectors_.pos.at(i)++;
            } else if (input.key_pressed(keys.at(i).up, sdlc::NO_AUTOFIRE)) {
                selectors_.pos.at(i)--;
            }
            selectors_.pos.at(i) = sdlc::bound(selectors_.pos.at(i), 0, 2);

            if (input.key_pressed(keys.at(i).fire_main, sdlc::NO_AUTOFIRE)) {
                press_enter(i, player_state);
            }
        } else {
            player_done_.at(i) = true;
        }
    }

    // check if done
    int players_done = 0;
    for (int i = 0; i < num_of_players_; i++) {
        if (player_done_.at(i))
            players_done++;
    }
    if (players_done == num_of_players_)
        done_ = true;
}

void BuyScreen::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    auto lvl = std::to_string(current_level_ - 1);
    auto rocket_cost = std::to_string(ROCKET_COST);
    screen.print(200, 50, "level " + lvl + " complete!", font);
    screen.print(150, 200, "buy rockets (cost: " + rocket_cost + ")", font);
    screen.print(150, 220, "buy megavapen (not avail)", font);
    screen.print(150, 260, "goto next level", font);

    draw_player_info(1, screen, font);

    if (num_of_players_ > 1) 
        draw_player_info(2, screen, font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void BuyScreen::press_enter(int i, PlayerState& player_state)
{
    // Buy weapons
    if (selectors_.pos[i] == 0 && player_state.score(i + 1) >= ROCKET_COST) {
        if (player_state.extra_weapon(i + 1) == "Rocket Weapon") {
            player_state.set_extra_weapon_count(i + 1, player_state.extra_weapon_count(i + 1) + 5);
        } else {
            player_state.set_extra_weapon(i + 1, "Rocket Weapon");
            player_state.set_extra_weapon_count(i + 1, 10);
        }
        player_state.set_score(i + 1, player_state.score(i + 1) - ROCKET_COST);

    // Exit
    } else if (selectors_.pos[i] == 2) {
        player_done_[i] = true;
    }
}

void BuyScreen::draw_player_info(int i, sdlc::Screen& screen, sdlc::Font& font)
{
    screen.print(30 + (i-1)*340, 100, "player " + std::to_string(i), font);
    screen.print(60 + (i-1)*340, 120, "score: " + std::to_string(player_state_.score(i)), font);

    draw_extra_weapon(i, screen, font);

    int pos = selectors_.pos.at(i-1);
    if (selectors_.pos.at(i-1) == 2)
        pos++;

    if (!player_done_.at(i-1))
        screen.blit(50 + 500*(i-1), 200 + (20 * pos), selectors_.gfx);
}

void BuyScreen::draw_extra_weapon(int i, sdlc::Screen& screen, sdlc::Font& font)
{
    if (player_state_.extra_weapon(i) != "none") {
        int length = (player_state_.extra_weapon(i)).length();
        auto text = (player_state_.extra_weapon(i)).substr(0, length - 7);
        auto num_weapons = std::to_string(player_state_.extra_weapon_count(i));
        int x = 60 + 340*(i - 1);
        screen.print(x, 140, text + " count: " + num_weapons, font);
    }
}


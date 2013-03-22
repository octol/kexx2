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
      player_state_(&player_state),
      current_level_(current_level),
      how_many_players_(options.num_of_players())
{
    selectors_.gfx.load(options.data_path + "gfx/EnemyRammer.png");
    
    if (how_many_players_ > 2)
        std::cout << "Warning: >2 players not supported" << std::endl;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void BuyScreen::run_logic(sdlc::Input& input, sdlc::Timer& timer, 
                          sdlc::Mixer& mixer, PlayerState& player_state)
{
    UNUSED(timer);
    UNUSED(mixer);

    KeySet keys[2];
    keys[0] = player_state.keyset(1);
    keys[1] = player_state.keyset(2);

    // TODO: range based for loop
    for (int i = 0; i < 2; i++) {
        if (player_state.energy_max(i + 1)) {
            if (input.keyPressed(keys[i].down, sdlc::NO_AUTOFIRE))
                selectors_.pos[i]++;
            else if (input.keyPressed(keys[i].up, sdlc::NO_AUTOFIRE))
                selectors_.pos[i]--;
            selectors_.pos[i] = sdlc::bound(selectors_.pos[i], 0, 2);

            if (input.keyPressed(keys[i].fire_main, sdlc::NO_AUTOFIRE)) {
                if (selectors_.pos[i] == 0 && player_state.score(i + 1) >= ROCKET_COST) {
                    if (player_state.extra_weapon(i + 1) == "Rocket Weapon") {
                        player_state.set_extra_weapon_count(i + 1, player_state.extra_weapon_count(i + 1) + 10);
                    } else {
                        player_state.set_extra_weapon(i + 1, "Rocket Weapon");
                        player_state.set_extra_weapon_count(i + 1, 10);
                    }
                    player_state.set_score(i + 1, player_state.score(i + 1) - ROCKET_COST);
                } else if (selectors_.pos[i] == 2) {
                    player_done_[i] = true;
                }
            }
        } else {
            player_done_[i] = true;
        }
    }

    // check if done
    int players_done = 0;
    for (int i = 0; i < how_many_players_; i++) {
        if (player_done_[i])
            players_done++;
    }
    if (players_done == how_many_players_)
        done_ = true;
}

void BuyScreen::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    screen.print(200, 50, "level " 
            + std::to_string(current_level_ - 1) + " complete!", font);
    screen.print(30, 100, "player 1", font);
    screen.print(60, 120, "score: " 
            + std::to_string(player_state_->score(1)), font);

    if (player_state_->extra_weapon(1) != "none") {
        int length = (player_state_->extra_weapon(1)).length();
        std::string text 
            = (player_state_->extra_weapon(1)).substr(0, length - 7);

        screen.print(60, 140, text + " count: " 
                + std::to_string(player_state_->extra_weapon_count(1)), font);
    }

    if (how_many_players_ > 1) {
        screen.print(370, 100, "player 2", font);
        screen.print(400, 120, "score: " 
                + std::to_string(player_state_->score(2)), font);

        if (player_state_->extra_weapon(2) != "none") {
            int length = (player_state_->extra_weapon(2)).length();
            std::string text 
                = (player_state_->extra_weapon(2)).substr(0, length - 7);
            screen.print(400, 140, text + " count: " 
                    + std::to_string(player_state_->extra_weapon_count(2)), font);
        }
    }

    int pos1 = selectors_.pos[0];
    int pos2 = selectors_.pos[1];
    if (selectors_.pos[0] == 2)
        pos1++;
    if (selectors_.pos[1] == 2)
        pos2++;

    if (!player_done_[0])
        screen.blit(50, 200 + (20 * pos1), selectors_.gfx);
    if (!player_done_[1])
        screen.blit(550, 200 + (20 * pos2), selectors_.gfx);

    screen.print(150, 200, "buy rockets (cost: " 
            + std::to_string(ROCKET_COST) + ")", font);
    screen.print(150, 220, "buy megavapen (not avail)", font);
    screen.print(150, 260, "goto next level", font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

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

#include <iostream>
#include <string>
#include "SDLc/Screen.h"
#include "SDLc/Font.h"
#include "SDLc/Misc.h"
#include "SDLc/Input.h"
#include "BuyScreen.h"
#include "Options.h"
#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

BuyScreen::BuyScreen(Options& options, PlayerState& playerState, int currentLevel)
    : currentLevel_(currentLevel),
      howManyPlayers_(options.getHowManyPlayers()),
      playerState_(&playerState)
{
    env_type_ = ENV_BUYSCREEN;

    selectors_.gfx.load(options.dataPath + "gfx/EnemyRammer.png");
    selectors_.pos[0] = 0;
    selectors_.pos[1] = 0;

    playerdone_[0] = playerdone_[1] = false;
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void BuyScreen::runLogic(Timer& timer, PlayerState& playerState)
{
    extern Input* input;
    KeySet keys[2];
    keys[0] = playerState.getKeySet(1);
    keys[1] = playerState.getKeySet(2);

    int i;
    if (howManyPlayers_ > 2)
        std::cout << "BuyScreen::runLogic() more than 2 players not supported" 
                  << std::endl;
    for (i = 0; i < 2; i++) {
        if (playerState.getEnergyMax(i + 1)) {
            if (input->keyPressed(keys[i].down, NO_AUTOFIRE))
                selectors_.pos[i]++;
            else if (input->keyPressed(keys[i].up, NO_AUTOFIRE))
                selectors_.pos[i]--;
            selectors_.pos[i] = bound(selectors_.pos[i], 0, 2);

            if (input->keyPressed(keys[i].fireMain, NO_AUTOFIRE)) {
                if (selectors_.pos[i] == 0 && playerState.getScore(i + 1) >= ROCKET_COST) {
                    if (playerState.getExtraWeapon(i + 1) == "Rocket Weapon")
                        playerState.setExtraWeaponCount(i + 1, playerState.getExtraWeaponCount(i + 1) + 10);
                    else {
                        playerState.setExtraWeapon(i + 1, "Rocket Weapon");
                        playerState.setExtraWeaponCount(i + 1, 10);
                    }
                    playerState.setScore(i + 1, playerState.getScore(i + 1) - ROCKET_COST);
                } else if (selectors_.pos[i] == 2)
                    playerdone_[i] = true;
            }
        } else playerdone_[i] = true;
    }

    // check if done
    int tmpcount = 0;
    for (i = 0; i < howManyPlayers_; i++) {
        if (playerdone_[i])
            tmpcount++;
    }
    if (tmpcount == howManyPlayers_)
        done_ = true;
}

void BuyScreen::draw(Screen& screen, Font& mainFont)
{
    screen.print(200, 50, "level " + std::to_string(currentLevel_ - 1) + " complete!", mainFont);
    screen.print(30, 100, "player 1", mainFont);
    screen.print(60, 120, "score: " + std::to_string(playerState_->getScore(1)), mainFont);
    if (playerState_->getExtraWeapon(1) != "none") {
        int length = (playerState_->getExtraWeapon(1)).length();
        std::string text = (playerState_->getExtraWeapon(1)).substr(0, length - 7);

        screen.print(60, 140, text + " count: " + std::to_string(playerState_->getExtraWeaponCount(1)), mainFont);
    }

    if (howManyPlayers_ > 1) {
        screen.print(370, 100, "player 2", mainFont);
        screen.print(400, 120, "score: " + std::to_string(playerState_->getScore(2)), mainFont);
        if (playerState_->getExtraWeapon(2) != "none") {
            int length = (playerState_->getExtraWeapon(2)).length();
            std::string text = (playerState_->getExtraWeapon(2)).substr(0, length - 7);
            screen.print(400, 140, text + " count: " + std::to_string(playerState_->getExtraWeaponCount(2)), mainFont);
        }
    }

    int pos1 = selectors_.pos[0], pos2 = selectors_.pos[1];
    if (selectors_.pos[0] == 2)
        pos1++;
    if (selectors_.pos[1] == 2)
        pos2++;

    if (!playerdone_[0])
        screen.blit(50, 200 + (20 * pos1), selectors_.gfx);
    if (!playerdone_[1])
        screen.blit(550, 200 + (20 * pos2), selectors_.gfx);
    screen.print(150, 200, "buy rockets (cost: " + std::to_string(ROCKET_COST) + ")", mainFont);
    screen.print(150, 220, "buy megavapen (not avail)", mainFont);
    screen.print(150, 260, "goto next level", mainFont);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

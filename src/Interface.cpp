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
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Interface::Interface()
{
    //int j;
    //for(j = 0; j < NUM_OF_POSSIBLE_PLAYERS; j++)
    //energy[j] = energyMax[j] = score[j] = 0;
    numOfPlayers = 0;
}

Interface::~Interface()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Interface::update(int numOfPlayers_, PlayerState& playerState)
{
    numOfPlayers = numOfPlayers_;
    playerStateCopy = playerState;
    /*
    ObjectList::iterator i = objectManager.list.begin();
    for(; i != objectManager.list.end(); i++)
    {
        Object *obj = *i;
        if(obj->getType() == OBJ_PLAYER)
        {
            for(j = 1; j <= NUM_OF_POSSIBLE_PLAYERS; j++)
            {
                if(obj->name == "Player "+toString(j))
                {
                    energy[j-1] = obj->getEnergy();
                    energyMax[j-1] = obj->getEnergyMax();
                    score[j-1] = obj->getScore();
                }
            }
        }
    }
    */
}

void Interface::draw(Font& font, Screen& screen)
{
    // player 1
    int e = playerStateCopy.getEnergy(1);
    int eMax = playerStateCopy.getEnergyMax(1);
    int s = playerStateCopy.getScore(1);
    if (!e)
        screen.print(20, 440, "dead", font);
    else drawEnergy(20, 440, e, eMax, font, screen);
    drawScore(20, 20, s, font, screen);
    drawWeapons(20, 40, playerStateCopy, 1, font, screen);

    // player 2
    e = playerStateCopy.getEnergy(2);
    eMax = playerStateCopy.getEnergyMax(2);
    s = playerStateCopy.getScore(2);
    if (numOfPlayers >= 2) {
        if (!e)
            screen.print(560, 440, "dead", font);
        else drawEnergy(560, 440, e, eMax, font, screen);
        drawScore(400, 20, s, font, screen);
        drawWeapons(400, 40, playerStateCopy, 2, font, screen);
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

void Interface::drawEnergy(int x, int y, int value, int maxValue, Font& font, Screen& screen)
{
    int w = 20 * maxValue;
    int h = 20;
    int multi = (w / maxValue);

    screen.fillRect(x - 1, y - 1, w + 2, h + 2, 150, 150, 150);
    screen.fillRect(x, y, w, h, 0, 0, 0);
    if (value > 0)
        screen.fillRect(x + 1, y + 1, multi * (maxValue - (maxValue - value)) - 1, \
                        h - 1, 100, 100, 100);
}

void Interface::drawScore(int x, int y, int value, Font& font, Screen& screen)
{
    screen.print(x, y, "score: " + std::to_string(value), font);
}

void Interface::drawWeapons(int x, int y, PlayerState& ps, int player, Font& font, Screen& screen)
{
    int length1 = (ps.getMainWeapon(player)).length();
    string text1 = (ps.getMainWeapon(player)).substr(0, length1 - 7);
    int length2 = (ps.getExtraWeapon(player)).length();
    string text2 = (ps.getExtraWeapon(player)).substr(0, length2 - 7);

    screen.print(x, y, text1 + " level " + std::to_string(ps.getMainWeaponLevel(player)), font);
    if (ps.getExtraWeapon(player) != "none")
        screen.print(x, y + 20, text2 + " count: " + std::to_string(ps.getExtraWeaponCount(player)), font);
}

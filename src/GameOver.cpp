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

#include "SDLc/Screen.h"
#include "SDLc/Font.h"
#include "SDLc/Misc.h"
#include "SDLc/Input.h"
#include "Options.h"
#include "PlayerState.h"
#include "GameOver.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

GameOver::GameOver()
{
    setType(ENV_GAMEOVER);
}

GameOver::~GameOver()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void GameOver::runLogic(Timer& timer, PlayerState& playerState)
{
    UNUSED(timer);
    UNUSED(playerState);

    extern Input* input;
    if (input->keyPressed(SDLK_RETURN, NO_AUTOFIRE))
        done(true);
}

void GameOver::draw(Screen& screen, Font& mainFont)
{
    screen.print(200, 180, "game over", mainFont);
    screen.print(200, 200, "press enter...", mainFont);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

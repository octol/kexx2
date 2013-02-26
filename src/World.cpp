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

#include "World.h"
#include "PlayerState.h"
#include "SDLc/Timer.h"
#include "SDLc/Screen.h"
#include "SDLc/Misc.h"
#include "SDLc/Input.h"
#include "Ship.h"
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

World::World(Options& options, PlayerState& player_state, int level)
    : IGameState(ENV_WORLD),
      numOfPlayers(options.num_of_players()), 
      currentLevel(level),
      timeWhenEnteringLevel(SDL_GetTicks()),
      flashingtextTimer(SDL_GetTicks())
{
    // setup data
    objectManager.loadData(options.data_path);

    // ship(s)
    objectManager.createShips(player_state);

    // level
    levelManager.loadLevel(options.data_path, level, objectManager);

    // effects
    fxManager.load(particleManager, options.data_path);

    // bgmusic
    bgmusic.load(options.data_path + "music/bgmusic1.xm");
    bgmusic.play(-1);

    // sounds
    levelcompleteSnd.load(options.data_path + "soundfx/levelcomplete.wav");
    gameoverSnd.load(options.data_path + "soundfx/die.wav");
    enteringlevelSnd.load(options.data_path + "soundfx/newlevel.wav");
    enteringlevelSnd.play(0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void World::run_logic(Input& input, Timer& timer, Mixer& mixer,
                      PlayerState& player_state)
{
    // update scrolling position
    worldYPos = worldYPos - SCROLLING_SPEED * timer.frame_time();

    // rest
    starfield.update(timer);
    objectManager.update(timer, fxManager, worldYPos, player_state);
    particleManager.update(timer);
    fxManager.update(timer);
    interface.update(numOfPlayers, player_state);

    // TODO: remove the calls to SDL_GetTicks()
    
    // when entering level
    if (timeWhenEnteringLevel) {
        if (SDL_GetTicks() - timeWhenEnteringLevel > 3000) {
            timeWhenEnteringLevel = 0;
            flashingtextTimer = 0;
        } else if (SDL_GetTicks() - flashingtextTimer > 400)
            flashingtextTimer = SDL_GetTicks();
    }

    // when level complete
    else if (objectManager.getHowManyEnemies() == 0 && !allplayersdead) {
        if (timeWhenAllEnemiesDead == 0) {
            levelcompleteSnd.play(0);

            timeWhenAllEnemiesDead = SDL_GetTicks();
            flashingtextTimer = SDL_GetTicks();
        }
        if (SDL_GetTicks() - timeWhenAllEnemiesDead > 3500)
            done_ = true;

        // flashing text
        if (flashingtextTimer == 0)
            flashingtextTimer = SDL_GetTicks();
        else if (SDL_GetTicks() - flashingtextTimer > 400)
            flashingtextTimer = 0;
        // stop flashing
        if (SDL_GetTicks() - timeWhenAllEnemiesDead > 2000)
            flashingtextTimer = 0;
    }

    // when all players are dead
    else if (objectManager.getHowManyPlayersAlive() == 0) {
        allplayersdead = true;
        if (timeWhenAllEnemiesDead == 0) {
            gameoverSnd.play(0);
            timeWhenAllEnemiesDead = SDL_GetTicks();
            flashingtextTimer = SDL_GetTicks();
        }
        if (SDL_GetTicks() - timeWhenAllEnemiesDead > 4000)
            done_ = true;

        // flashing text
        if (flashingtextTimer == 0)
            flashingtextTimer = SDL_GetTicks();
        else if (SDL_GetTicks() - flashingtextTimer > 400)
            flashingtextTimer = 0;
    }
}

void World::draw(Screen& screen, Font& mainFont)
{
    starfield.draw(screen);
    objectManager.draw(screen);
    particleManager.draw(screen);
    fxManager.draw(screen);
    interface.draw(mainFont, screen);

    if (timeWhenEnteringLevel)
        if (SDL_GetTicks() - timeWhenEnteringLevel < 1000 || SDL_GetTicks() - flashingtextTimer < 200) {
            screen.print(200, 200, "entering level " + std::to_string(currentLevel), mainFont);
            screen.print(200, 235, "ctrl for blaster", mainFont);
            screen.print(200, 255, "shift for rockets", mainFont);
        }

    if (objectManager.getHowManyEnemies() == 0 && !allplayersdead)
        if (SDL_GetTicks() - flashingtextTimer < 200 || flashingtextTimer == 0)
            screen.print(200, 200, "level complete!", mainFont);

    if (allplayersdead && SDL_GetTicks() - flashingtextTimer < 200)
        screen.print(200, 200, "inte stabilt!", mainFont);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

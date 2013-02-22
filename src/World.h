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

#ifndef KEXX2_WORLD_H
#define KEXX2_WORLD_H

#include "IGameState.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "ParticleManager.h"
#include "FxManager.h"
#include "Starfield.h"
#include "Interface.h"
#include "SDLc/Music.h"

class Options;
class PlayerState;

struct WorldData {
};

class World : public IGameState {
public:
    World(Options& options, PlayerState& player_state, int level);
    virtual ~World() {};

    void run_logic(Input& input, Timer& timer, 
                   PlayerState& player_state) override;
    void draw(Screen& screen, Font& font);

private:
    ObjectManager objectManager;
    LevelManager levelManager;
    ParticleManager particleManager;
    FxManager fxManager;
    Starfield starfield;
    WorldData worldData;
    Interface interface;

    float worldYPos = 0; // how much the screen has scrolled
    int numOfPlayers;
    int currentLevel = 0;
    Music bgmusic;
    Sound levelcompleteSnd;
    Sound enteringlevelSnd;
    Sound gameoverSnd;

    // minor stuff
    int timeWhenEnteringLevel;
    int timeWhenAllEnemiesDead = 0;
    int flashingtextTimer = 0;
    bool allplayersdead = false;
};

#endif // KEXX2_WORLD_H

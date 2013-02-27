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

// TODO: moved fields to this struct.
struct WorldData {
};

class World : public IGameState {
public:
    World(Timer& timer, Options& options,
          PlayerState& player_state, int level);
    virtual ~World() {};

    void run_logic(Input& input, Timer& timer, Mixer& mixer,
                   PlayerState& player_state) override;
    void draw(Screen& screen, Font& font);

private:
    ObjectManager object_manager_;
    LevelManager level_manager_;
    ParticleManager particle_manager_;
    FxManager fx_manager_;
    Starfield starfield_;
    WorldData world_data_;
    Interface interface_;

    float world_y_pos_ = 0; // how much the screen has scrolled
    int num_of_players_ = 1;
    int current_level_ = 0;
    Music bg_music_;
    Sound level_complete_snd_;
    Sound entering_level_snd_;
    Sound game_over_snd_;

    // minor stuff
    int time_when_entering_level_ = 0;
    int time_when_all_enemies_dead_ = 0;
    int flashing_text_timer_ = 0;
    bool all_players_dead_ = false;
};

#endif // KEXX2_WORLD_H

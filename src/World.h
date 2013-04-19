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

#include "SDLc.h"
#include "IGameState.h"
#include "ObjectManager.h"
#include "LevelManager.h"
#include "ParticleManager.h"
#include "FxManager.h"
#include "Starfield.h"
#include "Interface.h"

class Options;
class PlayerState;

const int TEXT_PERIOD_TIME = 400;
const int SHOW_TEXT = TEXT_PERIOD_TIME;

// -----------------------------------------------------------------------------
// Keeps track of the meta state of the World class.
// -----------------------------------------------------------------------------

class State {
public:
    enum Transition { ENTERING, IN_GAME, COMPLETE, DEAD };

    State() = default;
    State(State::Transition current, sdlc::Timer& timer);
    bool operator==(const State& rhs) const;
    bool operator==(const State::Transition& rhs) const;

    State::Transition transition_to(Transition new_state);
    State::Transition current() const;
    int time_since_transition() const;

private:
    sdlc::Timer* timer_;
    Transition current_ = ENTERING;
    int transition_time_ = 0; 
}; 

// -----------------------------------------------------------------------------
// The main game object, containing most of the game play.
// -----------------------------------------------------------------------------

class World final : public IGameState {
public:
    World(sdlc::Mixer&, sdlc::Timer&, Options&, PlayerState&, int level);

    void run_logic(sdlc::Input&, sdlc::Timer&, sdlc::Mixer&, PlayerState&) override;
    void draw(sdlc::Screen&, sdlc::Font&) override;

private:
    State transition_state(State state);

    ObjectManager object_manager_;
    LevelManager level_manager_;
    ParticleManager particle_manager_;
    FxManager fx_manager_;
    Starfield starfield_;
    Interface interface_;

    int num_of_players_ = 1;
    int current_level_ = 0;

    // Keeping track of the amount the screen has scrolled.
    float world_y_pos_ = 0;

    sdlc::Music bg_music_;
    sdlc::Sound level_complete_snd_;
    sdlc::Sound entering_level_snd_;
    //sdlc::Sound game_over_snd_;

    State state_;
    int show_text_ = 0;
};

#endif // KEXX2_WORLD_H

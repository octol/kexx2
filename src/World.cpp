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
#include "Ship.h"
#include "WeaponBlaster.h"
#include "WeaponRocket.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

World::World(sdlc::Mixer& mixer, sdlc::Timer& timer, Options& options, 
             PlayerState& player_state, int level)
    : IGameState(ENV_WORLD),
      num_of_players_(options.num_of_players()),
      current_level_(level),
      bg_music_(options.data_path + "music/bgmusic1.xm"),
      //game_over_snd_(options.data_path + "soundfx/die.wav"),
      level_complete_snd_(options.data_path + "soundfx/levelcomplete.wav"),
      entering_level_snd_(options.data_path + "soundfx/newlevel.wav"),
      state_(State::ENTERING, timer)
{
#ifdef DEBUG_LOG
    std::cerr << "World::World()" << std::endl;
#endif

    // setup data
    object_manager_.load_data(options.data_path);

    // ship(s)
    object_manager_.create_ships(player_state);

    // level
    level_manager_.load_level(options.data_path, level, object_manager_);

    // effects
    fx_manager_.load(particle_manager_, options.data_path);

    // bg_music_
    mixer.set_music_volume(mixer.music_volume());
    bg_music_.play(-1);

    // sounds
    entering_level_snd_.play(0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void World::run_logic(sdlc::Input& input, sdlc::Timer& timer,
                      sdlc::Mixer& mixer, PlayerState& player_state)
{
    UNUSED(input);
    UNUSED(mixer);

    // Update scrolling position
    world_y_pos_ += - SCROLLING_SPEED * timer.frame_time();

    // Update the different components.
    starfield_.update(timer);
    object_manager_.update(timer, fx_manager_, world_y_pos_, player_state);
    particle_manager_.update(timer);
    fx_manager_.update(timer);
    interface_.update(num_of_players_, player_state);

    state_ = transition_state(state_);

    // Run state dependent logic
    if (state_ == State::ENTERING) {
        show_text_ = SHOW_TEXT;  
    } else if (state_ == State::IN_GAME) {
        if (state_.time_since_transition() < 2000) {
            show_text_ = state_.time_since_transition() % TEXT_PERIOD_TIME; 
        } else { 
            show_text_ = 0; 
        }
    } else if (state_ == State::COMPLETE) {
        show_text_ = state_.time_since_transition() % TEXT_PERIOD_TIME; 
        if (state_.time_since_transition() > 3500)
           done_ = true;
        else if (state_.time_since_transition() > 2000)
            show_text_ = SHOW_TEXT;
    } else if (state_ == State::DEAD) {
        show_text_ = state_.time_since_transition() % TEXT_PERIOD_TIME; 
        if (state_.time_since_transition() > 4000)
           done_ = true;
    }
}

void World::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    starfield_.draw(screen);
    object_manager_.draw(screen);
    particle_manager_.draw(screen);
    fx_manager_.draw(screen);
    interface_.draw(font, screen);

    // Half of the time we show the text
    bool show_text = show_text_ > TEXT_PERIOD_TIME / 2;

    std::string lvl = std::to_string(current_level_);

    if ((state_ == State::ENTERING || state_ == State::IN_GAME) && show_text) {
        screen.print(200, 200, "entering level " + lvl, font);
        screen.print(200, 235, "ctrl for blaster", font);
        screen.print(200, 255, "shift for rockets", font);
    } else if (state_ == State::COMPLETE && show_text) {
        screen.print(220, 200, "level complete!", font);
    } else if (state_ == State::DEAD && show_text) {
        screen.print(250, 200, "terminated!", font);
    }
}
// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

State World::transition_state(State state)
{
    // Cache state
    bool all_players_dead = object_manager_.num_of_players_alive() == 0;

    // Transition state
    if (state == State::ENTERING) {
        if (state.time_since_transition() > 1000) {
            state.transition_to(State::IN_GAME);
        }
    } else if (state == State::IN_GAME) {
        if (object_manager_.num_of_enemies() == 0 && !all_players_dead) {
            state.transition_to(State::COMPLETE);
            level_complete_snd_.play(0);
        } else if (all_players_dead) {
            state.transition_to(State::DEAD);
            //game_over_snd_.play(0);
        }
    }

    return state;
}

// -----------------------------------------------------------------------------
// State class
// -----------------------------------------------------------------------------

State::State(Transition current, sdlc::Timer& timer)
    : timer_(&timer), current_(current), transition_time_(timer.ticks())
{
}

bool State::operator==(const State& rhs) const
{
    return current() == rhs.current();
}

bool State::operator==(const State::Transition& rhs) const
{
    return current() == rhs;
}

State::Transition State::transition_to(Transition new_state)
{
    current_ = new_state;
    transition_time_ = timer_->ticks();
    return current_;
}

State::Transition State::current() const
{
    return current_;
}

int State::time_since_transition() const
{
    return timer_->ticks() - transition_time_;
}


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

World::World(sdlc::Timer& timer, Options& options, PlayerState& player_state, 
             int level)
    : IGameState(ENV_WORLD),
      num_of_players_(options.num_of_players()), 
      current_level_(level),
      time_when_entering_level_(timer.ticks()),
      flashing_text_timer_(timer.ticks())
{
    // setup data
    object_manager_.loadData(options.data_path);

    // ship(s)
    object_manager_.createShips(player_state);

    // level
    level_manager_.loadLevel(options.data_path, level, object_manager_);

    // effects
    fx_manager_.load(particle_manager_, options.data_path);

    // bg_music_
    bg_music_.load(options.data_path + "music/bgmusic1.xm");
    bg_music_.play(-1);

    // sounds
    level_complete_snd_.load(options.data_path + "soundfx/levelcomplete.wav");
    game_over_snd_.load(options.data_path + "soundfx/die.wav");
    entering_level_snd_.load(options.data_path + "soundfx/newlevel.wav");
    entering_level_snd_.play(0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void World::run_logic(sdlc::Input& input, sdlc::Timer& timer, sdlc::Mixer& mixer,
                      PlayerState& player_state)
{
    UNUSED(input);
    UNUSED(mixer);

    // update scrolling position
    world_y_pos_ += - SCROLLING_SPEED * timer.frame_time();

    // rest
    starfield_.update(timer);
    object_manager_.update(timer, fx_manager_, world_y_pos_, player_state);
    particle_manager_.update(timer);
    fx_manager_.update(timer);
    interface_.update(num_of_players_, player_state);

    // when entering level
    if (time_when_entering_level_) {
        if (timer.ticks() - time_when_entering_level_ > 3000) {
            time_when_entering_level_ = 0;
            flashing_text_timer_ = 0;
        } else if (timer.ticks() - flashing_text_timer_ > 400)
            flashing_text_timer_ = timer.ticks();
    }

    // when level complete
    else if (object_manager_.getHowManyEnemies() == 0 && !all_players_dead_) {
        if (time_when_all_enemies_dead_ == 0) {
            level_complete_snd_.play(0);

            time_when_all_enemies_dead_ = timer.ticks();
            flashing_text_timer_ = timer.ticks();
        }
        if (timer.ticks() - time_when_all_enemies_dead_ > 3500)
            done_ = true;

        // flashing text
        if (flashing_text_timer_ == 0)
            flashing_text_timer_ = timer.ticks();
        else if (timer.ticks() - flashing_text_timer_ > 400)
            flashing_text_timer_ = 0;
        // stop flashing
        if (timer.ticks() - time_when_all_enemies_dead_ > 2000)
            flashing_text_timer_ = 0;
    }

    // when all players are dead
    else if (object_manager_.getHowManyPlayersAlive() == 0) {
        all_players_dead_ = true;
        if (time_when_all_enemies_dead_ == 0) {
            game_over_snd_.play(0);
            time_when_all_enemies_dead_ = timer.ticks();
            flashing_text_timer_ = timer.ticks();
        }
        if (timer.ticks() - time_when_all_enemies_dead_ > 4000)
            done_ = true;

        // flashing text
        if (flashing_text_timer_ == 0)
            flashing_text_timer_ = timer.ticks();
        else if (timer.ticks() - flashing_text_timer_ > 400)
            flashing_text_timer_ = 0;
    }
}

void World::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    starfield_.draw(screen);
    object_manager_.draw(screen);
    particle_manager_.draw(screen);
    fx_manager_.draw(screen);
    interface_.draw(font, screen);

    // TODO: move ticks logic to run_logic()
    if (time_when_entering_level_) {
        if (SDL_GetTicks() - time_when_entering_level_ < 1000 
                || SDL_GetTicks() - flashing_text_timer_ < 200) {
            screen.print(200, 200, "entering level " + std::to_string(current_level_), font);
            screen.print(200, 235, "ctrl for blaster", font);
            screen.print(200, 255, "shift for rockets", font);
        }
    }

    if (object_manager_.getHowManyEnemies() == 0 && !all_players_dead_) {
        if (SDL_GetTicks() - flashing_text_timer_ < 200 
                || flashing_text_timer_ == 0) {
            screen.print(250, 200, "level complete!", font);
        }
    }

    if (all_players_dead_ && SDL_GetTicks() - flashing_text_timer_ < 200)
        screen.print(250, 200, "terminated!", font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

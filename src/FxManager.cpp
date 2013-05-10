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

#include "FxManager.h"

#include <cmath>
#include <iostream>
#include <algorithm>

#include "Defines.h"
#include "ParticleManager.h"
#include "Object.h"

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void FxManager::load(ParticleManager& particle_manager, std::string data_path)
{
    particle_manager_ = &particle_manager;

    expl_snd_small_.load(data_path + "soundfx/explode2.wav");
    expl_snd_big_.load(data_path + "soundfx/explode3.wav");
    expl_img_.load(data_path + "gfx/Explosion1.png");
    expl_snd_player_.load(data_path + "soundfx/playerhit.wav");
    alarm_snd_.load(data_path + "soundfx/alarm.wav");

    expl_snd_small_.set_channel((int)SoundChannel::expl_small);
    expl_snd_big_.set_channel((int)SoundChannel::expl_big);
    expl_snd_player_.set_channel((int)SoundChannel::expl_player);
    alarm_snd_.set_channel((int)SoundChannel::alarm);

    // pre-calculate explosion data for speed
    for (auto& expl : precalc_norm_expl_) {
        for (auto& particle_data : expl) {
            int intensity = (rand() % 256);
            int degree = (rand() % 360);

            float x_vel = (float)(cos((double)degree) * intensity);
            float y_vel = (float)(sin((double)degree) * intensity);

            particle_data.intensity = intensity;
            particle_data.x_vel = x_vel;
            particle_data.y_vel = y_vel;
        }
    }
}

void FxManager::update(sdlc::Timer& timer)
{
    for (auto& sprite : explosion_list_)
        sprite->update(timer);

    explosion_list_.erase(std::remove_if(begin(explosion_list_), end(explosion_list_), 
        [](std::unique_ptr<sdlc::Sprite>& s) {
            return !s->animation_active();
        }), end(explosion_list_));
}

void FxManager::draw(sdlc::Screen& screen)
{
    for (auto& sprite : explosion_list_)
        screen.blit(*sprite);
}

void FxManager::explode_normal(int x, int y)
{
    const int PARTICLES = 128;

    expl_snd_big_.play(0);

    std::unique_ptr<sdlc::Sprite> sprite(new sdlc::Sprite(expl_img_));
    //sprite->link(expl_img_.data);
    sprite->init_animation(5, 11, 1);
    sprite->set_x((float)x - (float)sprite->width() / 2.0);
    sprite->set_y((float)y - (float)sprite->height() / 2.0);
    explosion_list_.push_back(std::move(sprite));

    // choose one of the precomputed particles.
    int i_expl = rand() % 10;

    for (int i = 0; i < PARTICLES; ++i) {
        int intensity = precalc_norm_expl_[i_expl][i].intensity;
        float x_vel = precalc_norm_expl_[i_expl][i].x_vel;
        float y_vel = precalc_norm_expl_[i_expl][i].y_vel;
        particle_manager_->create(x, y, x_vel, y_vel, 
                                  255, 255, 255, intensity, 100.0f);
    }
}

void FxManager::explode_tiny(int x, int y)
{
    explode_tiny(x, y, 0, 0);
}

void FxManager::explode_tiny(int x, int y, float vel, float angle)
{
    const int PARTICLES = 32;

    play_hit_snd();

    angle = angle * (3.1415927f / 180.0f);
    float x_vel_mod = (float)cos(angle) * vel;
    float y_vel_mod = (float)-sin(angle) * vel;

    for (int i = 0; i < PARTICLES; i++) {
        int intensity = (rand() % 256);
        int degree = (rand() % 360);
        int speed = intensity / 4;

        float x_vel = (float)(cos((double)degree) * speed) + x_vel_mod;
        float y_vel = (float)(sin((double)degree) * speed) + y_vel_mod;

        particle_manager_->create(x, y, x_vel, y_vel, 255, 255, 255, 
                                  intensity, 100.0f);
    }
}

void FxManager::smokepuff(int x, int y)
{
    // TODO: remove extern timer.
    extern sdlc::Timer* timer;

    int amount = (int)((float)timer->frame_time() / 0.004f);

    for (int i = 0; i < amount; i++) {
        float xVel = (float)((rand() % 50) - 25);
        float yVel = (float)((rand() % 50) - 25);
        particle_manager_->create((float)(x + (rand() % 10) - 5), 
                                  (float)(y + (rand() % 10) - 5), 
                                  xVel, yVel, 255, 255, 255, 100, 30.0f);
    }
}

void FxManager::play_hit_snd()
{
    expl_snd_small_.play(0);
}

void FxManager::play_player_hit_snd()
{
    expl_snd_player_.play(0);
    alarm_snd_.play(0);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

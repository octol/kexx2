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

#ifndef KEXX2_FXMANAGER_H
#define KEXX2_FXMANAGER_H

#include "SDLc.h"
#include <list>

class ParticleManager;

class FxManager {
public:
    ~FxManager();

    void load(ParticleManager& particle_manager, std::string data_path);
    void update(sdlc::Timer& timer);
    void draw(sdlc::Screen& screen);

    void explode_normal(int x, int y);
    void explode_tiny(int x, int y);
    void explode_tiny(int x, int y, float vel, float angle);
    void smokepuff(int x, int y);

    void play_hit_snd();
    void play_player_hit_snd();

private:
    // TODO: change to smart pointer
    ParticleManager* particle_manager_ = nullptr;
    sdlc::Sound expl_snd_small_;
    sdlc::Sound expl_snd_big_;
    sdlc::Sound expl_snd_player_;
    sdlc::Sound alarm_snd_;
    sdlc::Surface expl_img_;
    std::list<sdlc::Sprite*> explosion_list_;

    // TODO: replace with list?
    struct precalculated_data {
        int intensity;
        float x_vel;
        float y_vel;
    } precalc_norm_expl_[10][256];
};

#endif

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

#ifndef KEXX2_PARTICLEMANAGER_H
#define KEXX2_PARTICLEMANAGER_H

#include "SDLc.h"
#include <vector>

class ParticleManager {
public:
    virtual ~ParticleManager() {};

    void create(float x, float y, float x_vel, float y_vel, 
                int r, int g, int b, int a, float fade_speed);
    virtual void update(sdlc::Timer& timer);
    virtual void draw(sdlc::Screen& screen);
    uint64_t num_of_particles_active();

protected:
    std::vector<sdlc::Particle> particles_;
};

#endif

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

#include "ParticleManager.h"
#include <cassert>
#include <iostream>
#include <algorithm>
#include "Defines.h"

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ParticleManager::create(float x, float y, float x_vel, float y_vel,
                             int r, int g, int b, int a, float fade_speed)
{
    sdlc::Particle particle(x, y, x_vel, y_vel, r, g, b, (float)a);
    particle.set_fade_speed(fade_speed);
    particle.active(true);
    particles_.push_back(particle);
}

void ParticleManager::update(sdlc::Timer& timer)
{
    for (auto & p : particles_)
        p.update(timer);

    particles_.erase(remove_if(begin(particles_), end(particles_),
    [](sdlc::Particle p) {
        if (!p.inside(1, SCREEN_WIDTH - 2, 1, SCREEN_HEIGHT - 2) 
            || p.alpha() <= 50) {
            p.active(false);
        }
        return !p.active();
    }), end(particles_));
}

void ParticleManager::draw(sdlc::Screen& screen)
{
    auto draw_pix = [&screen](sdlc::Particle p) {
        auto a = (uint8_t)p.alpha();
        auto r = (uint8_t)p.r();
        auto b = (uint8_t)p.b();
        auto g = (uint8_t)p.g();
        screen.fast_blend_pix((int)p.x(), (int)p.y(), r, b, g, a);
        screen.fast_blend_pix((int)p.x() + 1, (int)p.y(), r, b, g, a);
        screen.fast_blend_pix((int)p.x(), (int)p.y() + 1, r, b, g, a);
        screen.fast_blend_pix((int)p.x() + 1, (int)p.y() + 1, r, b, g, a);
    };

    screen.lock();
    std::for_each(begin(particles_), end(particles_), draw_pix);
    screen.unlock();
}

uint64_t ParticleManager::num_of_particles_active()
{
    auto num = std::accumulate(begin(particles_), end(particles_), (uint64_t)0,
        [](uint64_t n, sdlc::Particle p) {
            return p.active() ? n + 1 : n;
        });

    return num;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------


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

#ifndef _KEXX2_PARTICLEMANAGER_H_
#define _KEXX2_PARTICLEMANAGER_H_

#include "SDLc.h"

const int NUM_OF_PARTICLES = 4096;

class ParticleManager {
public:
    ParticleManager();
    virtual ~ParticleManager();

    void create(float x, float y, float xVel, float yVel, int r, int g, int b, int a, float fadeSpeed);
    virtual void update(sdlc::Timer& timer);
    void draw(sdlc::Screen& screen);
    int numOfParticlesActive();

protected:
    sdlc::Particle particle[NUM_OF_PARTICLES];
    int nextAvailableSlot();
};

#endif

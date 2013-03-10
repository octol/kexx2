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
#include <iostream>

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ParticleManager::create(float x, float y, float xVel, float yVel, 
                             int r, int g, int b, int a, float fade_speed)
{
    int slot = next_available_slot();

    particle_[slot].setX(x);
    particle_[slot].setY(y);
    particle_[slot].setXVel(xVel);
    particle_[slot].setYVel(yVel);
    particle_[slot].setR(r);
    particle_[slot].setG(g);
    particle_[slot].setB(b);
    particle_[slot].setAlpha(a);
    particle_[slot].setFadeSpeed(fade_speed);
    particle_[slot].active(true);
}

void ParticleManager::update(sdlc::Timer& timer)
{
    // TODO: range based for loop
    for (int i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle_[i].active()) {
            particle_[i].update(timer);
            //cout << "x: " << particle_[i].getX();
            //cout << "\ty: " << particle_[i].getY() << endl;
            if (particle_[i].getAlpha() <= 50 || 
                    particle_[i].getY() < 1 || particle_[i].getY() > 478 || 
                    particle_[i].getX() < 1 || particle_[i].getX() > 638) {
                particle_[i].active(false);
            }
        }
    }
}

void ParticleManager::draw(sdlc::Screen& screen)
{
    screen.lock();
    // TODO: range based for loop
    for (int i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle_[i].active()) {
            screen.fastBlendPix((int)particle_[i].getX(), (int)particle_[i].getY(), 
                                particle_[i].getR(), particle_[i].getG(), 
                                particle_[i].getB(), (int)particle_[i].getAlpha());
            screen.fastBlendPix((int)(particle_[i].getX() + 1), (int)particle_[i].getY(), 
                                particle_[i].getR(), particle_[i].getG(), 
                                particle_[i].getB(), (int)particle_[i].getAlpha());
            screen.fastBlendPix((int)particle_[i].getX(), (int)(particle_[i].getY() + 1), 
                                particle_[i].getR(), particle_[i].getG(), 
                                particle_[i].getB(), (int)particle_[i].getAlpha());
            screen.fastBlendPix((int)particle_[i].getX() + 1, (int)(particle_[i].getY() + 1), 
                                particle_[i].getR(), particle_[i].getG(), 
                                particle_[i].getB(), (int)particle_[i].getAlpha());
        }
    }
    screen.unlock();
}

int ParticleManager::num_of_particles_active()
{
    int counter = 0;
    // TODO: range based for loop
    for (int i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle_[i].active())
            counter++;
    }
    return counter;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

int ParticleManager::next_available_slot()
{
    int i = 0;
    while (particle_[i].active()) {
        i++;
        if (i >= NUM_OF_PARTICLES) {
            std::cout << "ParticleManager::nextAvailableSlot() ";
            std::cout << "Particleslot overflow!" << std::endl;
            i = 0;
            break;
        }
    }
    return i;
}

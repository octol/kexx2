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
#include "SDLc/Screen.h"
#include <iostream>
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

ParticleManager::ParticleManager()
{
}

ParticleManager::~ParticleManager()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void ParticleManager::create(float x, float y, float xVel, float yVel, \
                             int r, int g, int b, int a, float fadeSpeed)
{
    int slot = nextAvailableSlot();

    particle[slot].setX(x);
    particle[slot].setY(y);
    particle[slot].setXVel(xVel);
    particle[slot].setYVel(yVel);
    particle[slot].setR(r);
    particle[slot].setG(g);
    particle[slot].setB(b);
    particle[slot].setAlpha(a);
    particle[slot].setFadeSpeed(fadeSpeed);
    particle[slot].active(true);
}

void ParticleManager::update(Timer& timer)
{
    //cout << "before: " << numOfParticlesActive();
    int i;
    for (i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle[i].active()) {
            particle[i].update(timer);
            //cout << "x: " << particle[i].getX();
            //cout << "\ty: " << particle[i].getY() << endl;
            if (particle[i].getAlpha() <= 50 || \
                    particle[i].getY() < 1 || particle[i].getY() > 478 || \
                    particle[i].getX() < 1 || particle[i].getX() > 638) {
                particle[i].active(false);
            }
        }
    }
    //cout << "\tafter: " << numOfParticlesActive() << endl;
}

void ParticleManager::draw(Screen& screen)
{
    screen.lock();
    int i;
    for (i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle[i].active()) {
            screen.fastBlendPix((int)particle[i].getX(), (int)particle[i].getY(), \
                                particle[i].getR(), particle[i].getG(), \
                                particle[i].getB(), (int)particle[i].getAlpha());
            screen.fastBlendPix((int)(particle[i].getX() + 1), (int)particle[i].getY(), \
                                particle[i].getR(), particle[i].getG(), \
                                particle[i].getB(), (int)particle[i].getAlpha());
            screen.fastBlendPix((int)particle[i].getX(), (int)(particle[i].getY() + 1), \
                                particle[i].getR(), particle[i].getG(), \
                                particle[i].getB(), (int)particle[i].getAlpha());
            screen.fastBlendPix((int)particle[i].getX() + 1, (int)(particle[i].getY() + 1), \
                                particle[i].getR(), particle[i].getG(), \
                                particle[i].getB(), (int)particle[i].getAlpha());
        }
    }
    screen.unlock();
}

int ParticleManager::numOfParticlesActive()
{
    int counter = 0, i;
    for (i = 0; i < NUM_OF_PARTICLES; i++) {
        if (particle[i].active())
            counter++;
    }
    return counter;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

int ParticleManager::nextAvailableSlot()
{
    int i = 0;
    while (particle[i].active()) {
        i++;
        if (i >= NUM_OF_PARTICLES) {
            cout << "ParticleManager::nextAvailableSlot() ";
            cout << "Particleslot overflow!" << endl;
            i = 0;
            break;
        }
    }
    return i;
}

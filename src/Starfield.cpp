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

#include "Starfield.h"
#include <cstdlib>
#include "SDLc/Screen.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Starfield::Starfield()
{
    // set intensity of stars
    int intensity_levels = 140;
    int stars_per_intensity_level = (NUM_OF_STARS / intensity_levels);
    int current_star = 0;
    int tmp_intensity = 0;

    while (current_star < NUM_OF_STARS) {
        for (int i = 0; i < stars_per_intensity_level; i++) {
            particle_[current_star].setR(tmp_intensity);
            particle_[current_star].setG(tmp_intensity);
            particle_[current_star].setB(tmp_intensity);
            current_star++;
        }
        tmp_intensity++;
    }

    // rand out positions
    for (int i = 0; i < NUM_OF_STARS; i++) {
        particle_[i].setX((rand() % 638) + 1);
        particle_[i].setY((rand() % 478) + 1);
        float tmp = particle_[i].getR()/**0.1*/;
        particle_[i].setYVel((tmp * tmp * tmp) / 200000.0f);
    }
}

Starfield::~Starfield()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Starfield::update(sdlc::Timer& timer)
{
    for (int i = 0; i < NUM_OF_STARS; i++) {
        particle_[i].update(timer);
        if (particle_[i].getY() > 478)
            particle_[i].setY(1);
    }
}

void Starfield::draw(sdlc::Screen& screen)
{
    screen.lock();
    for (int i = 0; i < NUM_OF_STARS; i++) {
        screen.fastSetPix((int)(particle_[i].getX()), (int)(particle_[i].getY()), 
                          particle_[i].getR(), particle_[i].getG(), particle_[i].getB());
//      if(particle_[i].r > 100)
//      {
        screen.fastSetPix((int)(particle_[i].getX() + 1), (int)(particle_[i].getY()),
                          particle_[i].getR(), particle_[i].getG(), particle_[i].getB());
        screen.fastSetPix((int)(particle_[i].getX()), (int)(particle_[i].getY() + 1),
                          particle_[i].getR(), particle_[i].getG(), particle_[i].getB());
        screen.fastSetPix((int)(particle_[i].getX() + 1), (int)(particle_[i].getY() + 1),
                          particle_[i].getR(), particle_[i].getG(), particle_[i].getB());
//      }
    }
    screen.unlock();
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

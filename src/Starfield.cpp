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
    int intensityLevels = 140;
    int starsPerIntensityLevel = (NUM_OF_STARS / intensityLevels);
    int currentStar = 0;
    int tmpIntensity = 0;

    int i;
    while (currentStar < NUM_OF_STARS) {
        for (i = 0; i < starsPerIntensityLevel; i++) {
            particle[currentStar].setR(tmpIntensity);
            particle[currentStar].setG(tmpIntensity);
            particle[currentStar].setB(tmpIntensity);
            currentStar++;
        }
        tmpIntensity++;
    }

    // rand out positions
    for (i = 0; i < NUM_OF_STARS; i++) {
        particle[i].setX((rand() % 638) + 1);
        particle[i].setY((rand() % 478) + 1);
        float tmp = particle[i].getR()/**0.1*/;
        particle[i].setYVel((tmp * tmp * tmp) / 200000.0f);
    }
}

Starfield::~Starfield()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Starfield::update(Timer& timer)
{
    int i;
    for (i = 0; i < NUM_OF_STARS; i++) {
        particle[i].update(timer);
        if (particle[i].getY() > 478)
            particle[i].setY(1);
    }
}

void Starfield::draw(Screen& screen)
{
    screen.lock();
    int i;
    for (i = 0; i < NUM_OF_STARS; i++) {
        screen.fastSetPix((int)(particle[i].getX()), (int)(particle[i].getY()), \
                          particle[i].getR(), particle[i].getG(), particle[i].getB());
//      if(particle[i].r > 100)
//      {
        screen.fastSetPix((int)(particle[i].getX() + 1), (int)(particle[i].getY()), \
                          particle[i].getR(), particle[i].getG(), particle[i].getB());
        screen.fastSetPix((int)(particle[i].getX()), (int)(particle[i].getY() + 1), \
                          particle[i].getR(), particle[i].getG(), particle[i].getB());
        screen.fastSetPix((int)(particle[i].getX() + 1), (int)(particle[i].getY() + 1), \
                          particle[i].getR(), particle[i].getG(), particle[i].getB());
//      }
    }
    screen.unlock();
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

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
#include "Defines.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Starfield::Starfield()
{
    // set intensity of stars
    int intensity_levels = 140;
    int stars_per_intensity_level = (NUM_OF_STARS / intensity_levels);
    int current_star = 0;
    int i_intensity = 0;

    while (current_star < NUM_OF_STARS) {
        for (int i = 0; i < stars_per_intensity_level; ++i) {
            sdlc::Particle star;

            star.set_r(i_intensity);
            star.set_g(i_intensity);
            star.set_b(i_intensity);

            star.set_x((float)((rand() % (SCREEN_WIDTH - 2)) + 1));
            star.set_y((float)((rand() % (SCREEN_HEIGHT - 2)) + 1));
            star.set_y_vel((float)(star.r() * star.r() * star.r()) / 200000.0f);

            particles_.push_back(star);
            ++current_star;
        }
        ++i_intensity;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Starfield::update(sdlc::Timer& timer)
{
    for (auto& p : particles_) {
        p.update(timer);
        if (p.y() > 478)
            p.set_y(1);
    }
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

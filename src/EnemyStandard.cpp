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

#include "EnemyStandard.h"
#include "FxManager.h"
#include "ObjectManager.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

EnemyStandard::EnemyStandard(std::string name, int energy, int score, 
                             sdlc::Surface& s, sdlc::Surface& hit_s, 
                             float init_y_vel)
    : Object(name, energy, score, s, hit_s, ObjType::enemy, init_y_vel)
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------


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

#include "Environment.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Environment::Environment()
{
}

Environment::~Environment()
{
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

EnvironmentType Environment::getType() const
{
    return envType;
}

bool Environment::done() const
{
    return m_done;
}

// -----------------------------------------------------------------------------
// Protected Functions
// -----------------------------------------------------------------------------

EnvironmentType Environment::setType(EnvironmentType type)   
{
    return envType = type;
}

bool Environment::done(bool value) 
{
    return m_done = value;
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

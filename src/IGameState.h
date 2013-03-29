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

// -----------------------------------------------------------------------------
// Interface for the different game states. Used to represent the menu, the
// game world, the screen between different levels etc.
// -----------------------------------------------------------------------------

#ifndef KEXX2_IGAMESTATE_H
#define KEXX2_IGAMESTATE_H

#include "SDLc.h"
#include "Defines.h"

class PlayerState;

enum EnvironmentType { 
    ENV_NONE, 
    ENV_INTRO, 
    ENV_FINISHED, 
    ENV_MENU, 
    ENV_WORLD, 
    ENV_BUYSCREEN, 
    ENV_GAMEOVER 
};

class IGameState {
public:
    IGameState(EnvironmentType env_type) : env_type_(env_type) {};
    virtual ~IGameState() {};

    // Main game state functions
    virtual void run_logic(sdlc::Input&, sdlc::Timer&, sdlc::Mixer&, PlayerState&) = 0;
    virtual void draw(sdlc::Screen& screen, sdlc::Font& font) = 0;

    // Querying the game state.
    virtual EnvironmentType type() const; 
    virtual bool done() const;

protected:
    const EnvironmentType env_type_ = ENV_NONE;
    bool done_ = false;
};

// -----------------------------------------------------------------------------
// Inlines
// -----------------------------------------------------------------------------

inline
EnvironmentType IGameState::type() const
{
    return env_type_;
}

inline
bool IGameState::done() const
{
    return done_;
}

#endif // KEXX2_IGAMESTATE_H


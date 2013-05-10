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

#include "Finished.h"
#include "Options.h"
#include "PlayerState.h"

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

Finished::Finished(Options& options, PlayerState& player_state)
    : IGameState(ENV_FINISHED)
{
    UNUSED(player_state);
    finished_snd_.load(options.data_path + "soundfx/gamecomplete.wav");
    finished_snd_.play(0);
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void Finished::run_logic(sdlc::Input& input, sdlc::Timer& timer, 
                         sdlc::Mixer& mixer, PlayerState& player_state)
{
    UNUSED(timer);
    UNUSED(mixer);
    UNUSED(player_state);

    if (input.key_pressed(SDLK_RETURN, sdlc::AutofireKeystate::off))
        done_ = true;
}

void Finished::draw(sdlc::Screen& screen, sdlc::Font& font)
{
    screen.print(100, 100, "game complete!", font);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

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

#ifndef _KEXX2_FXMANAGER_H_
#define _KEXX2_FXMANAGER_H_

#include "SDLc.h"
#include <list>

class ParticleManager;

class FxManager {
public:
    FxManager();
    virtual ~FxManager();

    void load(ParticleManager& pManager, std::string dataPath);
    void update(sdlc::Timer& timer);
    void draw(sdlc::Screen& screen);

    void explodeNormal(int x, int y);
    void explodeTiny(int x, int y);
    void explodeTiny(int x, int y, float vel, float angle);
    void smokepuff(int x, int y);

    void playhitSnd();
    void playPlayerHitSnd();

private:
    // TODO: change to smart pointer
    ParticleManager* particleManager;
    sdlc::Sound explSndSmall;
    sdlc::Sound explSndBig;
    sdlc::Sound explSndPlayer;
    sdlc::Sound alarmSnd;
    sdlc::Surface explImg;
    std::list<sdlc::Sprite*> explosionList;

    struct prealculatedData {
        int intensity;
        float xVel;
        float yVel;
    } precalcNormExpl[10][256];
};

#endif

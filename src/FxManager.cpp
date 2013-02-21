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

#include "FxManager.h"
#include "Defines.h"
#include "SDLc/Sprite.h"
#include "SDLc/Screen.h"
#include "ParticleManager.h"
#include "SDLc/Timer.h"
#include <cmath>
#include <iostream>
using namespace std;

// -----------------------------------------------------------------------------
// Construction/Destruction
// -----------------------------------------------------------------------------

FxManager::FxManager()
{
    particleManager = 0;
}

FxManager::~FxManager()
{
    // free explosionList
    list<Sprite*>::iterator i = explosionList.begin();
    for (; i != explosionList.end(); i++) {
        Sprite* s = *i;
        delete s;
    }
}

// -----------------------------------------------------------------------------
// Member Functions
// -----------------------------------------------------------------------------

void FxManager::load(ParticleManager& pManager, std::string dataPath)
{
    particleManager = &pManager;

    explSndSmall.load(dataPath + "soundfx/explode2.wav");
    explSndBig.load(dataPath + "soundfx/explode3.wav");
    explImg.load(dataPath + "gfx/Explosion1.png");
    explSndPlayer.load(dataPath + "soundfx/playerhit.wav");
    alarmSnd.load(dataPath + "soundfx/alarm.wav");

    explSndSmall.setChannel((int)SND_EXPL_SMALL);
    explSndBig.setChannel((int)SND_EXPL_BIG);
    explSndPlayer.setChannel((int)SND_EXPL_PLAYER);
    alarmSnd.setChannel((int)SND_ALARM);

    // pre-calculate explosions
    int i, j;
    for (j = 0; j < 10; j++) {
        for (i = 0; i < 256; i++) {
            int intensity = (rand() % 256);
            int degree = (rand() % 360);

            float xVel = (cos((double)degree) * intensity)/* * 0.001*/;
            float yVel = (sin((double)degree) * intensity)/* * 0.001*/;

            precalcNormExpl[j][i].intensity = intensity;
            precalcNormExpl[j][i].xVel = xVel;
            precalcNormExpl[j][i].yVel = yVel;
        }
    }
}

void FxManager::update(Timer& timer)
{
    list<Sprite*>::iterator i = explosionList.begin();
    while (i != explosionList.end()) {
        Sprite* current = *i;
        i++;

        current->update(timer);
        if (!current->animationActive()) {
            explosionList.remove(current);
            delete current;
        }
    }
}

void FxManager::draw(Screen& screen)
{
    list<Sprite*>::iterator i = explosionList.begin();
    for (; i != explosionList.end(); i++) {
        Sprite* current = *i;
        screen.blit(*current);
    }
}

void FxManager::explodeNormal(int x, int y)
{
    explSndBig.play(0);

    Sprite* sprite = new Sprite;
    sprite->link(explImg.data);
    sprite->initAnimation(5, 11, 1);
    sprite->setX(x - sprite->getWidth() / 2);
    sprite->setY(y - sprite->getHeight() / 2);
    explosionList.push_back(sprite);

    int i, tmp = rand() % 10;
    for (i = 0; i < 128; i++) {
        int intensity = precalcNormExpl[tmp][i].intensity;
        float xVel = precalcNormExpl[tmp][i].xVel;
        float yVel = precalcNormExpl[tmp][i].yVel;
        particleManager->create(x, y, xVel, yVel, 255, 255, 255, intensity, 100.0f/*0.1f*/);
    }
}

void FxManager::explodeTiny(int x, int y)
{
    explodeTiny(x, y, 0, 0);
}

void FxManager::explodeTiny(int x, int y, float vel, float angle)
{
    playhitSnd();

    angle = angle * (3.1415927f / 180.0f);
    float xVelMod = cos(angle) * vel;
    float yVelMod = -sin(angle) * vel;

    int i;
    for (i = 0; i < 32; i++) {
        int intensity = (rand() % 256);
        int degree = (rand() % 360);
        int speed = intensity / 4;

        float xVel = (cos((double)degree) * speed)/* * 0.001*/ + xVelMod;
        float yVel = (sin((double)degree) * speed)/* * 0.001*/ + yVelMod;

        if (intensity > 255) {
            cout << "ExplodeTiny() intensity out of bounds! = " << intensity;
            cout << "\tx=" << x;
            cout << "  y=" << y;
            cout << "  xVel=" << xVel;
            cout << "  yVel=" << yVel << endl;
        }
        particleManager->create(x, y, xVel, yVel, 255, 255, 255, intensity, 100.0f/*0.1f*/);
    }
}

void FxManager::smokepuff(int x, int y)
{
    int i;
    extern Timer* timer;
    int amount = (int)((float)timer->frame_time() / 0.004f/*4.0f*/);
    for (i = 0; i < amount; i++) {
        float xVel = ((rand() % 50) - 25)/**0.001*/;
        float yVel = ((rand() % 50) - 25)/**0.001*/ + 100.0f/*0.1f*/;
        particleManager->create(x + (rand() % 10) - 5, y + (rand() % 10) - 5, xVel, yVel, 255, 255, 255, 100, 30.0f/*0.03f*/);
    }
}

void FxManager::playhitSnd()
{
    explSndSmall.play(0);
}

void FxManager::playPlayerHitSnd()
{
    explSndPlayer.play(0);
    alarmSnd.play(0);
}

// -----------------------------------------------------------------------------
// Private Functions
// -----------------------------------------------------------------------------

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

#ifndef _KEXX2_DEFINES_H_
#define _KEXX2_DEFINES_H_

#define VERSION "1.1.0"
#define UNUSED(x) ((void)x)

//extern int newCount;

const float SCROLLING_SPEED = 50.0f; //0.05f;
const int NUM_OF_POSSIBLE_PLAYERS = 2;

enum EnvironmentType { ENV_NONE, ENV_INTRO, ENV_FINISHED, ENV_MENU, ENV_WORLD, ENV_BUYSCREEN, ENV_GAMEOVER };
enum SoundChannel { SND_DIV, SND_W_BLASTER, SND_W_ROCKET, SND_EXPL_SMALL, SND_EXPL_BIG, SND_EXPL_PLAYER, SND_ALARM };
enum Owner   { OWNER_NONE, OWNER_PLAYER1, OWNER_PLAYER2, OWNER_ENEMY };
enum ObjType     { OBJ_UNDEFINED, OBJ_PASSIVE, OBJ_ENEMY, OBJ_BONUS, OBJ_PLAYER, OBJ_PLAYERPASSIVE, OBJ_SHOT };
enum ObjIndex {
    PLAYER1,
    PLAYER2,
    ENEMYSTD,
    ENEMYSIDEWAYS,
    ENEMYRAMMER,
    ENEMYBONUS,
    OBJECTBIGSHIP,
    BONUSBLASTER,
    BONUSROCKET,

    // shots
    SHOTBLASTER,
    SHOTBLASTERBIG,
    SHOTROCKET,
    SHOTBOMBFRAGMENT,
    SHOTENEMYSTD,

    // misc
    SMOKETRAIL,

    // formations
    ENEMYSTD_V_FORMATION,
    ENEMYSTD_3V_FORMATION,
    ENEMYSTD_DIAGONAL_FORMATION,
    ENEMYSTD_MASSIVE_FORMATION,

    ENEMYSIDEWAYS_VLINE_FORMATION,
    ENEMYSIDEWAYS_HLINE_FORMATION,
    ENEMYSIDEWAYS_V_FORMATION,
    ENEMYSIDEWAYS_MASSIVE_FORMATION,

    ENEMYRAMMER_VLINE_FORMATION,
    ENEMYRAMMER_DIAGONAL_FORMATION,
    ENEMYRAMMER_FULLDIAGONAL_FORMATION
};

// object sound (for those that has sound)
enum ObjSnd {
    SND_SHOTBLASTER,
    SND_SHOTROCKET
};

#endif

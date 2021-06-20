#pragma once
#ifndef _CONSTANT_H__
#define _CONSTANT_H__

#define GAME_SCREEN_WIDTH 1280
#define GAME_SCREEN_HEIGHT 800

#define MAX_LENGTH 7

#define BLOCK_SIZE 90

#define TOTAL_ANI 5

#define TOTAL_PRO 5

#include "AudioEngine.h"
#define SOUND_KEY "sound_key"
#define MUSIC_KEY "music_key"

#define userDefault CCUserDefault::sharedUserDefault()

enum AnimalsState
{
    COMMON = 0,
    HORIZONTAL,
    VERTICAL,
    BOMB,
    SUPERBIRD
};

enum BlockState
{
    EMPTY = 0,
    ANIMAL,
    ICE
};

enum PropType
{
    RESET = 0,
    ADDTHREE,
    ADDFIVE,
    ADDMAGIC,
    HAMMER
};

#define CAT 0

static const char *Common[TOTAL_ANI] =
{
    "Cat.png",
    "Chook.png",
    "Fox.png",
    "Frog.png",
    "Hippo.png"
};

static const char *CommonAni[TOTAL_ANI] =
{
    "CatCommon",
    "ChookCommon",
    "FoxCommon",
    "FrogCommon",
    "HippoCommon",
};

static const char *HorizontalAni[TOTAL_ANI] =
{
    "CatHorizontal",
    "ChookHorizontal",
    "FoxHorizontal",
    "FrogHorizontal",
    "HippoHorizontal"
};
static const char *VerticalAni[TOTAL_ANI] =
{
    "CatVertical",
    "ChookVertical",
    "FoxVertical",
    "FrogVertical",
    "HippoVertical"
};

static const char *BombAni[TOTAL_ANI] =
{
    "CatBomb",
    "ChookBomb",
    "FoxBomb",
    "FrogBomb",
    "HippoBomb"
};

static const char *PropPics[TOTAL_PRO] =
{
    "Props/Reset.png",
    "Props/AddThree.png",
    "Props/AddFive.png",
    "Props/AddMagic.png",
    "Props/Hammer.png"
};

static const char *SelPropPics[TOTAL_PRO] =
{
    "Props/SelReset.png",
    "Props/SelAddThree.png",
    "Props/SelAddFive.png",
    "Props/SelAddMagic.png",
    "Props/SelHammer.png"
};

static int bgmId;
static int worldBgmId;
#endif

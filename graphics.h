/*
 *  This file is part of 'Johnny Reborn'
 *
 *  An open-source engine for the classic
 *  'Johnny Castaway' screensaver by Sierra.
 *
 *  Copyright (C) 2019 Jeremie GUILLAUME
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <SDL2/SDL.h>

#define MAX_BMP_SLOTS       6
#define MAX_SPRITES_PER_BMP 120
#define MAX_TTM_SLOTS       10
#define MAX_TTM_THREADS     10


struct TAdsScene {
    uint16 slot;
    uint16 tag;
    uint16 numPlays;
};


struct TTtmSlot {
    uint8       *data;
    uint32      dataSize;
    struct      TTtmTag *tags;
    int         numTags;
    int         numSprites[MAX_BMP_SLOTS];
    SDL_Surface *sprites[MAX_BMP_SLOTS][MAX_SPRITES_PER_BMP];
};

struct TTtmTag {  // TODO : rename, used for ADS too
    uint16 id;
    uint32 offset;
};

struct TTtmThread {
    struct TTtmSlot   *ttmSlot;
    int    isRunning;
    uint16 sceneSlot;
    uint16 sceneTag;
    short  sceneTimer;
    uint16 sceneIterations;
    uint32 ip;
    uint16 delay;
    uint16 timer;
    uint32 nextGotoOffset;
    uint8  selectedBmpSlot;
    uint8  fgColor;
    uint8  bgColor;
    SDL_Surface *ttmLayer;
};

extern SDL_Surface *grBackgroundSfc;

extern int grDx;
extern int grDy;
extern int grWindowed;
extern int grUpdateDelay;


void graphicsInit();
void graphicsEnd();
void grRefreshDisplay();
void grToggleFullScreen();
void grUpdateDisplay(struct TTtmThread *ttmBackgroundThread,
                     struct TTtmThread *ttmThreads,
                     struct TTtmThread *ttmHolidayThreads);

SDL_Surface *grNewEmptyBackground();
SDL_Surface *grNewLayer();
void grFreeLayer(SDL_Surface *sfc);

void grLoadBmp(struct TTtmSlot *ttmSlot, uint16 slotNo, char *strArg);
void grReleaseBmp(struct TTtmSlot *ttmSlot, uint16 bmpSlotNo);

void grSetClipZone(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2);
void grCopyZoneToBg(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grSaveImage1(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grSaveZone(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grRestoreZone(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grDrawPixel(SDL_Surface *sfc, sint16 x, sint16 y, uint8 color);
void grDrawLine(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2, uint8 color);
void grDrawRect(SDL_Surface *sfc, sint16 x, sint16 y, uint16 width, uint16 height, uint8 color);
void grDrawCircle(SDL_Surface *sfc, sint16 x1, sint16 y1, uint16 width, uint16 height, uint8 fgColor, uint8 bgColor);
void grDrawSprite(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo);
void grDrawSpriteFlip(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo);
void grInitEmptyBackground();
void grClearScreen(SDL_Surface *sfc);
void grFadeOut();

void grLoadPalette();   // TODO
void grLoadScreen(char *strArg);


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

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#include "mytypes.h"
#include "utils.h"
#include "graphics.h"
#include "resource.h"
#include "ticks.h"


static SDL_Window *sdl_window;

static uint8 ttmPalette[16][4];

static SDL_Surface *grSavedZonesLayer = NULL;

SDL_Surface *grBackgroundSfc = NULL;

int grDx = 0;
int grDy = 0;
int grWindowed = 0;


static void grReleaseScreen()
{
    free(grBackgroundSfc->pixels);
    SDL_FreeSurface(grBackgroundSfc);
    grBackgroundSfc = NULL;
}


static void grReleaseSavedLayer()
{
    SDL_FreeSurface(grSavedZonesLayer);
    grSavedZonesLayer = NULL;
}


static void grPutPixel(SDL_Surface *sfc, uint16 x, uint16 y, uint8 color)
{
    // TODO: Implement Cohen-Sutherland clipping algorithm or such for
    // grDrawLine(), and another ad hoc algorithm for grDrawCircle()

    if (x>=0 && y>=0 && x<640 && y<480) {

        uint8 *pixel = (uint8*) sfc->pixels;

        pixel += (y * sfc->pitch) + (x * sfc->format->BytesPerPixel);

        pixel[0] = ttmPalette[color][0];
        pixel[1] = ttmPalette[color][1];
        pixel[2] = ttmPalette[color][2];
        pixel[3] = 0;
    }
}


static void grDrawHorizontalLine(SDL_Surface *sfc, sint16 x1, sint16 x2, sint16 y, uint8 color)
{
    if (y < 0 || y > 479)
        return;

    x1 = x1 < 0   ? 0   : x1;
    x2 = x2 > 639 ? 639 : x2;

    for (int x=x1; x<=x2; x++)
        grPutPixel(sfc, x, y, color);
}


void grLoadPalette(struct TPalResource *palResource)
{
    if (palResource == NULL)
        fatalError("NULL palette\n");

    for (int i=0; i < 16; i++) {
        ttmPalette[i][0] = palResource->colors[i].b << 2;
        ttmPalette[i][1] = palResource->colors[i].g << 2;
        ttmPalette[i][2] = palResource->colors[i].r << 2;
        ttmPalette[i][3] = 0;
    }
}


void graphicsInit()
{
    SDL_Init(SDL_INIT_VIDEO);

    sdl_window = SDL_CreateWindow(
        "Johnny Reborn ...?",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        (grWindowed ? 0 : SDL_WINDOW_FULLSCREEN)
    );

    if (sdl_window == NULL)
        fatalError("Could not create window: %s", SDL_GetError());

    if (!grWindowed)
        SDL_ShowCursor(SDL_DISABLE);

    SDL_UpdateWindowSurface(sdl_window);

    grLoadPalette(palResources[0]);  // TODO ?

    srand(time(NULL));
}


void graphicsEnd()
{
    SDL_DestroyWindow(sdl_window);
    SDL_Quit();
}


void graphicsRefreshWindow()
{
    SDL_UpdateWindowSurface(sdl_window);
}


void graphicsToggleFullScreen()
{
    grWindowed = !grWindowed;

    if (grWindowed) {
        SDL_SetWindowFullscreen(sdl_window, 0);
        SDL_ShowCursor(SDL_ENABLE);
    }
    else {
        SDL_SetWindowFullscreen(sdl_window, SDL_WINDOW_FULLSCREEN);
        SDL_ShowCursor(SDL_DISABLE);
    }

    SDL_UpdateWindowSurface(sdl_window);
}


void graphicsUpdate(struct TTtmThread *ttmBackgroundThread,
                    struct TTtmThread *ttmThreads,
                    struct TTtmThread *ttmHolidayThread)
{
    // Blit the background
    if (grBackgroundSfc != NULL)
        SDL_BlitSurface(grBackgroundSfc,
                        NULL,
                        SDL_GetWindowSurface(sdl_window),
                        NULL);

    // If not NULL, blit the optional layer of saved zones
    if (grSavedZonesLayer != NULL)
        SDL_BlitSurface(grSavedZonesLayer,
                        NULL,
                        SDL_GetWindowSurface(sdl_window),
                        NULL);


    // Blit successively each thread's layer
    for (int i=0; i < MAX_TTM_THREADS; i++)
        if (ttmThreads[i].isRunning)
            SDL_BlitSurface(ttmThreads[i].ttmLayer,
                            NULL,
                            SDL_GetWindowSurface(sdl_window),
                            NULL);

    // Finally, blit the holiday layer
    if (ttmHolidayThread != NULL)
        if (ttmHolidayThread->isRunning)
            SDL_BlitSurface(ttmHolidayThread->ttmLayer,
                            NULL,
                            SDL_GetWindowSurface(sdl_window),
                            NULL);


    SDL_UpdateWindowSurface(sdl_window);
}


SDL_Surface *grNewLayer()
{
    SDL_Surface *sfc = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, 0, 0, 0, 0);
    SDL_Rect dest = { 0, 0, 640, 480 };
    SDL_FillRect(sfc, &dest, SDL_MapRGB(sfc->format, 0xa8, 0, 0xa8));
    SDL_SetColorKey(sfc, SDL_TRUE, SDL_MapRGB(sfc->format, 0xa8, 0, 0xa8));

    return sfc;
}


void grFreeLayer(SDL_Surface *sfc)
{
    SDL_FreeSurface(sfc);
}


void grSetClipZone(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2)
{
    x1 += grDx; y1 += grDy;
    x2 += grDx; y2 += grDy;

    SDL_Rect rect = { x1, y1, x2-x1, y2-y1 };
    SDL_SetClipRect(sfc, &rect);
}


void grCopyZoneToBg(SDL_Surface *sfc, uint16 x, uint16 y, uint16 width, uint16 height)
{
    x += grDx; y += grDy;
    SDL_Rect rect = { (short) x, (short) y, width + 2, height };

    if (grSavedZonesLayer == NULL)
        grSavedZonesLayer = grNewLayer();

    SDL_BlitSurface(sfc, &rect, grSavedZonesLayer, &rect);

    // Note : without the +2 in width+2 above, there would be a graphical
    // glitch (2 unfilled pixels) on the hull of the cargo, caused by an
    // error in coordinates in GJIVS6.TTM
    // Obviously, the original soft rounds the SAVE_IMAGE boundaries on
    // one way or another.
}


void grSaveImage1(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3) // TODO : rename ?
{
//    ttmSetColors(4,4);
//    ttmDrawRect(arg0,arg1,arg2,arg3);
//    ttmSaveImage0(arg0,arg1,arg2,arg3);
//    ttmUpdate();
}


void grSaveZone(SDL_Surface *sfc, uint16 x, uint16 y, uint16 width, uint16 height)
{
    // Minimalistic implementation: we don't really save the zone,
    // and let grRestoreZone() simply erase the 'saved zones' layer
}


void grRestoreZone(SDL_Surface *sfc, uint16 x, uint16 y, uint16 width, uint16 height)
{
    // In Johnny's TTMs, we never have RESTORE_ZONE called
    // while several zones are saved. So we simply free the
    // whole saved zones layer
    grReleaseSavedLayer();
}


void grDrawLine(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2, uint8 color)
{
    x1 += grDx; y1 += grDy;
    x2 += grDx; y2 += grDy;

    SDL_LockSurface(sfc);

    // Bresenham's line drawing algorithm
    // Note : the code below intends to be pixel-perfect

    uint16 dx, dy, cumul, x, y;
    int xinc, yinc;

    x = x1;
    y = y1;
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);

    xinc = (x2>x1 ? 1 : -1);
    yinc = (y2>y1 ? 1 : -1);

    if (dy < dx) {
        cumul = (dx + 1) >> 1;

        for (int i=0; i < dx; i++) {

            grPutPixel(sfc, x, y, color);

            x += xinc;
            cumul += dy;

            if (cumul > dx) {
                cumul -= dx;
                y += yinc;
            }
        }
    }
    else {
        cumul = (dy + 1) >> 1;

        for (int i=0; i < dy; i++) {

            grPutPixel(sfc, x, y, color);

            y += yinc;
            cumul += dx;

            if (cumul > dy) {
                cumul -= dy;
                x += xinc;
            }
        }
    }

    SDL_UnlockSurface(sfc);
}


void grDrawRect(SDL_Surface *sfc, sint16 x, sint16 y, uint16 width, uint16 height, uint8 color)
{
    x += grDx; y += grDy;

    SDL_Rect dest = { x, y, width, height };
    SDL_FillRect(sfc,
                 &dest,
                 SDL_MapRGB(sfc->format,
                            ttmPalette[color][2],  // TODO ?
                            ttmPalette[color][1],
                            ttmPalette[color][0]
                 )
    );
}


void grDrawCircle(SDL_Surface *sfc, sint16 x1, sint16 y1, uint16 width, uint16 height, uint8 fgColor, uint8 bgColor)
{
    x1 += grDx; y1 += grDy;

    // We can only draw regular circles
    if (width != height) {
        fprintf(stderr, "Warning : grDrawCircle() : unable to draw ellipse\n");
        return;
    }

    // In original data, every width is even
    if (width % 2) {
        fprintf(stderr, "Warning : grDrawCircle() : unable to process odd diameters\n");
        return;
    }

    // Bresenham's circle drawing algorithm
    // Note : the code below intends to be pixel-perfect

    SDL_LockSurface(sfc);

    uint16 r = (width >> 1) - 1;
    uint16 xc = x1 + r;
    uint16 yc = y1 + r;
    sint16 x = 0;
    sint16 y = r;
    int d = 1 - r;

    while (1) {

        grDrawHorizontalLine(sfc, xc-x, xc+x+1, yc+y+1, bgColor);
        grDrawHorizontalLine(sfc, xc-x, xc+x+1, yc-y  , bgColor);

        grDrawHorizontalLine(sfc, xc-y, xc+y+1, yc+x+1, bgColor);
        grDrawHorizontalLine(sfc, xc-y, xc+y+1, yc-x  , bgColor);

        if (y-x <= 1)
            break;

        if (d < 0)
            d += (x << 1) + 3;
        else {
            d += ((x - y) << 1) + 5;
            y--;
        }

        x++;
    }

    if (fgColor != bgColor) {

        x = 0;
        y = r;
        d = 1 - r;

        while (1) {

            grPutPixel(sfc, xc-x  , yc+y+1, fgColor);
            grPutPixel(sfc, xc+x+1, yc+y+1, fgColor);

            grPutPixel(sfc, xc-x  , yc-y  , fgColor);
            grPutPixel(sfc, xc+x+1, yc-y  , fgColor);

            grPutPixel(sfc, xc-y  , yc+x+1, fgColor);
            grPutPixel(sfc, xc+y+1, yc+x+1, fgColor);

            grPutPixel(sfc, xc-y  , yc-x  , fgColor);
            grPutPixel(sfc, xc+y+1, yc-x  , fgColor);

            if (y-x <= 1)
                break;

            if (d < 0)
                d += (x << 1) + 3;
            else {
                d += ((x - y) << 1) + 5;
                y--;
            }

            x++;
        }
    }

    SDL_UnlockSurface(sfc);
}


void grDrawSprite(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo)
{
    if (spriteNo >= ttmSlot->numSprites[imageNo]) {
        fprintf(stderr, "Warning : grDrawSprite(): less than %d sprites loaded in slot %d\n", imageNo, spriteNo);
        return;
    }

    x += grDx; y += grDy;

    SDL_Surface *srcSfc = ttmSlot->sprites[imageNo][spriteNo];

    SDL_Rect dest = { x, y, 0, 0 };
    SDL_BlitSurface(srcSfc, NULL, sfc, &dest);
}


void grDrawSpriteFlip(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo)
{
    if (spriteNo >= ttmSlot->numSprites[imageNo]) {
        fprintf(stderr, "Warning : grDrawSpriteFlip(): less than %d sprites loaded in slot %d\n", imageNo, spriteNo);
        return;
    }

    x += grDx; y += grDy;

    SDL_Surface *srcSfc = ttmSlot->sprites[imageNo][spriteNo];

    for (int i=0; i < srcSfc->w; i++) {

        SDL_Rect src = { i, 0, 1, srcSfc->h };
        SDL_Rect dest = { x + srcSfc->w - i, y, 0, 0 };

        SDL_BlitSurface(srcSfc, &src, sfc, &dest);
    }
}


void grClearScreen(SDL_Surface *sfc)
{
    SDL_Rect rect;

    SDL_GetClipRect(sfc, &rect);
    SDL_SetClipRect(sfc, NULL);
    SDL_FillRect(sfc, NULL, SDL_MapRGB(sfc->format, 0xa8, 0, 0xa8));
    SDL_SetClipRect(sfc, &rect);
}


void grLoadScreen(char *strArg)
{
    if (grBackgroundSfc != NULL)
        grReleaseScreen();

    if (grSavedZonesLayer != NULL)
        grReleaseSavedLayer();

    struct TScrResource *scrResource = findScrResource(strArg);

    if ((scrResource->width % 2) == 1) {
        fprintf(stderr, "Warning: grLoadScreen(): can't manage odd widths\n");
    }

    if (scrResource->width > 640 || scrResource->height > 480) {
        fatalError("grLoadScreen(): can't manage more than 640x480 resolutions");
    }

    uint16 width  = scrResource->width;
    uint16 height = scrResource->height;

    uint8 *outData = safe_malloc(width * height * sizeof(uint32));

    uint8 *inPtr  = scrResource->uncompressedData;
    uint8 *outPtr = outData;

    for (int inOffset=0; inOffset < width*height/2; inOffset++) {
        memcpy(outPtr, ttmPalette[(inPtr[0] & 0xf0) >> 4] , 4); outPtr += 4;
        memcpy(outPtr, ttmPalette[(inPtr[0] & 0x0f)     ] , 4); outPtr += 4;
        inPtr++;
    }

    grBackgroundSfc = SDL_CreateRGBSurfaceFrom((void*)outData,
                                      width, height, 32, 4*width, 0, 0, 0, 0);
}


void grInitEmptyBackground()
{
    if (grBackgroundSfc != NULL)
        grReleaseScreen();

    if (grSavedZonesLayer != NULL)
        grReleaseSavedLayer();

    uint8 *data = safe_malloc(640 * 480 * sizeof(uint32));
    memset(data, 0, 640 * 480 * sizeof(uint32));
    grBackgroundSfc = SDL_CreateRGBSurfaceFrom((void*)data,
                                      640, 480, 32, 4*640, 0, 0, 0, 0);
}


void grReleaseBmp(struct TTtmSlot *ttmSlot, uint16 bmpSlotNo)
{
    for (int i=0; i < ttmSlot->numSprites[bmpSlotNo]; i++) {
        free(ttmSlot->sprites[bmpSlotNo][i]->pixels);
        SDL_FreeSurface(ttmSlot->sprites[bmpSlotNo][i]);
    }

    ttmSlot->numSprites[bmpSlotNo] = 0;
}


void grLoadBmp(struct TTtmSlot *ttmSlot, uint16 slotNo, char *strArg)
{
    if (ttmSlot->numSprites[slotNo])
        grReleaseBmp(ttmSlot, slotNo);

    struct TBmpResource *bmpResource = findBmpResource(strArg);
    uint8 *inPtr = bmpResource->uncompressedData;

    ttmSlot->numSprites[slotNo] = bmpResource->numImages;

    for (int image=0; image < bmpResource->numImages; image++) {

        if ((bmpResource->widths[image] % 2) == 1)
            fatalError("grLoadBmp(): can't manage odd widths");

        uint16 width  = bmpResource->widths[image];
        uint16 height = bmpResource->heights[image];

        uint8 *outData = safe_malloc(width * height * sizeof(uint32));

        uint8 *outPtr = outData;

        for (int inOffset=0; inOffset < (width*height/2); inOffset++) {
            memcpy(outPtr, ttmPalette[(inPtr[0] & 0xf0) >> 4] , 4); outPtr += 4;
            memcpy(outPtr, ttmPalette[(inPtr[0] & 0x0f)     ] , 4); outPtr += 4;
            inPtr++;
        }

        SDL_Surface *surface = SDL_CreateRGBSurfaceFrom((void*)outData,
                                               width, height, 32, 4*width, 0, 0, 0, 0);
        SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0xa8, 0, 0xa8));
        ttmSlot->sprites[slotNo][image] = surface;
    }
}


void grFadeOut()
{
    static int fadeOutType = 0;
    SDL_Surface *sfc = SDL_GetWindowSurface(sdl_window);
    SDL_Surface *tmpSfc = grNewLayer();


    grDx = grDy = 0;

    switch (fadeOutType) {

        // Circle from center
        case 0:
            // Note: we use tmpSfc to be sure we have a 32bpp surface,
            // which is needed by grDrawCircle()
            for (int radius=20; radius <= 400; radius += 20) {
                grDrawCircle(tmpSfc, 320 - radius, 240 - radius,
                    radius << 1, radius << 1, 5, 5);
                SDL_BlitSurface(tmpSfc, NULL, sfc, NULL);
                SDL_UpdateWindowSurface(sdl_window);
                ticksWait(1);
            }
            break;

        // Rectangle from center
        case 1:
            for (int i=1; i <= 20; i++) {
                grDrawRect(sfc, 320 - i*16, 240 - i*12, i*32, i*24, 5);
                SDL_UpdateWindowSurface(sdl_window);
                ticksWait(1);
            }
            break;

        // Right to left
        case 2:
            for (int i=600; i >= 0; i -= 40) {
                grDrawRect(sfc, i, 0, 40, 480, 5);
                SDL_UpdateWindowSurface(sdl_window);
                ticksWait(1);
            }
            break;

        // Left to right
        case 3:
            for (int i=0; i < 640; i += 40) {
                grDrawRect(sfc, i, 0, 40, 480, 5);
                SDL_UpdateWindowSurface(sdl_window);
                ticksWait(1);
            }
            break;

        // Middle to left and right
        case 4:
            for (int i=0; i < 320; i += 20) {
                grDrawRect(sfc, 320+i, 0, 20, 480, 5);
                grDrawRect(sfc, 300-i, 0, 20, 480, 5);
                SDL_UpdateWindowSurface(sdl_window);
                ticksWait(1);
            }
            break;
    }

    grFreeLayer(tmpSfc);

    fadeOutType = (fadeOutType + 1) % 5;
}


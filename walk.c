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

#include "mytypes.h"
#include "graphics.h"
#include "ticks.h"
#include "utils.h"
#include "calcpath.h"
#include "walk.h"
#include "walk_data.h"


static int *walkPath;
static int currentSpot;
static int currentHdg;
static int nextSpot;
static int nextHdg;
static int finalSpot;
static int finalHdg;
static int increment;
static int lastTurn;
static int hasArrived;
static int isBehindTree;


void walkInit(int fromSpot, int fromHdg, int toSpot, int toHdg)
{
    walkPath = calcPath(fromSpot, toSpot);

    currentSpot  = fromSpot;
    currentHdg   = fromHdg;
    finalSpot    = toSpot;
    finalHdg     = toHdg;
    hasArrived   = 0;
    isBehindTree = 0;

    if (currentSpot == finalSpot) {
        nextSpot = -1;
        nextHdg = finalHdg;
        lastTurn = 1;
    }
    else {
        nextSpot = *(++walkPath);
        nextHdg = walkDataStartHeadings[currentSpot][nextSpot];
        lastTurn = 0;
    }

    if ((increment = ((nextHdg - currentHdg) & 0x07)))
        increment = (increment < 4 ? 1 : -1);
}


int walkAnimate(struct TTtmThread *ttmThread, struct TTtmSlot *ttmBgSlot)
{
    struct TTtmSlot *ttmSlot = ttmThread->ttmSlot;
    SDL_Surface *sfc = ttmThread->ttmLayer;
    static uint16 (*data)[4] = NULL;
    int delay;

    if (!hasArrived) {

        // Are we turning ?
        if (nextHdg != -1) {

            // More than one iteration left? yes, so let's turn
            if ((((nextHdg - currentHdg) & 0x07) % 7 ) > 1) {
                currentHdg = (currentHdg + increment ) & 7;
                data = &walkData[walkDataBookmarksTurns[currentSpot] + currentHdg];
                if (lastTurn)
                    data += 9;
            }

            // The turn is over
            else {

                // Do we have another spot to walk to ?
                if (currentSpot != finalSpot) {
                    nextHdg = -1;
                    isBehindTree = ((currentSpot == 3) && (nextSpot == 4))
                                      || ((currentSpot == 4) && (nextSpot == 3));
                    data = &walkData[walkDataBookmarks[currentSpot][nextSpot]];
                }

                // Else, we arrived to destination
                else {
                    data = &walkData[walkDataBookmarksTurns[finalSpot] + finalHdg];
                    data += 9;     // hands in pockets
                    hasArrived = 1;
                }
            }
        }

        // Walking forward
        else {

            data++;

            // Have we reached a spot ? So lets begin a turn...
            if (!(*data)[1]) {

                currentHdg = walkDataEndHeadings[currentSpot][nextSpot];
                currentSpot = nextSpot;

                // What's the next heading ?
                // And the next spot of the path to reach ?
                if (currentSpot != finalSpot) {
                    nextSpot = *(++walkPath);
                    nextHdg = walkDataStartHeadings[currentSpot][nextSpot];
                }
                else {
                    nextHdg = finalHdg;
                    lastTurn = 1;
                }

                // Turning: left or right ?
                if ((increment = ((nextHdg - currentHdg) & 0x07)))
                    increment = (increment < 4 ? 1 : -1);

                currentHdg = (currentHdg + increment) & 7;
                data = &walkData[walkDataBookmarksTurns[currentSpot] + currentHdg];

                if (lastTurn) {
                    data += 9;   // hands in pockets
                    if (currentHdg == finalHdg)
                        hasArrived = 1;
                }
            }
        }

        debugMsg("WALKING:  spot=%d hdg=%d next=%d  -  data %d %d %d %d\n",
            currentSpot, currentHdg, nextHdg,
            (*data)[0], (*data)[1], (*data)[2], (*data)[3]);

        grClearScreen(sfc);

        if ((*data)[0])
            grDrawSpriteFlip(sfc, ttmSlot,
                (*data)[1] - 1, (*data)[2], (*data)[3], 0);
        else
            grDrawSprite(sfc, ttmSlot,
                (*data)[1] - 1, (*data)[2], (*data)[3], 0);

        if (isBehindTree)
            grDrawSprite(sfc, ttmBgSlot, 442, 148, 13, 0);

        if (hasArrived)
            delay = 80;
        else
            delay = 6;
    }
    else {
        debugMsg("WALKING: end walk\n");
        delay = 0;
    }

    return delay;
}


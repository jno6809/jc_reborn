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

#include "mytypes.h"
#include "graphics.h"
#include "island.h"


struct TIslandState islandState = { 0, 0, 0, 0, 0, 0 };


void islandInit(struct TTtmThread *ttmThread)
{
    struct TTtmSlot *ttmSlot = ttmThread->ttmSlot;


    if (islandState.night) {
        grLoadScreen("NIGHT.SCR");
    }
    else {
        char scrName[12];
        sprintf(scrName, "OCEAN0%d.SCR", rand() % 3);
        grLoadScreen(scrName);
    }

    ttmThread->ttmLayer = grBackgroundSfc;

    grDx = islandState.xPos;
    grDy = islandState.yPos;


    // Raft

    grLoadBmp(ttmSlot, 0, "MRAFT.BMP");

    int xRaft = (islandState.lowTide ? 529 : 512);
    int yRaft = (islandState.lowTide ? 281 : 266);

    switch (islandState.raft) {
        case 1: grDrawSprite(grBackgroundSfc, ttmSlot, xRaft, yRaft, 0, 0); break;  // raft-1
        case 2: grDrawSprite(grBackgroundSfc, ttmSlot, xRaft, yRaft, 1, 0); break;  // raft-2
        case 3: grDrawSprite(grBackgroundSfc, ttmSlot, xRaft, yRaft, 2, 0); break;  // raft-3
        case 4: grDrawSprite(grBackgroundSfc, ttmSlot, xRaft, yRaft, 3, 0); break;  // raft-4
        case 5: grDrawSprite(grBackgroundSfc, ttmSlot, xRaft, yRaft, 4, 0); break;  // raft-5
    }


    grLoadBmp(ttmSlot, 0, "BACKGRND.BMP");


    // Clouds

    int windDirection = rand() % 2;
    int numClouds;
    uint16 cloudX, cloudY;

    grDx = grDy = 0;

    if (rand() % 2)
         numClouds = 1;
    else if (rand() % 2)
        numClouds = 0;
    else if (rand() % 4)
        numClouds = 2;
    else if (rand() % 4)
        numClouds = 3;
    else if (rand() % 4)
        numClouds = 4;
    else
        numClouds = 5;

    for (int i=0; i < numClouds; i++) {

        int cloudNo = rand() % 3;

        switch (cloudNo) {

            case 0:
                cloudX = rand() % (640 - 129);
                cloudY = rand() % (135 - 36 );
                break;

            case 1:
                cloudX = rand() % (640 - 192);
                cloudY = rand() % (135 - 57 );
                break;

            case 2:
                cloudX = rand() % (640 - 264);
                cloudY = rand() % (135 - 76 );
                break;
        }

        if (windDirection)
            grDrawSprite(grBackgroundSfc, ttmSlot, cloudX, cloudY, 15 + cloudNo, 0);
        else
            grDrawSpriteFlip(grBackgroundSfc, ttmSlot, cloudX, cloudY, 15 + cloudNo, 0);
    }

    grDx = islandState.xPos;
    grDy = islandState.yPos;


    // The island itself

    grDrawSprite(grBackgroundSfc, ttmSlot, 288, 279,  0, 0);      // island
    grDrawSprite(grBackgroundSfc, ttmSlot, 442, 148, 13, 0);      // trunk
    grDrawSprite(grBackgroundSfc, ttmSlot, 365, 122, 12, 0);      // leafs
    grDrawSprite(grBackgroundSfc, ttmSlot, 396, 279, 14, 0);      // palmtree's shadow

    if (islandState.lowTide) {
        grDrawSprite(grBackgroundSfc, ttmSlot, 249, 303,  1, 0);  // low tide shore
        grDrawSprite(grBackgroundSfc, ttmSlot, 150, 328,  2, 0);  // rock
    }


    // Initial waves on the shore
    for (int i=0; i < 4; i++)
        islandAnimate(ttmThread);


    // Waves animation thread
    ttmThread->delay = ttmThread->timer = 8;
}


void islandAnimate(struct TTtmThread *ttmThread)
{
    static int counter1 = 0;
    static int counter2 = 0;

    struct TTtmSlot *ttmSlot = ttmThread->ttmSlot;


    grDx = islandState.xPos;
    grDy = islandState.yPos;

    if (islandState.lowTide) {

        counter2++;
        counter2 %= 4;

        switch (counter2) {
            case 0: grDrawSprite(grBackgroundSfc, ttmSlot, 129, 340, 39+counter1, 0); break;  // rock waves (40)
            case 1: grDrawSprite(grBackgroundSfc, ttmSlot, 233, 323, 30+counter1, 0); break;  // low tide waves - left (31)
            case 2: grDrawSprite(grBackgroundSfc, ttmSlot, 367, 356, 33+counter1, 0); break;  // low tide waves - center (33)
            case 3: grDrawSprite(grBackgroundSfc, ttmSlot, 558, 323, 36+counter1, 0); break;  // low tide waves - right (36)
        }
    }
    else {

        counter2++;
        counter2 %= 3;

        switch (counter2) {
            case 0: grDrawSprite(grBackgroundSfc, ttmSlot, 270, 306, 3+counter1, 0); break;  // high tide waves - left (3)
            case 1: grDrawSprite(grBackgroundSfc, ttmSlot, 364, 319, 6+counter1, 0); break;  // high tide waves - center (6)
            case 2: grDrawSprite(grBackgroundSfc, ttmSlot, 518, 303, 9+counter1, 0); break;  // high tide waves - right (9)
        }
    }

    if (!counter2) {
        counter1++;
        counter1 %= 3;
    }
}


void islandInitHoliday(struct TTtmThread *ttmThread)
{
    struct TTtmSlot *ttmSlot = ttmThread->ttmSlot;

    if (islandState.holiday) {

        grDx = islandState.xPos;
        grDy = islandState.yPos;

        grLoadBmp(ttmSlot, 0, "HOLIDAY.BMP");

        switch (islandState.holiday) {
            case 1: grDrawSprite(ttmThread->ttmLayer, ttmSlot, 410, 298, 0, 0); break;      // Halloween : 29/10 to 31/10
            case 2: grDrawSprite(ttmThread->ttmLayer, ttmSlot, 333, 286, 1, 0); break;      // St Patrick: 15/03 to 17/03
            case 3: grDrawSprite(ttmThread->ttmLayer, ttmSlot, 404, 267, 2, 0); break;      // Christmas : 23/12 to 25/12
            case 4: grDrawSprite(ttmThread->ttmLayer, ttmSlot, 361, 155, 3, 0); break;      // New year  : 29/12 to 01/01
        }

        grReleaseBmp(ttmSlot,0);
    }
}


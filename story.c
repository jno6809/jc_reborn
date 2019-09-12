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

#include "mytypes.h"
#include "utils.h"
#include "graphics.h"
#include "sound.h"
#include "ttm.h"
#include "ads.h"
#include "island.h"
#include "config.h"
#include "story.h"
#include "story_data.h"


static int storyCurrentDay = 1;


static struct TStoryScene *storyPickScene(
                uint16 wantedFlags, uint16 unwantedFlags)
{
    int scenes[NUM_SCENES];
    int numScenes = 0;


    for (int i=0; i < NUM_SCENES; i++) {

        struct TStoryScene scene = storyScenes[i];

        if ((scene.flags & wantedFlags) == wantedFlags
             && !(scene.flags & unwantedFlags)
             && (scene.dayNo == 0 || scene.dayNo == storyCurrentDay)
           ) {
            scenes[numScenes++] = i;
        }
    }

    return &storyScenes[scenes[rand() % numScenes]];
}


static void storyUpdateCurrentDay()
{
    struct TConfig config;
    int today;
    int hasChanged = 0;

    cfgFileRead(&config);
    today = getDayOfYear();

    if (today != config.date) {
        debugMsg("System date has changed since last sequence -> next day of the story");
        config.date = today;
        config.currentDay += 1;
        hasChanged = 1;
    }

    if (config.currentDay < 1 || config.currentDay > 11) {
        config.currentDay = 1;
        hasChanged = 1;
    }

    if (hasChanged)
        cfgFileWrite(&config);

    storyCurrentDay = config.currentDay;
    debugMsg("The day of the story is: %d", storyCurrentDay);
}


static void storyCalculateIslandFromDateAndTime()
{
    // Night ?
    int hour = (getHour() % 8);
    islandState.night = (hour == 0 || hour == 7);

    // Holidays ?
    islandState.holiday = 0;
    char *currentDate = getMonthAndDay();

    // Halloween : 29/10 to 31/10
    if (strcmp("1028", currentDate) < 0 && strcmp(currentDate, "1101") < 0)
        islandState.holiday = 1;
    else
    // St Patrick: 15/03 to 17/03
    if (strcmp("0314", currentDate) < 0 && strcmp(currentDate, "0318") < 0)
        islandState.holiday = 2;
    else
    // Christmas : 23/12 to 25/12
    if (strcmp("1222", currentDate) < 0 && strcmp(currentDate, "1226") < 0)
        islandState.holiday = 3;
    else
    // New year  : 29/12 to 01/01
    if (strcmp("1228", currentDate) < 0 || strcmp(currentDate, "0102") < 0)
        islandState.holiday = 4;

}


static void storyCalculateIslandFromScene(struct TStoryScene *scene)
{
    // Low tide ?
    if ((scene->flags & LOWTIDE_OK) && (rand() % 2))
        islandState.lowTide = 1;
    else
        islandState.lowTide = 0;


    // Randomize the position of the island
    if (scene->flags  & VARPOS_OK) {
        if (rand() % 2) {
            islandState.xPos = -222 + (rand() % 109);
            islandState.yPos = -44  + (rand() % 128);
        }
        else if (rand() % 2) {
            islandState.xPos = -114 + (rand() % 134);
            islandState.yPos = -14  + (rand() % 99 );
        }
        else {
            islandState.xPos = -114 + (rand() % 119);
            islandState.yPos = -73  + (rand() % 60 );
        }
    }
    else {
        if (scene->flags & LEFT_ISLAND) {
            islandState.xPos    = -272;
            islandState.yPos    = 0;
        }
        else {
            islandState.xPos    = 0;
            islandState.yPos    = 0;
        }
    }


    // How much of the raft was John able to build ?
    if (scene->flags & NORAFT) {
        islandState.raft = 0;
    }
    else {
        switch (storyCurrentDay) {

            case 0:
            case 1:
            case 2:
                islandState.raft = 1;
                break;

            case 3:
            case 4:
            case 5:
                islandState.raft = storyCurrentDay - 1;
                break;

            default:
                islandState.raft = 5;
                break;
        }
    }


    // For scene VISITOR.ADS#3 (cargo), never display holiday items - or they
    // will be drawn over the hull when it fills the screen at the end. This
    // conforms to the behavior of the original - which, moreover, freezes
    // the shore animation while we dont
    if (scene->flags & HOLIDAY_NOK)
        islandState.holiday = 0;
}


void storyPlay()
{
    uint16 wantedFlags   = 0;
    uint16 unwantedFlags = 0;


    adsInit();
    adsPlayIntro();

    while (1) {

        storyUpdateCurrentDay();
        storyCalculateIslandFromDateAndTime();
        unwantedFlags = 0;

        struct TStoryScene *finalScene = storyPickScene(FINAL, unwantedFlags);

        if (finalScene->flags & ISLAND) {
            storyCalculateIslandFromScene(finalScene);
            adsInitIsland();
        }
        else {
            adsNoIsland();
        }

        int prevSpot = -1;
        int prevHdg  = -1;

        if (!(finalScene->flags & FIRST)) {

            wantedFlags = 0;
            unwantedFlags |= FINAL;

            if (islandState.lowTide)
                wantedFlags |= LOWTIDE_OK;

            if (islandState.xPos || islandState.yPos)
                wantedFlags |= VARPOS_OK;

            for (int i=0; i < 6 + (rand() % 14); i++) {

                struct TStoryScene *scene = storyPickScene(wantedFlags,
                                                           unwantedFlags);

                if (prevSpot != -1)
                    adsPlayWalk(prevSpot, prevHdg,
                        scene->spotStart, scene->hdgStart);

                ttmDx = islandState.xPos
                            + (scene->flags & LEFT_ISLAND ? 272 : 0);
                ttmDy = islandState.yPos;

                if (scene->dayNo)
                    soundPlay(0);

                adsPlay(scene->adsName, scene->adsTagNo);

                unwantedFlags |= FIRST;
                prevSpot = scene->spotEnd;
                prevHdg = scene->hdgEnd;
            }
        }

        if (prevSpot != -1)
            adsPlayWalk(prevSpot, prevHdg, finalScene->spotStart, finalScene->hdgStart);

        if (finalScene->flags & ISLAND) {
            ttmDx = islandState.xPos + (finalScene->flags & LEFT_ISLAND ? 272 : 0);
            ttmDy = islandState.yPos;
        }
        else {
            ttmDx = ttmDy = 0;
        }

        if (finalScene->dayNo)
            soundPlay(0);

        adsPlay(finalScene->adsName, finalScene->adsTagNo);

        grFadeOut();

        if (finalScene->flags & ISLAND)
            adsReleaseIsland();
    }
}


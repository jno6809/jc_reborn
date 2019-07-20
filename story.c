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

#include "mytypes.h"
#include "utils.h"
#include "graphics.h"
#include "ttm.h"
#include "ads.h"
#include "island.h"
#include "config.h"
#include "story.h"


#define NUM_SCENES    48

#define FINAL         0x01
#define FIRST         0x02
#define ISLAND        0x04
#define LEFT_ISLAND   0x08
#define VARPOS_OK     0x10
#define LOWTIDE_OK    0x20
#define NORAFT        0x40
#define HOLIDAY_NOK   0x80

#define SPOT_x 0       // TODO - tempo
#define SPOT_A 0
#define SPOT_B 1
#define SPOT_C 2
#define SPOT_D 3
#define SPOT_E 4
#define SPOT_F 5


struct TStoryScene {
    char adsName[13];
    int  adsTagNo;
    int  spotStart;
    int  spotEnd;
    int  dayNo;
    int  flags;
};


static int storyCurrentDay = 1;

static struct TStoryScene storyScenes[NUM_SCENES] = {

//  { "ACTIVITY.ADS",  1,       0,      0,  0,  ISLAND                                                    },  // bugged, loops indefinitely, too
    { "ACTIVITY.ADS", 12,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },
    { "ACTIVITY.ADS", 11,       0,      0,  0,  ISLAND | FINAL | FIRST | VARPOS_OK                        },
    { "ACTIVITY.ADS", 10,  SPOT_D,      0,  0,  ISLAND | FINAL                                            },
//  { "ACTIVITY.ADS",  4,  SPOT_E, SPOT_E,  0,  FINAL | VARPOS_OK                                         },  // bugged, loops indefinitely
    { "ACTIVITY.ADS",  5,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },
    { "ACTIVITY.ADS",  6,  SPOT_D,      0,  0,  ISLAND | FINAL | VARPOS_OK                                },
    { "ACTIVITY.ADS",  7,  SPOT_D, SPOT_F,  0,  ISLAND | LOWTIDE_OK                                       },
    { "ACTIVITY.ADS",  8,       0, SPOT_D,  0,  ISLAND | FIRST                                            },
    { "ACTIVITY.ADS",  9,  SPOT_x,      0,  0,  ISLAND | FINAL | LOWTIDE_OK                               },

    { "BUILDING.ADS",  1,  SPOT_F, SPOT_A,  0,  ISLAND | LOWTIDE_OK                                       },
    { "BUILDING.ADS",  4,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },
    { "BUILDING.ADS",  3,  SPOT_A, SPOT_C,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "BUILDING.ADS",  2,  SPOT_F,      0,  0,  ISLAND | FINAL                                            },
    { "BUILDING.ADS",  5,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
//  { "BUILDING.ADS",  9,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
    { "BUILDING.ADS",  7,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
//  { "BUILDING.ADS",  8,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "BUILDING.ADS",  6,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },

    { "FISHING.ADS" ,  1,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "FISHING.ADS" ,  2,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK                                        },
    { "FISHING.ADS" ,  3,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },
    { "FISHING.ADS" ,  4,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL | LEFT_ISLAND                              },  // TODO : night NOK ?
    { "FISHING.ADS" ,  5,  SPOT_E,      0,  0,  ISLAND | FINAL | VARPOS_OK                                },
    { "FISHING.ADS" ,  6,  SPOT_D,      0,  0,  ISLAND | FINAL                                            },
    { "FISHING.ADS" ,  7,  SPOT_E, SPOT_E,  0,  ISLAND | LEFT_ISLAND | VARPOS_OK | LOWTIDE_OK             },
    { "FISHING.ADS" ,  8,  SPOT_E, SPOT_E,  0,  ISLAND | LEFT_ISLAND | VARPOS_OK | LOWTIDE_OK             },

    { "JOHNNY.ADS"  ,  1,       0,      0, 11,  FINAL | FIRST                                             },
    { "JOHNNY.ADS"  ,  2,  SPOT_E, SPOT_F,  2,  ISLAND | FINAL                                            },  // VARPOS_OK ?
    { "JOHNNY.ADS"  ,  3,  SPOT_x, SPOT_x,  6,  ISLAND                                                    },  // VARPOS_OK ?
    { "JOHNNY.ADS"  ,  4,  SPOT_E, SPOT_F,  0,  ISLAND | VARPOS_OK                                        },
    { "JOHNNY.ADS"  ,  5,  SPOT_E, SPOT_F,  0,  ISLAND | VARPOS_OK                                        },
    { "JOHNNY.ADS"  ,  6,       0,      0, 10,  FINAL | FIRST                                             },

    { "MARY.ADS"    ,  1,  SPOT_x, SPOT_x,  5,  ISLAND | FINAL                                            },
    { "MARY.ADS"    ,  3,  SPOT_x, SPOT_x,  4,  ISLAND | FINAL                                            },
    { "MARY.ADS"    ,  2,  SPOT_x, SPOT_x,  1,  ISLAND | FINAL                                            },
    { "MARY.ADS"    ,  4,  SPOT_x, SPOT_x,  7,  ISLAND | FINAL                                            },
    { "MARY.ADS"    ,  5,  SPOT_x, SPOT_x,  8,  ISLAND | LEFT_ISLAND | FINAL | FIRST | NORAFT | VARPOS_OK },

    { "MISCGAG.ADS" ,  1,  SPOT_x, SPOT_x,  0,  ISLAND | FINAL                                            },
    { "MISCGAG.ADS" ,  2,  SPOT_D,      0,  0,  ISLAND | FINAL                                            },

//  { "STAND.ADS"   ,  1,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  2,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  3,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  4,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  5,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  6,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  7,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  8,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   ,  9,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   , 10,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   , 11,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
//  { "STAND.ADS"   , 12,  SPOT_x, SPOT_x,  0,  ISLAND                                                    },
    { "STAND.ADS"   , 15,  SPOT_x, SPOT_x,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 16,  SPOT_x, SPOT_x,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },

    { "SUZY.ADS"    ,  1,       0,      0,  3,  FINAL | FIRST                                             },
    { "SUZY.ADS"    ,  2,       0,      0,  9,  FINAL | FIRST                                             },

    { "VISITOR.ADS" ,  1,  SPOT_A, SPOT_A,  0,  ISLAND | LOWTIDE_OK                                       },  // not VARPOS_OK
    { "VISITOR.ADS" ,  3,  SPOT_D, SPOT_D,  0,  ISLAND | FINAL | HOLIDAY_NOK                              },  // not VARPOS_OK
    { "VISITOR.ADS" ,  4,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK                                        },
    { "VISITOR.ADS" ,  6,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK                                        },
    { "VISITOR.ADS" ,  7,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "VISITOR.ADS" ,  5,  SPOT_D, SPOT_D,  0,  ISLAND | FINAL | LEFT_ISLAND                              },

    { "WALKSTUF.ADS",  1,  SPOT_A,      0,  0,  ISLAND | FINAL                                            },  // not VARPOS_OK
    { "WALKSTUF.ADS",  2,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK                                        },
    { "WALKSTUF.ADS",  3,  SPOT_D, SPOT_D,  0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
};


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
    islandState.night = !(rand() %3);

    // Holidays ?
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
}


void storyPlay()
{
    uint16 wantedFlags   = 0;
    uint16 unwantedFlags = 0;


    adsPlayIntro();

    while (1) {

        storyUpdateCurrentDay();
        storyCalculateIslandFromDateAndTime();
        unwantedFlags = 0;

        if (islandState.holiday)
            unwantedFlags |= HOLIDAY_NOK;

        struct TStoryScene *finalScene = storyPickScene(FINAL, unwantedFlags);

        if (finalScene->flags & ISLAND) {
            storyCalculateIslandFromScene(finalScene);
            adsInitIsland();
        }
        else {
            adsNoIsland();
        }

        if (!(finalScene->flags & FIRST)) {

            wantedFlags = 0;
            unwantedFlags |= FINAL;

            if (islandState.lowTide)
                wantedFlags |= LOWTIDE_OK;

            if (islandState.xPos || islandState.yPos)
                wantedFlags |= VARPOS_OK;

            for (int i=0; i < 2; i++) {  // TODO 6 + (rand() % 14)

                struct TStoryScene *scene = storyPickScene(wantedFlags,
                                                           unwantedFlags);

                ttmDx = islandState.xPos
                            + (scene->flags & LEFT_ISLAND ? 272 : 0);
                ttmDy = islandState.yPos;

                adsPlay(scene->adsName, scene->adsTagNo);

                unwantedFlags |= FIRST;
            }
        }

        ttmDx = islandState.xPos + (finalScene->flags & LEFT_ISLAND ? 272 : 0);
        ttmDy = islandState.yPos;

        adsPlay(finalScene->adsName, finalScene->adsTagNo);

        grFadeOut();
    }
}


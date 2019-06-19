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
#include <stdio.h>
#include <string.h>

#include "mytypes.h"
#include "utils.h"
#include "resource.h"
#include "dump.h"
#include "graphics.h"
#include "ttm.h"
#include "ads.h"
#include "story.h"


static int  argDump       = 0;
static int  argBench      = 0;
static int  argTtm        = 0;
static int  argAds        = 0;
static int  argPlayAll    = 0;
static int  argFullScreen = 0;
static int  argIsland     = 0;

static char *args[3];
static int  numArgs  = 0;


static void usage()
{
        printf("\n");
        printf(" Usage :\n");
        printf("         myscrantic\n");
        printf("         myscrantic help\n");
        printf("         myscrantic version\n");
        printf("         myscrantic dump\n");
        printf("         myscrantic [<options>] bench\n");
        printf("         myscrantic [<options>] ttm <TTM name>\n");
        printf("         myscrantic [<options>] ads <ADS name> <ADS tag no>\n");
        printf("\n");
        printf(" Available options are:\n");
        printf("         fs - play in full screen video mode\n");
        printf("         island - display the island as background for ADS play\n");
        printf("         debug - print some debug info on stdout\n");
        printf("\n");
        exit(1);
}


static void version()
{
        printf("\n");
        printf("    Johnny Reborn, an open-source engine for\n");
        printf("    the classic Johnny Castaway screensaver by Sierra.\n");
        printf("    Development version Copyright (C) 2019 Jeremie GUILLAUME\n");
        printf("\n");
        exit(1);
}


static void parseArgs(int argc, char **argv)
{
    int numExpectedArgs = 0;

    for (int i=0; i < argc; i++) {

        if (numExpectedArgs) {
            args[numArgs++] = argv[i];
            numExpectedArgs--;
        }
        else {
            if (!strcmp(argv[i], "help")) {
                usage();
            }
            if (!strcmp(argv[i], "version")) {
                version();
            }
            else if (!strcmp(argv[i], "dump")) {
                argDump = 1;
            }
            else if (!strcmp(argv[i], "bench")) {
                argBench = 1;
            }
            else if (!strcmp(argv[i], "ttm")) {
                argTtm = 1;
                numExpectedArgs = 1;
            }
            else if (!strcmp(argv[i], "ads")) {
                argAds = 1;
                numExpectedArgs = 2;
            }
            else if (!strcmp(argv[i], "island")) {
                argIsland = 1;
            }
            else if (!strcmp(argv[i], "fs")) {
                argFullScreen = 1;
            }
            else if (!strcmp(argv[i], "debug")) {
                debugMode = 1;
            }
        }
    }

    if (numExpectedArgs)
        usage();

    if (argDump + argBench + argTtm + argAds > 1)
        usage();

    if (argDump + argBench + argTtm + argAds == 0)
        argPlayAll = 1;
}


int main(int argc, char **argv)
{
    parseArgs(argc, argv);

    if (argDump)
        debugMode = 1;

    parseResourceFiles("RESOURCE.MAP");

    if (argPlayAll) {
        graphicsInit(argFullScreen);
        storyPlay();
        graphicsEnd();
    }

    else if (argDump) {
        dumpAllResources();
    }

    else if (argBench) {
        graphicsInit(argFullScreen);
        adsPlayBench();
        graphicsEnd();
    }

    else if (argTtm) {
        graphicsInit(argFullScreen);
        adsPlaySingleTtm(args[0]);
        graphicsEnd();
    }

    else if (argAds) {

        graphicsInit(argFullScreen);

        if (argIsland)
            adsInitIsland();
        else
            adsNoIsland();

        adsPlay(args[0], atoi(args[1]));

        graphicsEnd();
    }
}


#include <SDL2/SDL.h>

#include "mytypes.h"
#include "graphics.h"
#include "ticks.h"
#include "utils.h"
#include "walk.h"
#include "walk_data.h"


void walkPlay(struct TTtmThread *ttmThreads, int tag) {

    struct TTtmSlot *ttmSlot = ttmThreads[0].ttmSlot;
    SDL_Surface *sfc = ttmThreads[0].ttmLayer;

    int i = walk_data_bookmarks[tag];

    while (walkingData[i][1] != 0) {

        grClearScreen(sfc);

        uint16 *frameData = walkingData[i++];

        debugMsg("Walk data : %d %d %d %d",
            frameData[0], frameData[1], frameData[2], frameData[3]);

        if (frameData[0])
            grDrawSpriteFlip(sfc, ttmSlot, frameData[1] - 1, frameData[2], frameData[3], 0);
        else
            grDrawSprite(sfc, ttmSlot, frameData[1] - 1, frameData[2], frameData[3], 0);

        graphicsUpdate(NULL, ttmThreads, NULL);

printf("%3d -- (%3d,%3d), %s%d\n",
    i,
    frameData[1],
    frameData[2],
    (frameData[0] ? "-" : " "),
    frameData[3]);
char buffer[10];
fgets(buffer, 10, stdin);

//        ticksWait(12);
    }
}


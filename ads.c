#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <SDL2/SDL.h>

#include "mytypes.h"
#include "utils.h"
#include "resource.h"
#include "graphics.h"
#include "ticks.h"
#include "ttm.h"
#include "island.h"
#include "walk.h"
#include "bench.h"
#include "ads.h"

#define MAX_RANDOM_SCENES  10
#define MAX_ADS_CHUNKS     100


struct TAdsChunk {   // TODO should not be here
    struct TAdsScene scene;
    uint32 offset;
};


static struct TAdsChunk adsChunks[MAX_ADS_CHUNKS];
static int    numAdsChunks;

static struct TTtmSlot ttmBackgroundSlot;
static struct TTtmSlot ttmHolidaySlot;
static struct TTtmSlot ttmSlots[MAX_TTM_SLOTS];

static struct TTtmThread ttmBackgroundThread;
static struct TTtmThread ttmHolidayThread;
static struct TTtmThread ttmThreads[MAX_TTM_THREADS];

static struct TAdsChunk adsChunks[MAX_ADS_CHUNKS];

static struct TTtmTag *adsTags;
static int    adsNumTags = 0;

static struct TAdsScene adsRandomScenes[MAX_RANDOM_SCENES];
static int    adsNumRandomScenes = 0;

static int    numThreads       = 0;
static int    adsStopRequested = 0;


static void adsLoad(uint8 *data, uint32 dataSize, uint16 numTags, uint16 tag, uint32 *tagOffset)
{
    uint32 offset = 0;
    uint16 args[10];
    int bookmarkingChunks = 0;

    numAdsChunks = 0;
    *tagOffset   = 0;
    adsNumTags   = 0;
    adsTags = safe_malloc(numTags * sizeof(struct TTtmTag));

    while (offset < dataSize) {

        uint16 opcode = peekUint16(data, &offset);

        switch (opcode) {

            case 0x1350:     // IF_LASTPLAYED
                if (bookmarkingChunks) {
                    peekUint16Block(data, &offset, args, 2);
                    adsChunks[numAdsChunks].scene.slot = args[0];
                    adsChunks[numAdsChunks].scene.tag  = args[1];
                    adsChunks[numAdsChunks].offset     = offset;
                    numAdsChunks++;
                }
                else {
                    offset += 2<<1;
                }
                break;

            case 0x1070: offset += 2<<1; break;
            case 0x1330: offset += 2<<1; break;
            case 0x1360: offset += 2<<1; break;
            case 0x1370: offset += 2<<1; break;
            case 0x1420: offset += 0<<1; break;
            case 0x1430: offset += 0<<1; break; // OR   // TODO : manage here if_lastplayed OK tags ?
            case 0x1510: offset += 0<<1; break;
            case 0x1520: offset += 5<<1; break;
            case 0x2005: offset += 4<<1; break;
            case 0x2010: offset += 3<<1; break;
            case 0x2014: offset += 0<<1; break;
            case 0x3010: offset += 0<<1; break;
            case 0x3020: offset += 1<<1; break;
            case 0x30ff: offset += 0<<1; break;
            case 0x4000: offset += 3<<1; break;
            case 0xf010: offset += 0<<1; break;
            case 0xf200: offset += 1<<1; break;
            case 0xffff: offset += 0<<1; break;
            case 0xfff0: offset += 0<<1; break;

            default:
                adsTags[adsNumTags].id     = opcode;
                adsTags[adsNumTags].offset = offset;
                adsNumTags++;

                if (opcode == tag) {
                    *tagOffset = offset;
                    bookmarkingChunks = 1;
                }
                else {
                    bookmarkingChunks = 0;
                }

                break;
        }
    }

    if (adsNumTags != numTags)
        debugMsg("Warning : didn't find every tag in ADS data");

    if (*tagOffset == 0)
        debugMsg("Warning : ADS tag #%d not found, starting from offset 0", tag);
}


static void adsReleaseAds()
{
    free(adsTags);
}


static uint32 adsFindTag(uint16 reqdTag)
{
    uint32 result = 0;
    int i = 0;

    while (result == 0 && i < adsNumTags) {

        if (adsTags[i].id == reqdTag)
            result = adsTags[i].offset;
        else
            i++;
    }

    if (result == 0)
        fprintf(stderr, "Warning : ADS tag #%d not found, returning offset 0000\n", reqdTag);

    return result;
}


static void adsAddScene(uint16 ttmSlotNo, uint16 ttmTag, uint16 arg3)
{
    for (int i=0; i < MAX_TTM_THREADS; i++) {

        struct TTtmThread *ttmThread = &ttmThreads[i];

        if (ttmThread->isRunning) {

            if (ttmThread->sceneSlot == ttmSlotNo && ttmThread->sceneTag == ttmTag) {
                debugMsg("(%d,%d) thread is already running - didn't add extra one\n", ttmSlotNo, ttmTag);
                return;
            }
        }
    }
    
    int i=0;

    while (ttmThreads[i].isRunning)
        i++;

    struct TTtmThread *ttmThread = &ttmThreads[i]; //struct TTtmThread *ttmThread = ttmThreads + i;

    ttmThread->ttmSlot         = &ttmSlots[ttmSlotNo];
    ttmThread->isRunning       = 1;
    ttmThread->sceneSlot       = ttmSlotNo;
    ttmThread->sceneTag        = ttmTag;
    ttmThread->sceneTimer      = 0;
    ttmThread->sceneIterations = 0;
    ttmThread->delay           = 2;
    ttmThread->timer           = 0;
    ttmThread->nextGotoOffset  = 0;
    ttmThread->selectedBmpSlot = 0;
    ttmThread->fgColor         = 0x0f;
    ttmThread->bgColor         = 0x0f;

    if (ttmSlotNo)
        ttmThread->ip = ttmFindTag(&ttmSlots[ttmSlotNo], ttmTag);
    else
        ttmThread->ip = 0;

    if (((short)arg3) < 0) {
        ttmThread->sceneTimer = -((short)arg3);
    }
    else if (((short)arg3) > 0) {
        ttmThread->sceneIterations = arg3 - 1;
    }

    ttmThread->ttmLayer = grNewLayer();

    numThreads++;
}


static void adsStopScene(int sceneNo)
{
    grFreeLayer(ttmThreads[sceneNo].ttmLayer);
    ttmThreads[sceneNo].isRunning = 0;
    numThreads--;
}


static void adsStopAllScenes()
{
    for (int i=0; i < MAX_TTM_THREADS; i++)
        if (ttmThreads[i].isRunning)
            adsStopScene(i);
}


static void adsStopSceneByTtmTag(uint16 ttmSlotNo, uint16 ttmTag)
{
    struct TTtmThread *ttmThread;

    for (int i=0; i < MAX_TTM_THREADS; i++) {

        ttmThread = &ttmThreads[i];

        if (ttmThread->isRunning) {

            if (ttmThread->sceneSlot == ttmSlotNo && ttmThread->sceneTag == ttmTag)
                adsStopScene(i);
        }
    }
}


static int isSceneRunning(uint16 ttmSlotNo, uint16 ttmTag)
{
    for (int i=0; i < MAX_TTM_THREADS; i++) {

        struct TTtmThread thread = ttmThreads[i];

        if (    thread.isRunning == 1
             && thread.sceneSlot == ttmSlotNo
             && thread.sceneTag  == ttmTag    ) {
            return 1;
        }
    }

    return 0;
}


static void adsRandomStart()
{
    // TODO : maybe stop any previously running thread ?
    adsNumRandomScenes = 0;
}


static void adsRandomAddScene(uint16 ttmSlotNo, uint16 ttmTag)
{
   adsRandomScenes[adsNumRandomScenes].slot = ttmSlotNo;
   adsRandomScenes[adsNumRandomScenes].tag  = ttmTag;
   adsNumRandomScenes++;
}


static void adsRandomEnd()
{
   if (adsNumRandomScenes) {
       int i = rand() % adsNumRandomScenes;
       debugMsg("RANDOM : chose scene %d %d", adsRandomScenes[i].slot, adsRandomScenes[i].tag);
       adsAddScene(adsRandomScenes[i].slot, adsRandomScenes[i].tag, adsRandomScenes[i].numPlays);
   }
   else {
       debugMsg("RANDOM : no scene to choose from");
   }
}


void adsInit()    // Init slots and threads for TTM scripts  // TODO : rename
{
    for (int i=0; i < MAX_TTM_SLOTS; i++)
        ttmInitSlot(&ttmSlots[i]);

    for (int i=0; i < MAX_TTM_THREADS; i++) {
        ttmThreads[i].isRunning = 0;
        ttmThreads[i].timer     = 0;
    }

    numThreads = 0;
    adsStopRequested = 0;
}


void adsPlaySingleTtm(char *ttmName)  // TODO - tempo
{
    adsInit();
    ttmLoadTtm(ttmSlots, ttmName);
    adsAddScene(0,0,0);
    ttmThreads[0].ip = 0;
    ticksInit();

    while (ttmThreads[0].ip < ttmSlots[0].dataSize) {
        ttmPlay(ttmThreads);
        ttmThreads[0].isRunning = 1;
        graphicsUpdate(NULL, ttmThreads, NULL);
        ticksWait(ttmThreads[0].delay);
    }

    adsStopScene(0);
    ttmResetSlot(ttmSlots);
}


static void adsPlayChunk(uint8 *data, uint32 dataSize, uint32 offset)
{
    uint16 opcode;
    uint16 args[10];
    int inRandBlock   = 0;
    int inOrBlock     = 0;
    int inSkipBlock   = 0;
    int continueLoop  = 1;


    while (continueLoop && offset < dataSize) {

        opcode = peekUint16(data, &offset);

        switch (opcode) {

            case 0x1070:
                peekUint16Block(data, &offset, args, 2);
                debugMsg("UNKNOWN_0");
                break;

            case 0x1330:
                // always just before a call to ADD_SCENE with same (ttm,tag)
                // references tags which init commands : LOAD_IMAGE LOAD_SCREEN etc.
                //   - one exception: FISHING.ADS tag 3
                //   - seems to be a synonym of "IF_NOT_RUNNING"
                //   - if so, our implementation works fine anyway by ignoring this one...
                peekUint16Block(data, &offset, args, 2);
                debugMsg("IF_UNKNOWN_1 %d %d", args[0], args[1]);
                break;

            case 0x1350:
                peekUint16Block(data, &offset, args, 2);
                debugMsg("IF_LASTPLAYED %d %d", args[0], args[1]);

                if (!inOrBlock)
                    continueLoop = 0;

                inOrBlock = 0;

                break;

            case 0x1360:
                peekUint16Block(data, &offset, args, 2);
                debugMsg("IF_SKIP_NEXT2 %d %d", args[0], args[1]);
                inSkipBlock = 1;
                break;

            case 0x1370:
                peekUint16Block(data, &offset, args, 2);
                debugMsg("IF_IS_RUNNING %d %d", args[0], args[1]);
                inSkipBlock = ! isSceneRunning(args[0], args[1]);
                break;

            case 0x1420:
                debugMsg("OR_UNKNOWN_3");
                break;

            case 0x1430:
                debugMsg("OR");
                inOrBlock = 1;
                break;

            case 0x1510:
                debugMsg("PLAY_SCENE");
                if (inSkipBlock)
                    inSkipBlock = 0;    // TODO ?
                else
                    continueLoop = 0;
                break;

            case 0x1520:
                peekUint16Block(data, &offset, args, 5);
                debugMsg("PLAY_SCENE_2");
                break;

            case 0x2005:
                peekUint16Block(data, &offset, args, 4);
                debugMsg("ADD_SCENE %d %d %d %d", args[0], args[1], args[2], args[3]);

                if (inRandBlock)
                    adsRandomAddScene(args[0],args[1]);
                else if (!inSkipBlock)                // TODO - TEMPO
                    adsAddScene(args[0],args[1],args[2]);

                break;

            case 0x2010:
                peekUint16Block(data, &offset, args, 3);
                debugMsg("STOP_SCENE %d %d %d", args[0], args[1], args[2]);
                adsStopSceneByTtmTag(args[0], args[1]);
                break;

            case 0x3010:
                debugMsg("RANDOM_START");
                adsRandomStart();
                inRandBlock = 1;
                break;

            case 0x3020:
                peekUint16Block(data, &offset, args, 1);
                debugMsg("RANDOM_UNKNOWN_0");   // cf. ACTIVITY.ADS:1 , = when swimming back to island
                break;

            case 0x30ff:
                debugMsg("RANDOM_END");
                adsRandomEnd();
                inRandBlock = 0;
                break;

            case 0x4000:
                peekUint16Block(data, &offset, args, 3);
                debugMsg("UNKNOWN_6");    // only in BUILDING.ADS tag 7
                break;

            case 0xf010:
                debugMsg("FADE_OUT");
                break;

            case 0xf200:
                peekUint16Block(data, &offset, args, 1);
                debugMsg("GOSUB_TAG");    // ex UNKNOWN_8
                // "quick and dirty" implementation, sufficient for
                // JCastaway : only encountered in STAND.ADS to tag 14
                // which only contains 1 scene
                adsPlayChunk(data, dataSize, adsFindTag(args[0]));
                break;

            case 0xffff:
                debugMsg("END");

                if (inSkipBlock)     // TODO - no doubt this is q&d
                    inSkipBlock = 0;
                else
                    adsStopRequested = 1;

                break;

            case 0xfff0:
                debugMsg("END_IF");
                break;

            default :
                debugMsg(":TAG_%d\n", opcode);
                break;

        }
    }
}


static void adsPlayTriggeredChunks(uint8 *data, uint32 dataSize, uint16 ttmSlotNo, uint16 ttmTag)
{
    // Note : in a few rare cases (eg BUILDING.ADS tag2), the ADS script
    // contains several 'IF_LASTPLAYED' commands for one given scene.

    for (int i=0; i < numAdsChunks; i++) 
        if (adsChunks[i].scene.slot == ttmSlotNo && adsChunks[i].scene.tag == ttmTag)
            adsPlayChunk(data, dataSize, adsChunks[i].offset);
}


void adsPlay(char *adsName, uint16 adsTag)
{
    uint32 offset;
    uint8  *data;
    uint32 dataSize;

    struct TAdsResource *adsResource = findAdsResource(adsName);

    debugMsg("\n\n========== Playing ADS: %s:%d ==========\n", adsResource->resName, adsTag);

    data = adsResource->uncompressedData;
    dataSize = adsResource->uncompressedSize;

    for (int i=0; i < adsResource->numRes; i++)
        ttmLoadTtm(&ttmSlots[adsResource->res[i].id], adsResource->res[i].name);

    adsLoad(data, dataSize, adsResource->numTags, adsTag, &offset);

    ticksInit();
    adsStopRequested = 0;

    // Play the first ADS chunk of the sequence
    adsPlayChunk(data, dataSize, offset);

    // Main ADS loop
    while (numThreads) {

        if (ttmBackgroundThread.isRunning && ttmBackgroundThread.timer == 0) {
            ttmBackgroundThread.timer = ttmBackgroundThread.delay;
            debugMsg("    ------> Animate bg");
            islandAnimate(&ttmBackgroundThread);
        }

        for (int i=0; i < MAX_TTM_THREADS; i++) {

            // Call ttmPlay() for each thread which timer reaches 0
            if (ttmThreads[i].isRunning && ttmThreads[i].timer == 0) {
                debugMsg("    ------> Thread #%d", i);
                ttmThreads[i].timer = ttmThreads[i].delay;
                ttmPlay(&ttmThreads[i]);
            }
        }

        if (debugMode) {

            debugMsg("\n  +------ THREADS: %d -------", numThreads);

            if (ttmBackgroundThread.isRunning) {
                    debugMsg("  |");
                    debugMsg("  | #bg:");
                    debugMsg("  |   delay........... %d" , ttmBackgroundThread.delay    );
                    debugMsg("  |   timer........... %d" , ttmBackgroundThread.timer    );
            }

            for (int i=0; i < MAX_TTM_THREADS; i++) {
                if (ttmThreads[i].isRunning) {
                    debugMsg("  |");
                    debugMsg("  | #%d: (%d,%d)", i, ttmThreads[i].sceneSlot, ttmThreads[i].sceneTag);
                    debugMsg("  |   sceneTimer...... %d" , ttmThreads[i].sceneTimer     );
                    debugMsg("  |   isRunning....... %d" , ttmThreads[i].isRunning      );
                    debugMsg("  |   offset.......... %ld", ttmThreads[i].ip             );
                    debugMsg("  |   nextGotoOffset.. %ld", ttmThreads[i].nextGotoOffset );
                    debugMsg("  |   delay........... %d" , ttmThreads[i].delay          );
                    debugMsg("  |   timer........... %d" , ttmThreads[i].timer          );
                }
            }

            debugMsg("  +-------------------------\n");
        }

        // Refresh display
        graphicsUpdate(&ttmBackgroundThread, ttmThreads, &ttmHolidayThread);

        // Determine min timer through all threads
        uint16 mini = 300;

        if (ttmBackgroundThread.isRunning)
            mini = ttmBackgroundThread.timer;

        for (int i=0; i < MAX_TTM_THREADS; i++) {

            if (ttmThreads[i].isRunning) {

                if (ttmThreads[i].delay < mini)
                    mini = ttmThreads[i].delay;

                if (ttmThreads[i].timer < mini)
                    mini = ttmThreads[i].timer;
            }
        }

        // Decrease all timers by the shortest one, and wait that amount of time
        ttmBackgroundThread.timer -= mini;

        for (int i=0; i < MAX_TTM_THREADS; i++)
            if (ttmThreads[i].isRunning)
                ttmThreads[i].timer -= mini;

        debugMsg(" ******* WAIT: %d ticks *******\n", mini);
        ticksWait(mini);

        // Various threads processes
        for (int i=0; i < MAX_TTM_THREADS; i++) {

            if (ttmThreads[i].isRunning && ttmThreads[i].timer == 0) {

                // Process jumps
                if (ttmThreads[i].nextGotoOffset) {
                    ttmThreads[i].ip = ttmThreads[i].nextGotoOffset;
                    ttmThreads[i].nextGotoOffset = 0;
                }

                // Managing the timer which was indicated in ADD_SCENE arg3 (neg. value)
                if (ttmThreads[i].sceneTimer > 0) {
                    ttmThreads[i].sceneTimer -= ttmThreads[i].delay;
                    if (ttmThreads[i].sceneTimer <= 0)
                        ttmThreads[i].isRunning = 2;
                }

                // Free terminated threads
                if (ttmThreads[i].isRunning == 2) {

                    // END encountered ?
                    if (adsStopRequested) {
                        adsStopAllScenes();
                    }

                    // Managing the numPlays which was indicated in ADD_SCENE arg3 (postive value)
                    else if (ttmThreads[i].sceneIterations) {
                        ttmThreads[i].sceneIterations--;
                        ttmThreads[i].isRunning = 1;
                        ttmThreads[i].ip = ttmFindTag(&ttmSlots[ttmThreads[i].sceneSlot], ttmThreads[i].sceneTag);
                    }

                    // Is there one (or more) IF_LASTPLAYED matching the terminated thread ?
                    else {
                        adsPlayTriggeredChunks(data, dataSize, ttmThreads[i].sceneSlot, ttmThreads[i].sceneTag);
                        adsStopScene(i);
                    }
                }
            }
        }
    }

    for (int i=0; i < MAX_TTM_SLOTS; i++)
        ttmResetSlot(&ttmSlots[i]);

    adsReleaseAds();
}


void adsPlayBench()  // TODO - tempo
{

    int numsLayers[] = { 1, 4, 8 };

    uint32 startTicks, counter;

    adsInit();
    benchInit(ttmSlots);

    for (int j=0; j < 3 ; j++) {

        int numLayers = numsLayers[j];

        for (int i=0; i < numLayers; i++)
            adsAddScene(0,0,0);

        startTicks = SDL_GetTicks();
        counter = 0;

        while ((SDL_GetTicks() - startTicks) <= 3000) {

            for (int i=0; i < numLayers; i++)
                benchPlay(&ttmThreads[i], i);

            graphicsUpdate(NULL, ttmThreads, NULL);

            counter++;
        }

        adsStopAllScenes();

        printf(" %d-layers test --> %ld fps\n", numLayers, counter/3);
    }

    ttmResetSlot(ttmSlots);
}


void adsPlayIntro()
{
    adsInit();
    grLoadScreen("INTRO.SCR");
    ticksInit();
    graphicsUpdate(NULL, ttmThreads, NULL);
    ticksWait(100);
    grFadeOut();
    ttmResetSlot(ttmSlots);
}


void adsInitIsland()
{
    // Init the background thread (animated waves)
    // and call ttmInitIsland() to draw the background

    ttmInitSlot(&ttmBackgroundSlot);

    ttmBackgroundThread.ttmSlot   = &ttmBackgroundSlot;
    ttmBackgroundThread.isRunning = 3;
    ttmBackgroundThread.delay     = 40;  // TODO
    ttmBackgroundThread.timer     = 0;

    islandInit(&ttmBackgroundThread);


    // Init the "holiday" layer and thread

    ttmInitSlot(&ttmHolidaySlot);

    ttmHolidayThread.ttmSlot   = &ttmHolidaySlot;
    ttmHolidayThread.isRunning = 3;
    ttmHolidayThread.delay     = 0;
    ttmHolidayThread.timer     = 0;

    ttmHolidayThread.ttmLayer  = grNewLayer();

    islandInitHoliday(&ttmHolidayThread);
}


void adsNoIsland()
{
    grDx = grDy = 0;

    ttmBackgroundThread.isRunning = 0;
    ttmResetSlot(&ttmBackgroundSlot);
    grInitEmptyBackground();

    ttmHolidayThread.isRunning = 0;
    grFreeLayer(ttmHolidayThread.ttmLayer);
}


void adsPlayWalk()
{
    adsInit();
    adsAddScene(0,0,0);
    grLoadBmp(ttmSlots, 0, "JOHNWALK.BMP");
    ticksInit();

    walkPlay(ttmThreads, 3);
    walkPlay(ttmThreads, 7);
    walkPlay(ttmThreads, 12);
    walkPlay(ttmThreads, 18);
    walkPlay(ttmThreads, 25);
    walkPlay(ttmThreads, 28);

    adsStopScene(0);
}


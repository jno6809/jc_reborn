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

#include <stdio.h>

#include "mytypes.h"
#include "utils.h"
#include "resource.h"
#include "graphics.h"
#include "sound.h"
#include "ttm.h"


int ttmDx = 0;
int ttmDy = 0;


static uint32 ttmFindPreviousTag(struct TTtmSlot *ttmSlot, uint32 offset)
{
    uint32 result = 0;
    int i = 0;

    while (ttmSlot->tags[i].offset < offset) {
        result = ttmSlot->tags[i].offset;
        i++;
    }

    return result;
}


uint32 ttmFindTag(struct TTtmSlot *ttmSlot, uint16 reqdTag)
{
    uint32 result = 0;
    int i = 0;

    while (result == 0 && i < ttmSlot->numTags) {

        if (ttmSlot->tags[i].id == reqdTag)
            result = ttmSlot->tags[i].offset;
        else
            i++;
    }

    if (result == 0)
        fprintf(stderr, "Warning : TTM tag #%d not found, returning offset 0000\n", reqdTag);

    return result;
}


void ttmLoadTtm(struct TTtmSlot *ttmSlot, char *ttmName)     // TODO
{
    struct TTtmResource *ttmResource = findTtmResource(ttmName);

    debugMsg("---- Loading %s", ttmResource->resName);

    ttmSlot->data     = ttmResource->uncompressedData;
    ttmSlot->dataSize = ttmResource->uncompressedSize;
    ttmSlot->numTags  = ttmResource->numTags;
    ttmSlot->tags     = safe_malloc(ttmSlot->numTags * sizeof(struct TTtmTag));

    // we have to bookmark every tag for later jumps
    uint32 offset=0;
    int tagNo = 0;

    while (offset < ttmSlot->dataSize) {

        uint16 opcode = peekUint16(ttmSlot->data, &offset);

        if (opcode == 0x1111 || opcode == 0x1101) {
            uint16 arg = peekUint16(ttmSlot->data, &offset);
            ttmSlot->tags[tagNo].id     = arg;
            ttmSlot->tags[tagNo].offset = offset;
            tagNo++; // TODO
        }
        else {

            uint8 numArgs = (uint8) opcode & 0x000f;

            if (numArgs == 0x0f) {
                while (ttmSlot->data[offset] != 0 && ttmSlot->data[offset+1] != 0)
                    offset += 2;
                offset += 2;
            }
            else {
                offset += (numArgs << 1);
            }
        }
    }

    // TODO : in SASKDATE.TTM, num SET_SCENE != ttmResource->numTags
    while (tagNo < ttmSlot->numTags)
        ttmSlot->tags[tagNo++].id = 0;  // TODO is this useful ?
}


void ttmInitSlot(struct TTtmSlot *ttmSlot)
{
    for (int i=0; i < MAX_BMP_SLOTS; i++) {
        ttmSlot->data          = NULL;
        ttmSlot->numSprites[i] = 0;
    }
}


void ttmResetSlot(struct TTtmSlot *ttmSlot)
{
    if (ttmSlot->data != NULL) {
        ttmSlot->data = NULL;
        free(ttmSlot->tags);
    }

    for (int i=0; i < MAX_BMP_SLOTS; i++) {
        if (ttmSlot->numSprites[i])
            grReleaseBmp(ttmSlot, i);
    }
}


void ttmPlay(struct TTtmThread *ttmThread)     // TODO
{
    uint8 *data;
    uint32 offset;
    uint16 opcode;
    uint8 numArgs;
    uint16 args[10];
    char strArg[20];
    int continueLoop = 1;
    struct TTtmSlot *ttmSlot;


    grDx = ttmDx;
    grDy = ttmDy;

    ttmSlot = ttmThread->ttmSlot;
    offset = ttmThread->ip;
    data = ttmSlot->data;

    while (continueLoop) {

        opcode = peekUint16(data, &offset);

        numArgs = (uint8) opcode & 0x0000f;

        if (numArgs == 0x0f) {        // arg is a string

            int i=0;

            while (data[offset] != 0)
                strArg[i++] = data[offset++];

            strArg[i++] = data[offset++];

            if ((i & 0x01) == 0x01)   // always read an even number of uint8s
                strArg[i++] = data[offset++];   // TODO
        }
        else {                        // args are numArgs words
            peekUint16Block(data, &offset, args, numArgs);
        }

        switch (opcode) {

            case 0x0080:
                debugMsg("    DRAW_BACKGROUND");
                // Free images slots - see for example tag 11 of GFFFOOD.TTM
                break;

            case 0x0110:
                debugMsg("    PURGE");
                if (ttmThread->sceneTimer)
                    ttmThread->nextGotoOffset = ttmFindPreviousTag(ttmSlot, offset);
                else
                    ttmThread->isRunning = 2;
                break;

            case 0x0FF0:
                debugMsg("    UPDATE");
                continueLoop = 0;
                break;

            case 0x1021:
                debugMsg("    SET_DELAY %d", args[0]);
                ttmThread->timer = ttmThread->delay = (args[0] > 4 ? args[0] : 4);  // TODO ?
                break;

            case 0x1051:
                debugMsg("    SET_BMP_SLOT %d", args[0]);
                ttmThread->selectedBmpSlot = args[0];
                break;

            case 0x1061:
                debugMsg("    SET_PALETTE_SLOT %d", args[0]);
                break;

            case 0x1101:
                debugMsg("    :LOCAL_TAG %d", args[0]);
                break;

            case 0x1111:
                debugMsg("\n    :TAG %d ------------------------", args[0]);
                break;

            case 0x1121:
                // is called before SAVE_IMAGE1, defines the id of the region
                // for further use by CLEAR_SCREEN
                // (see WOULDBE.TTM for a nice example)
                debugMsg("    TTM_UNKNOWN_1 %d", args[0]);
                break;

            case 0x1201:
                // ex TTM_UNKNOWN_2
                debugMsg("    GOTO_TAG %d", args[0]);
                ttmThread->nextGotoOffset = ttmFindTag(ttmSlot, args[0]);
                break;

            case 0x2002:
                debugMsg("    SET_COLORS %d %d", args[0], args[1]);
                ttmThread->fgColor = args[0];
                ttmThread->bgColor = args[1];
                break;

            case 0x2012:
                // args always == (0,0)
                // at beginning of scenes, near LOAD_IMAGEs
                debugMsg("    SET_FRAME1 %d %d", args[0], args[1]);
                break;

            case 0x2022:
                debugMsg("    TIMER %d %d", args[0], args[1]);
                // Really, really not sure about this formula... but things
                // do work not so bad like that
                ttmThread->delay = ttmThread->timer = (args[0] + args[1]) / 2;
                break;

            case 0x4004:
                debugMsg("    SET_CLIP_ZONE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grSetClipZone(ttmThread->ttmLayer, args[0], args[1], args[2], args[3]);
                break;

            case 0x4204:
                debugMsg("    COPY_ZONE_TO_BG %d %d %d %d", args[0], args[1], args[2], args[3]);
                grCopyZoneToBg(ttmThread->ttmLayer, args[0], args[1], args[2], args[3]);
                break;

            case 0x4214:
                // defines the zone to be redrawn at each update ?
                // but seems not used in the original
                debugMsg("    SAVE_IMAGE1 %d %d %d %d", args[0], args[1], args[2], args[3]);
                grSaveImage1(ttmThread->ttmLayer, args[0], args[1], args[2], args[3]);
                break;

            case 0xA002:
                debugMsg("    DRAW_PIXEL %d %d", args[0], args[1]);
                grDrawPixel(ttmThread->ttmLayer, args[0], args[1], ttmThread->fgColor);
                break;

            case 0xA054:
                // only once, in GJGULIVR.TTM.txt
                debugMsg("    SAVE_ZONE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grSaveZone(ttmThread->ttmLayer, args[0], args[1], args[2], args[3]);
                break;

            case 0xA064:
                // only once, in GJGULIVR.TTM.txt
                debugMsg("    RESTORE_ZONE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grRestoreZone(ttmThread->ttmLayer, args[0], args[1], args[2], args[3]);
                break;

            case 0xA0A4:
                debugMsg("    DRAW_LINE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grDrawLine(ttmThread->ttmLayer, args[0], args[1], args[2], args[3], ttmThread->fgColor);
                break;

            case 0xA104:
                debugMsg("    DRAW_RECT %d %d %d %d", args[0], args[1], args[2], args[3]);
                grDrawRect(ttmThread->ttmLayer, args[0], args[1], args[2], args[3], ttmThread->fgColor);
                break;

            case 0xA404:
                debugMsg("    DRAW_CIRCLE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grDrawCircle(ttmThread->ttmLayer, args[0], args[1], args[2], args[3], ttmThread->fgColor, ttmThread->bgColor);
                break;

            case 0xA504:
                debugMsg("    DRAW_SPRITE %d %d %d %d", args[0], args[1], args[2], args[3]);
                grDrawSprite(ttmThread->ttmLayer, ttmThread->ttmSlot, args[0], args[1], args[2], args[3]);
                break;

            case 0xA524:
                debugMsg("    DRAW_SPRITE_FLIP %d %d %d %d", args[0], args[1], args[2], args[3]);
                grDrawSpriteFlip(ttmThread->ttmLayer, ttmThread->ttmSlot, args[0], args[1], args[2], args[3]);
                break;

            case 0xA601:
                // arg : indicates the SAVE_IMAGE1 nb to be used ?
                debugMsg("    CLEAR_SCREEN %d", args[0]);
                grClearScreen(ttmThread->ttmLayer);
                break;

            case 0xB606:
                debugMsg("    DRAW_SCREEN %d %d %d %d %d %d", args[0], args[1], args[2], args[3], args[4], args[5]);
                break;

            case 0xC051:
                debugMsg("    PLAY_SAMPLE %d", args[0]);
                soundPlay(args[0]);
                break;

            case 0xF01F:
                debugMsg("    LOAD_SCREEN %s", strArg);
                grLoadScreen(strArg);
                break;

            case 0xF02F:
                debugMsg("    LOAD_IMAGE %s", strArg);
                grLoadBmp(ttmSlot, ttmThread->selectedBmpSlot, strArg);
                break;

            case 0xF05F:
                debugMsg("    LOAD_PALETTE %s", strArg);
                break;
        }

        if (offset >= ttmSlot->dataSize) {
            ttmThread->isRunning = 2;
            continueLoop = 0;
        }
    }

    ttmThread->ip = offset;
}


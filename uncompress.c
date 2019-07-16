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

#include "mytypes.h"
#include "utils.h"

static int nextbit;
static uint8 current;
static uint32 inOffset;
static uint32 maxInOffset;

struct TCodeTableEntry {
    uint16 prefix;
    uint8 append;
};


static uint8 getByte(FILE *f)
{
    if (inOffset >= maxInOffset) {
        return 0;
    }
    else {
        inOffset++;
        uint8 b = (uint8) fgetc(f);
        return b;
    }
}


static uint16 getBits(FILE *f, uint32 n)
{
    if (n == 0)
        return 0;

    uint32 x = 0;

    for (uint32 i=0; i < n; i++) {
        if (current & (1 << nextbit))
            x |= (uint32) (1 << i);

        nextbit++;

        if (nextbit > 7) {
            current = (uint8) getByte(f);
            nextbit = 0;
        }
    }

    return x;
}


uint8 *uncompressLZW(FILE *f, uint32 inSize, uint32 outSize)
{
    uint8  *outData;
    struct TCodeTableEntry codeTable[4096];
    uint8  decodeStack[4096];
    uint32 stackPtr = 0;
    uint8  n_bits = 9;
    uint32 free_entry = 257;
    uint16 oldcode;
    uint16 lastbyte;
    uint32 bitpos = 0;
    uint32 outOffset = 0;


    if (outSize == 0)
        fatalError("uncompressLZW() : can't uncompress to 0 bytes\n");

    maxInOffset = inSize;
    nextbit     = 0;
    inOffset    = 0;
    outData     = safe_malloc(outSize * sizeof(uint8));

    current  = (uint8) getByte(f);
    lastbyte = oldcode = getBits(f, n_bits);

    outData[outOffset++] = (uint8) oldcode;

    while (inOffset < inSize) {

        uint16 newcode = getBits(f, n_bits);
        bitpos += n_bits;

        if (newcode == 256) {

            uint32 nbits3 = n_bits << 3;
            uint32 nskip = (nbits3 - ((bitpos - 1) % nbits3)) - 1;
            getBits(f, nskip);
            n_bits = 9;
            free_entry = 256;
            bitpos = 0;
        }
        else {

            uint16 code = newcode;

            if (code >= free_entry) {

                if (stackPtr > 4095)
                    break;

                decodeStack[stackPtr] = (uint8) lastbyte;
                stackPtr++;
                code = oldcode;
            }

            while (code > 255) {

                if (code > 4095)
                    break;

                decodeStack[stackPtr] = codeTable[code].append;  // TODO stackPtr >= 4096 ?
                stackPtr++;
                code = codeTable[code].prefix;
            }

            decodeStack[stackPtr] = (uint8) code;  // TODO stackPtr >= 4096 ?
            stackPtr++;
            lastbyte = code;

            while (stackPtr > 0) {

                stackPtr--;

                if (outOffset >= outSize)
                    return outData;

                outData[outOffset++] = decodeStack[stackPtr];
            }

            if (free_entry < 4096) {

                codeTable[free_entry].prefix = (uint16)oldcode;
                codeTable[free_entry].append = (uint8)lastbyte;
                free_entry++;
                uint32 temp = 1 << n_bits;

                if (free_entry >= temp && n_bits < 12) {
                    n_bits++;
                    bitpos = 0;
                }
            }

            oldcode = newcode;
        }
    }

    if (inOffset != inSize)
        fatalError("error while uncompressing LZW");

    return outData;
}


uint8 *uncompressRLE(FILE *f, uint32 inSize, uint32 outSize)
{
    uint8 *outData;
    uint32 outOffset = 0;

    inOffset    = 0;
    maxInOffset = inSize;

    outData = safe_malloc(outSize * sizeof(uint8));

    while (outOffset < outSize) {

        uint8 control = (uint8) fgetc(f);
        inOffset++;

        if ((control & 0x80) == 0x80) {
            uint8 length = control & 0x7F;
            uint8 b = (uint8) fgetc(f);
            inOffset++;

            for (int i=0; i < length; i++)
                outData[outOffset++] = b;    // TODO outOffset > outSize ?
        }
        else {
            for (int i=0;  i < control; i++) {
                outData[outOffset++] = (uint8) fgetc(f);  // TODO idem
                inOffset++;
            }
        }
    }

    if (inOffset != inSize)
        fatalError("error while uncompressing RLE");

    return outData;
}


uint8 *uncompress(FILE *f, uint8 compressionMethod, uint32 inSize, uint32 outSize)
{
    switch (compressionMethod) {

        case 1:
            return uncompressRLE(f, inSize, outSize);
            break;

        case 2:
            return uncompressLZW(f, inSize, outSize);
            break;

        default:
            return NULL;
            break;
    }
}


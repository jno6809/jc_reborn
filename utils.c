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
#include <stdarg.h>
#include <time.h>
#include <sys/time.h>

#include "mytypes.h"


#define BUF_LEN 256

int debugMode = 0;

void fatalError(char *message, ... )
{
    va_list(args);

    va_start(args, message);
    fprintf(stderr, "\n\n Fatal error : ");
    vfprintf(stderr, message, args);
    fprintf(stderr, "\n\n");
    va_end(args);

    exit(1);
}


void debugMsg(char *message, ... )
{
    if (debugMode) {
        va_list(args);
        va_start(args, message);
        vprintf(message, args);
        printf("\n");
        va_end(args);
    }
}


void *safe_malloc(size_t size)
{
    void *ptr = malloc(size);

    if (ptr == NULL)
        fatalError("failed to malloc() %d bytes", size);

    return ptr;
}


FILE *safe_fopen(const char *pathname, const char *mode)
{
    FILE *f;

    f = fopen(pathname, mode);

    if (f == NULL)
        fatalError("unable to open file %s in mode '%s'", pathname, mode);

    return f;
}


uint8 readUint8(FILE *f)
{
    return fgetc(f);
}


uint16 readUint16(FILE *f)
{
    uint16 a;

    a  = fgetc(f);
    a += fgetc(f) << 8;

    return a;
}


uint32 readUint32(FILE *f)
{
    uint32 a;

    a  = fgetc(f);
    a += fgetc(f) << 8;
    a += fgetc(f) << 16;
    a += fgetc(f) << 24;

    return a;
}


char *getString(FILE *f, int maxlen)
{
    int numread = 0;
    int lastread = -1;
    char buf[BUF_LEN];
    char *out;

    while ((numread < maxlen) && (numread < BUF_LEN) && (lastread != 0)) {

        lastread = buf[numread++] = fgetc(f);
    }

    out = safe_malloc(numread * sizeof(char));
    memcpy(out, buf, numread);
    return out;
}


uint8 *readUint8Block(FILE *f, int len)
{
    uint8 *out = safe_malloc(len * sizeof(char));

    for (int i=0; i < len; i++)
        out[i] = fgetc(f);

    return out;
}


uint16 *readUint16Block(FILE *f, int len)
{
    uint16 *out = safe_malloc(len * sizeof(uint16));

    for (int i=0; i < len; i++)
        out[i] = readUint16(f);

    return out;
}


uint16 peekUint16(uint8 *data, uint32 *offset)
{
    uint16 result;

    result  = data[(*offset)++];
    result |= data[(*offset)++] << 8;

    return result;
}


void peekUint16Block(uint8 *data, uint32 *offset, uint16 *dest, int len)
{
    for (int i=0; i < len ; i++)
        dest[i] = peekUint16(data, offset);
}


void hexdump(uint8 *data, uint32 len)
{
    if (data==NULL)
        printf("Can't dump NULL data\n");

    printf("\n");

    for (uint32 i=0; i < len; i++) {

        printf("%02x ",data[i]);

        if ((i & 0x0f) == 0x07) { printf (" ");  }
        if ((i & 0x0f) == 0x0f) { printf ("\n"); }
    }

    printf("\n");
}


int getDayOfYear()
{
    struct timeval tv;
    struct tm *localTime;

    gettimeofday(&tv, NULL);
    localTime = localtime(&tv.tv_sec);
    return localTime->tm_yday;
}


int getHour()
{
    struct timeval tv;
    struct tm *localTime;

    gettimeofday(&tv, NULL);
    localTime = localtime(&tv.tv_sec);
    return localTime->tm_hour;
}


char *getMonthAndDay()
{
    struct timeval tv;
    struct tm *localTime;
    static char result[5];

    gettimeofday(&tv, NULL);
    localTime = localtime(&tv.tv_sec);
    strftime(result, 5, "%m%d", localTime);
    return result;
}


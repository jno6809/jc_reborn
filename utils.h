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
#include <stdarg.h>

extern int debugMode;

void   fatalError(char *message, ... );
void   debugMsg(char *message, ... );
void   *safe_malloc(size_t size);
FILE   *safe_fopen(const char *pathname, const char *mode);
uint8  readUint8(FILE *f);
uint16 readUint16(FILE *f);
uint32 readUint32(FILE *f);
char   *getString(FILE *f, int maxlen);
uint8  *readUint8Block(FILE *f, int len);
uint16 *readUint16Block(FILE *f, int len);
uint16 peekUint16(uint8 *data, uint32 *offset);
void   peekUint16Block(uint8 *data, uint32 *offset, uint16 *dest, int len);
void   hexdump(uint8 *data, uint32 len);
int    getDayOfYear();
int    getHour();
char   *getMonthAndDay();


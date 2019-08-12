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


int main()
{
    FILE *f;
    unsigned short data[4];
    unsigned long a;

    f = fopen("SCRANTIC.SCR","r");

    fseek(f, 0x188ea, SEEK_SET);

    for (int j=0; a != 0x019456; j++) {

        a = ftell(f);

        for (int i=0; i < 3; i++) {
            data[i]  = fgetc(f);
            data[i] |= fgetc(f) << 8;
        }

        printf("    { %d, %3d, %3d, %2d },\n",
            data[0] >> 15,
            data[1],
            data[2],
            data[0] & 0x7fff);
    }
}


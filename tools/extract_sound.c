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

#include "../mytypes.h"
#include "../utils.h"

int offsets[] = {
    0x1DC00, 0x20800, 0x20E00,
    0x22C00, 0x24000, 0x24C00,
    0x28A00, 0x2C600, 0x2D000,
    0x2DE00, 0x34400, 0x32E00,
    0x39C00, 0x43400, 0x37200,
    0x37E00, 0x45A00, 0x3AE00,
    0x3E600, 0x3F400, 0x41200,
    0x42600, 0x42C00, 0x43400
};

// gcc extract_sound.c utils.c -o extract_sound
int main()
{
    FILE *f;
    f = fopen("../data/SCRANTIC.SCR","r");

    for (int j=0; j < 26; j++) {
        FILE *fw;
        uint8 *data;
        int size = 0;
        char *buffer = NULL;
        char filename[20];

        printf("offset %d\n", offsets[j]);
        if (offsets[j] == -1) {
            continue;
        }
        
        fseek(f, offsets[j], SEEK_SET);
        size = readUint16(f) + 8;
        printf("j %d, size: %d\n", j, size);

        buffer = (char*)malloc(size * sizeof(char));
        fseek(f, offsets[j], SEEK_SET);
        fread(buffer, sizeof(char), size, f);

        sprintf(filename, "../data/sound%d.wav", j + 1);
        fw = fopen(filename,"w");
        fwrite(buffer, sizeof(char), size, fw);
        fclose(fw);

        free(buffer);
    }

    fclose(f);
}

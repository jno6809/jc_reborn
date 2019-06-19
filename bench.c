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

#include "mytypes.h"
#include "graphics.h"
#include "bench.h"


void benchInit(struct TTtmSlot *ttmSlot)
{
    grLoadScreen("OCEAN00.SCR");
    grLoadBmp(ttmSlot, 0, "BOAT.BMP");
}


void benchPlay(struct TTtmThread *ttmThread, int threadNo)
{
    static int x = 10;

    grClearScreen(ttmThread->ttmLayer);
    grDrawSprite(ttmThread->ttmLayer, ttmThread->ttmSlot, x, 180+25*threadNo, 0, 0);
    x += 5;
    x %= 640;
}


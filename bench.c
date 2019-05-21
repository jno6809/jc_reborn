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


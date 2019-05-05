#include <SDL2/SDL.h>
#include "mytypes.h"
#include "ticks.h"


static uint32 lastTicks = 0x00ffffff;


void ticksInit()
{
    lastTicks = SDL_GetTicks();
}


void ticksWait(uint16 delay)
{
    delay *= 20;

    while (SDL_GetTicks() - lastTicks < delay) {

        SDL_Delay(5);  // TODO

//        SDL_Event event;   // TODO - this is TEMPO
//        while ( SDL_PollEvent(&event) ) {
//            switch(event.type) {
//                case SDL_KEYDOWN:
//                    graphicsEnd();
//                    exit(255);
//                    break;
//            }
//        }
    }

    lastTicks = SDL_GetTicks();
}


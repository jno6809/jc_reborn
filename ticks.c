#include <SDL2/SDL.h>
#include "mytypes.h"
#include "graphics.h"
#include "ticks.h"


static uint32 lastTicks = 0x00ffffff;


static void ticksPause()
{
    int stillPaused =1;
    SDL_Event event;

    SDL_Delay(5);

    while ( stillPaused && SDL_WaitEvent(&event) ) {

        switch(event.type) {

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_p)
                    stillPaused = 0;
                break;

            case SDL_WINDOWEVENT:
                graphicsRefreshWindow();
                break;

            case SDL_QUIT:
                graphicsEnd();
                exit(255);
                break;
        }
    }
}


void ticksInit()
{
    lastTicks = SDL_GetTicks();
}


void ticksWait(uint16 delay)
{
    delay *= 20;

    while (SDL_GetTicks() - lastTicks < delay) {

        SDL_Delay(5);  // TODO

        SDL_Event event;
        while ( SDL_PollEvent(&event) ) {

            switch(event.type) {

                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == SDLK_p)
                        ticksPause();
                    break;

                case SDL_QUIT:
                    graphicsEnd();
                    exit(255);
                    break;
            }
        }
    }

    lastTicks = SDL_GetTicks();
}


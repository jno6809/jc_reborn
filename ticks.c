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

    while (stillPaused && SDL_WaitEvent(&event)) {

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
        while (SDL_PollEvent(&event)) {

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


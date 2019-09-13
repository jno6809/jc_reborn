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
static int paused   = 0;
static int maxSpeed = 0;
static int oneFrame = 0;


static void ticksProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_KEYDOWN:

                switch (event.key.keysym.sym) {

                    case SDLK_SPACE:
                        paused = !paused;
                        break;

                    case SDLK_m:
                        maxSpeed = !maxSpeed;
                        break;

                    case SDLK_RETURN:
                        if (event.key.keysym.mod & KMOD_LALT) {
                            graphicsToggleFullScreen();
                            oneFrame = 1;       // to redraw the window // TODO
                        }
                        else {
                            oneFrame = 1;
                        }
                        break;

                    case SDLK_ESCAPE:
                        graphicsEnd();
                        exit(255);
                        break;
                }
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
    oneFrame = 0;

    ticksProcessEvents();

    while ((paused && !oneFrame)
            || (!maxSpeed && (SDL_GetTicks() - lastTicks < delay))) {
        SDL_Delay(5);
        ticksProcessEvents();
    }

    lastTicks = SDL_GetTicks();
}


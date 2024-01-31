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
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#include "mytypes.h"
#include "graphics.h"
#include "events.h"


static uint32 lastTicks = 0x00ffffff;
static int paused   = 0;
static int maxSpeed = 0;
static int oneFrame = 0;

int evHotKeysEnabled = 0;


static void eventsProcessEvents()
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch(event.type) {

            case SDL_KEYDOWN:

                if (evHotKeysEnabled) {

                    switch (event.key.keysym.sym) {

                        case SDLK_SPACE:
                            paused = !paused;
                            break;

                        case SDLK_m:
                            maxSpeed = !maxSpeed;
                            break;

                        case SDLK_RETURN:
                            if (event.key.keysym.mod & KMOD_LALT) {
                                grToggleFullScreen();
                                oneFrame = 1;   // to redraw the window // TODO
                            }
                            else {
                                oneFrame = 1;
                            }
                            break;

                        case SDLK_ESCAPE:
                            graphicsEnd();
#ifdef __EMSCRIPTEN__
                            emscripten_force_exit(255);
#else
                            exit(255);
#endif
                            break;
                    }
                }
                else {
                    // Normal behaviour : no hot keys, the screen saver
                    // terminates if any key is pressed
                    graphicsEnd();
#ifdef __EMSCRIPTEN__
                    emscripten_force_exit(255);
#else
                    exit(255);
#endif
                }
                break;

            case SDL_WINDOWEVENT:
                grRefreshDisplay();
                break;

            case SDL_QUIT:
                graphicsEnd();
#ifdef __EMSCRIPTEN__
                emscripten_force_exit(255);
#else
                exit(255);
#endif
                break;
        }
    }
}


void eventsInit()
{
    lastTicks = SDL_GetTicks();
}


void eventsWaitTick(uint16 delay)
{
    delay *= 20;
    oneFrame = 0;

    eventsProcessEvents();

    while ((paused && !oneFrame)
            || (!maxSpeed && (SDL_GetTicks() - lastTicks < delay))) {
#ifdef __EMSCRIPTEN__
        emscripten_sleep(5);
#else
        SDL_Delay(5);
#endif
        eventsProcessEvents();
    }

    lastTicks = SDL_GetTicks();
}


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

#include <SDL2/SDL.h>
#include <string.h>

#include "mytypes.h"
#include "utils.h"
#include "sound.h"


#define NUM_OF_SOUNDS  25


struct TSound {
    uint32  length;
    uint8   *data;
};


int soundDisabled = 0;


static struct TSound sounds[NUM_OF_SOUNDS];
static struct TSound *currentSound;

static uint8  *currentPtr;
static uint32 currentRemaining;


static void soundCallback(void *userdata, uint8 *stream, int rqdLen)
{
    if (!currentRemaining)
        return;

    if (currentRemaining > rqdLen) {
        memcpy(stream, currentPtr, rqdLen);
        currentPtr += rqdLen;
        currentRemaining -= rqdLen;
    }
    else {
        memcpy(stream, currentPtr, currentRemaining);
        memset(stream + currentRemaining, 127, rqdLen - currentRemaining);  // 127 == silence
        currentRemaining = 0;
        SDL_PauseAudio(1);
    }
}


void soundInit()
{
    if (soundDisabled)
        return;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
        debugMsg("SDL init audio error: %s", SDL_GetError());
        soundDisabled = 1;
        return;
    }

    SDL_AudioSpec audioSpec;

    for (int i=0; i < NUM_OF_SOUNDS; i++) {

        char filename[20];

        sprintf(filename, "sound%d.wav", i);

        if (SDL_LoadWAV(filename, &audioSpec, &sounds[i].data, &sounds[i].length) == NULL) {
            sounds[i].data   = NULL;
            sounds[i].length = 0;
            debugMsg("SDL_LoadWAV() warning: %s", SDL_GetError());
        }
    }

    audioSpec.callback = soundCallback;
    audioSpec.userdata = NULL;
    audioSpec.samples  = 1024;

    if (SDL_OpenAudio(&audioSpec, NULL) < 0) {
        debugMsg("SDL_OpenAudio() error: %s", SDL_GetError());
        soundDisabled = 1;
        return;
    }

    SDL_PauseAudio(1);
}


void soundEnd()
{
    if (soundDisabled)
        return;

    SDL_CloseAudio();

    for (int i=0; i < NUM_OF_SOUNDS; i++)
        if (sounds[i].data != NULL)
            SDL_FreeWAV(sounds[i].data);
}


void soundPlay(int nb)
{
    if (soundDisabled)
        return;

    if (nb < 0 || NUM_OF_SOUNDS <= nb) {
        debugMsg("soundPlay(): wrong sound sample index #%d", nb);
        return;
    }

    if (sounds[nb].length) {

        SDL_LockAudio();
        SDL_PauseAudio(1);

        currentSound     = &sounds[nb];
        currentPtr       = currentSound->data;
        currentRemaining = currentSound->length;

        SDL_PauseAudio(0);
        SDL_UnlockAudio();
    }
    else {
        debugMsg("Non-existent sound sample #%d", nb);
    }
}


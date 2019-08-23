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

#define NUM_OF_NODES  6
#define UNDEF_NODE    6


static int walkMatrix[NUM_OF_NODES+1][NUM_OF_NODES][NUM_OF_NODES] = {

      //  A  B  C  D  E  F

    // from A
    {   { 0, 0, 0, 0, 0, 0 },  // A
        { 0, 0, 1, 0, 0, 0 },  // B
        { 0, 0, 0, 1, 0, 0 },  // C
        { 0, 0, 0, 0, 0, 0 },  // D
        { 0, 0, 0, 1, 0, 1 },  // E
        { 0, 0, 0, 0, 0, 0 },  // F
    },

    // from B
    {   { 0, 0, 0, 0, 1, 0 },  // A
        { 0, 0, 0, 0, 0, 0 },  // B
        { 0, 0, 0, 1, 0, 0 },  // C
        { 0, 0, 0, 0, 0, 0 },  // D
        { 0, 0, 0, 0, 0, 0 },  // E
        { 0, 0, 0, 0, 0, 0 },  // F
    },

    // from C
    {   { 0, 0, 0, 0, 1, 0 },  // A
        { 1, 0, 0, 0, 0, 0 },  // B
        { 0, 0, 0, 0, 0, 0 },  // C
        { 0, 0, 0, 0, 1, 0 },  // D
        { 0, 0, 0, 0, 0, 0 },  // E
        { 0, 0, 0, 0, 0, 0 },  // F
    },

    // from D
    {   { 0, 0, 0, 0, 0, 0 },  // A
        { 0, 0, 0, 0, 0, 0 },  // B
        { 1, 1, 0, 0, 0, 1 },  // C
        { 0, 0, 0, 0, 0, 0 },  // D
        { 1, 0, 0, 0, 0, 0 },  // E
        { 0, 0, 0, 0, 1, 0 },  // F
    },

    // from E
    {   { 0, 1, 1, 0, 0, 0 },  // A
        { 0, 0, 0, 0, 0, 0 },  // B
        { 0, 0, 0, 0, 0, 0 },  // C
        { 0, 0, 1, 0, 0, 0 },  // D
        { 0, 0, 0, 0, 0, 0 },  // E
        { 0, 0, 0, 1, 0, 0 },  // F
    },

    // from F
    {   { 0, 0, 0, 0, 0, 0 },  // A
        { 0, 0, 0, 0, 0, 0 },  // B
        { 0, 0, 0, 1, 0, 0 },  // C
        { 0, 0, 0, 0, 0, 0 },  // D
        { 1, 0, 0, 0, 0, 0 },  // E
        { 0, 0, 0, 0, 0, 0 },  // F
    },

    // from any spot
    {   { 0, 1, 1, 0, 1, 1 },  // A
        { 1, 0, 1, 0, 0, 0 },  // B
        { 1, 1, 0, 1, 1, 1 },  // C
        { 0, 0, 1, 0, 1, 1 },  // D
        { 1, 0, 1, 1, 0, 1 },  // E
        { 1, 0, 1, 1, 1, 0 },  // F
    },
};


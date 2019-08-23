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

#include "mytypes.h"
#include "utils.h"
#include "calcpath.h"
#include "calcpath_data.h"


struct TNodeState {
    int isMarked;
    int fromNode;
};


#define MAX_NUM_PATHS  50
#define MAX_PATH_LEN   7

static int paths[MAX_NUM_PATHS][MAX_PATH_LEN];
static int numPaths;

static struct TNodeState nodeStates[NUM_OF_NODES];
static int dstNode;
static int pathLen;


static void calcPathRecurse(int prevNode, int curNode)
{
    if (curNode == dstNode) {

        // One possible path found, let's add it to the list
        for (int i=pathLen-1; i >= 0; i--) {
            paths[numPaths][i] = curNode;
            curNode = nodeStates[curNode].fromNode;
        }

        paths[numPaths++][pathLen] = UNDEF_NODE;
    }

    else {

        // Call recursively each node we can reach from our current position
        for (int nextNode=0; nextNode < NUM_OF_NODES; nextNode++) {

            if (walkMatrix[prevNode][curNode][nextNode] && !nodeStates[nextNode].isMarked) {
                nodeStates[nextNode].isMarked = 1;
                nodeStates[nextNode].fromNode = curNode;
                pathLen++;
                calcPathRecurse(curNode, nextNode);
                nodeStates[nextNode].isMarked = 0;
                pathLen--;
            }
        }
    }
}


int *calcPath(int fromNode, int toNode)
{
    // Note: this is certainly not the exact algorithm used in the original,
    // but so far it is the best I could imagine to fit the need.

    int *res;

    for (int i=0; i < NUM_OF_NODES; i++) {
        nodeStates[i].isMarked = 0;
        nodeStates[i].fromNode = 0;
    }

    dstNode  = toNode;
    numPaths = 0;
    pathLen  = 1;
    nodeStates[fromNode].isMarked = 1;
    nodeStates[fromNode].fromNode = UNDEF_NODE;

    calcPathRecurse(UNDEF_NODE, fromNode);

    if (debugMode) {

        printf("\n +-- CALCULATE PATH --\n |\n");
        printf(" |  . walking from %c to %c:\n", 'A' + fromNode, 'A' + toNode);
        printf(" |  . possible paths: ");

        for (int j=0; j < numPaths; j++) {
            putchar(' ');
            for (int i=0; paths[j][i] != UNDEF_NODE; i++)
                printf("%c", 'A' + paths[j][i]);
        }

        putchar('\n');
    }

    res = paths[rand() % numPaths];

    if (debugMode) {

        printf(" |  . chosen path: ");

        for (int i=0; res[i] != UNDEF_NODE; i++)
            printf("%c", 'A' + res[i]);

        printf("\n +--------------------\n\n");
    }

    return res;
}


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
#include "ads.h"
#include "story.h"


void storyPlay()
{
    //randomizeIsland();

    adsPlayIntro();
return;

    adsInitIsland();

    adsPlayWalk();

    adsPlay("STAND.ADS"   , 11);// E
    adsPlay("ACTIVITY.ADS", 4 );
    adsPlay("STAND.ADS"   , 11);// E

//    adsPlay("STAND.ADS"   , 9 );// D nw
//    adsPlay("BUILDING.ADS", 7 );
//    adsPlay("STAND.ADS"   , 10);// D ne

//    adsPlay("STAND.ADS"   , 9 );// D nw
//    adsPlay("FISHING.ADS" , 1 );
//    adsPlay("STAND.ADS"   , 10);// D

//    adsPlay("STAND.ADS"   , 10);// D
//    adsPlay("VISITOR.ADS" , 4 );
//    adsPlay("STAND.ADS"   , 10);// D

///    adsPlay("STAND.ADS"   , 11);// E
///    adsPlay("FISHING.ADS" , 8 );
///    adsPlay("STAND.ADS"   , 11);// E

    adsPlay("STAND.ADS"   , 12);// F
    adsPlay("BUILDING.ADS", 1 );
    adsPlay("STAND.ADS"   , 1 ); // A

///    adsPlay("STAND.ADS"   , 11);// E
///    adsPlay("FISHING.ADS" , 7 );
///    adsPlay("STAND.ADS"   , 11);// E

    adsPlay("STAND.ADS"   , 10);// D
    adsPlay("WALKSTUF.ADS", 3 );
    adsPlay("STAND.ADS"   , 11);// E

    adsPlay("STAND.ADS"   , 10);// D
    adsPlay("BUILDING.ADS", 5 );
    adsPlay("STAND.ADS"   , 10);// D

///    adsPlay("STAND.ADS"   , 11);// E
///    adsPlay("FISHING.ADS" , 8 );
///    adsPlay("STAND.ADS"   , 11);// E

    adsPlay("STAND.ADS"   , 10);// D
    adsPlay("BUILDING.ADS", 5 );
    adsPlay("STAND.ADS"   , 10);// D

    adsNoIsland();
}


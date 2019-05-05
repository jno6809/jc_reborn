#include "mytypes.h"
#include "ads.h"
#include "story.h"


void storyPlay()
{
    //randomizeIsland();

//    adsPlayIntro();

    adsInitIsland();

    adsPlayWalk();
return;
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


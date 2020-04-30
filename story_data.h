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

#define NUM_SCENES    63

#define FINAL         0x01
#define FIRST         0x02
#define ISLAND        0x04
#define LEFT_ISLAND   0x08
#define VARPOS_OK     0x10
#define LOWTIDE_OK    0x20
#define NORAFT        0x40
#define HOLIDAY_NOK   0x80

#define SPOT_A 0
#define SPOT_B 1
#define SPOT_C 2
#define SPOT_D 3
#define SPOT_E 4
#define SPOT_F 5

#define HDG_S   0
#define HDG_SW  1
#define HDG_W   2
#define HDG_NW  3
#define HDG_N   4
#define HDG_NE  5
#define HDG_E   6
#define HDG_SE  7

struct TStoryScene {
    char adsName[13];
    int  adsTagNo;
    int  spotStart;
    int  hdgStart;
    int  spotEnd;
    int  hdgEnd;
    int  dayNo;
    int  flags;
};


static struct TStoryScene storyScenes[NUM_SCENES] = {

  //           Name  Tag    Start   Start      End     End Day of  Flags
  //                         spot     hdg     spot     hdg  story
    { "ACTIVITY.ADS",  1,  SPOT_E, HDG_SE,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },
    { "ACTIVITY.ADS", 12,  SPOT_D, HDG_SW,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "ACTIVITY.ADS", 11,       0,      0,       0,      0,   0,  ISLAND | FINAL | FIRST | VARPOS_OK                        },
    { "ACTIVITY.ADS", 10,  SPOT_D, HDG_SW,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "ACTIVITY.ADS",  4,  SPOT_E, HDG_SE,  SPOT_E, HDG_SE,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "ACTIVITY.ADS",  5,  SPOT_E, HDG_SW,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "ACTIVITY.ADS",  6,  SPOT_D, HDG_SW,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },
    { "ACTIVITY.ADS",  7,  SPOT_D, HDG_SW,  SPOT_F, HDG_SW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "ACTIVITY.ADS",  8,       0,      0,  SPOT_D, HDG_SE,   0,  ISLAND | FIRST | VARPOS_OK                                },
    { "ACTIVITY.ADS",  9,  SPOT_E, HDG_E ,       0,      0,   0,  ISLAND | FINAL | LOWTIDE_OK                               },

    { "BUILDING.ADS",  1,  SPOT_F, HDG_W ,  SPOT_A, HDG_W ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "BUILDING.ADS",  4,  SPOT_A, HDG_E ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },
    { "BUILDING.ADS",  3,  SPOT_A, HDG_E ,  SPOT_C, HDG_SE,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "BUILDING.ADS",  2,  SPOT_F, HDG_W ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },
    { "BUILDING.ADS",  5,  SPOT_D, HDG_W ,  SPOT_D, HDG_E ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
//  { "BUILDING.ADS",  9,  SPOT_x, HDG_xx,  SPOT_x, HDG_xx,   0,  ISLAND                                                    }, // strictly identical to tag 5
    { "BUILDING.ADS",  7,  SPOT_D, HDG_W ,  SPOT_D, HDG_E ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
//  { "BUILDING.ADS",  8,  SPOT_x, HDG_xx,  SPOT_x, HDG_xx,   0,  ISLAND                                                    }, // strictly identical to tag 7
    { "BUILDING.ADS",  6,  SPOT_A, HDG_E ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },

    { "FISHING.ADS" ,  1,  SPOT_D, HDG_W ,  SPOT_D, HDG_E ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "FISHING.ADS" ,  2,  SPOT_D, HDG_W ,  SPOT_D, HDG_E ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "FISHING.ADS" ,  3,  SPOT_D, HDG_W ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "FISHING.ADS" ,  4,  SPOT_E, HDG_E ,       0,      0,   0,  ISLAND | FINAL | LEFT_ISLAND | LOWTIDE_OK                 },
    { "FISHING.ADS" ,  5,  SPOT_E, HDG_E ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },
    { "FISHING.ADS" ,  6,  SPOT_D, HDG_W ,       0,      0,   0,  ISLAND | FINAL | LOWTIDE_OK                               },
    { "FISHING.ADS" ,  7,  SPOT_E, HDG_E ,  SPOT_E, HDG_W ,   0,  ISLAND | LEFT_ISLAND | VARPOS_OK | LOWTIDE_OK             },
    { "FISHING.ADS" ,  8,  SPOT_E, HDG_E ,  SPOT_E, HDG_W ,   0,  ISLAND | LEFT_ISLAND | VARPOS_OK | LOWTIDE_OK             },

    { "JOHNNY.ADS"  ,  1,       0,      0,       0,      0,  11,  FINAL | FIRST                                             },
    { "JOHNNY.ADS"  ,  2,  SPOT_E, HDG_SW,  SPOT_F,      0,   2,  ISLAND | FINAL | VARPOS_OK                                },
    { "JOHNNY.ADS"  ,  3,  SPOT_E, HDG_SW,  SPOT_F, HDG_NE,   6,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "JOHNNY.ADS"  ,  4,  SPOT_E, HDG_SW,  SPOT_F, HDG_NE,   0,  ISLAND | VARPOS_OK                                        },
    { "JOHNNY.ADS"  ,  5,  SPOT_E, HDG_SW,  SPOT_F, HDG_NE,   0,  ISLAND | VARPOS_OK                                        },
    { "JOHNNY.ADS"  ,  6,       0,      0,       0,      0,  10,  FINAL | FIRST                                             },

    { "MARY.ADS"    ,  1,  SPOT_E, HDG_SW,       0,      0,   5,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "MARY.ADS"    ,  3,  SPOT_F, HDG_SW,       0,      0,   4,  ISLAND | FINAL | FIRST | VARPOS_OK                        },
    { "MARY.ADS"    ,  2,  SPOT_E, HDG_E ,       0,      0,   1,  ISLAND | FINAL | VARPOS_OK                                },
    { "MARY.ADS"    ,  4,  SPOT_E, HDG_E ,       0,      0,   7,  ISLAND | FINAL | VARPOS_OK                                },
    { "MARY.ADS"    ,  5,  SPOT_E, HDG_NW,       0,      0,   8,  ISLAND | LEFT_ISLAND | FINAL | FIRST | NORAFT | VARPOS_OK },

    { "MISCGAG.ADS" ,  1,  SPOT_D, HDG_W ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK | LOWTIDE_OK                   },
    { "MISCGAG.ADS" ,  2,  SPOT_D, HDG_W ,       0,      0,   0,  ISLAND | FINAL | VARPOS_OK                                },

    { "STAND.ADS"   ,  1,  SPOT_A, HDG_SW,  SPOT_A, HDG_SW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  2,  SPOT_A, HDG_W ,  SPOT_A, HDG_W ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  3,  SPOT_A, HDG_NW,  SPOT_A, HDG_NW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  4,  SPOT_B, HDG_SW,  SPOT_B, HDG_SW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  5,  SPOT_B, HDG_S ,  SPOT_B, HDG_S ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  6,  SPOT_B, HDG_SE,  SPOT_B, HDG_SE,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  7,  SPOT_C, HDG_NE,  SPOT_C, HDG_NE,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  8,  SPOT_C, HDG_E ,  SPOT_C, HDG_E ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   ,  9,  SPOT_D, HDG_NW,  SPOT_D, HDG_NW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 10,  SPOT_D, HDG_NE,  SPOT_D, HDG_NE,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 11,  SPOT_E, HDG_NW,  SPOT_E, HDG_NW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 12,  SPOT_F, HDG_S ,  SPOT_F, HDG_S ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 15,  SPOT_A, HDG_S ,  SPOT_A, HDG_S ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "STAND.ADS"   , 16,  SPOT_C, HDG_S ,  SPOT_C, HDG_S ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },

    { "SUZY.ADS"    ,  1,       0,      0,       0,      0,   3,  FINAL | FIRST                                             },
    { "SUZY.ADS"    ,  2,       0,      0,       0,      0,   9,  FINAL | FIRST                                             },

    { "VISITOR.ADS" ,  1,  SPOT_A, HDG_S ,  SPOT_A, HDG_S ,   0,  ISLAND | LOWTIDE_OK                                       },
    { "VISITOR.ADS" ,  3,  SPOT_B, HDG_NW,  SPOT_D,      0,   0,  ISLAND | FINAL | HOLIDAY_NOK                              },
    { "VISITOR.ADS" ,  4,  SPOT_D, HDG_S ,  SPOT_D, HDG_W ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "VISITOR.ADS" ,  6,  SPOT_D, HDG_S ,  SPOT_D, HDG_SW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "VISITOR.ADS" ,  7,  SPOT_D, HDG_S ,  SPOT_D, HDG_SW,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
    { "VISITOR.ADS" ,  5,  SPOT_E, HDG_SW,       0,      0,   0,  ISLAND | FINAL | LEFT_ISLAND | VARPOS_OK | LOWTIDE_OK     },

    { "WALKSTUF.ADS",  1,  SPOT_A, HDG_NE,       0,      0,   0,  ISLAND | FINAL | LOWTIDE_OK                               },
    { "WALKSTUF.ADS",  2,  SPOT_E, HDG_E ,  SPOT_D, HDG_SE,   0,  ISLAND | VARPOS_OK                                        },
    { "WALKSTUF.ADS",  3,  SPOT_D, HDG_W ,  SPOT_E, HDG_W ,   0,  ISLAND | VARPOS_OK | LOWTIDE_OK                           },
};


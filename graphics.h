#include <SDL2/SDL.h>

#define MAX_BMP_SLOTS       6
#define MAX_SPRITES_PER_BMP 120
#define MAX_TTM_SLOTS       10
#define MAX_TTM_THREADS     10


struct TAdsScene {
    uint16 slot;
    uint16 tag;
    uint16 numPlays;
};


struct TTtmSlot {
    uint8       *data;
    uint32      dataSize;
    struct      TTtmTag *tags;
    int         numTags;
    int         numSprites[MAX_BMP_SLOTS];
    SDL_Surface *sprites[MAX_BMP_SLOTS][MAX_SPRITES_PER_BMP];
};

struct TTtmTag {  // TODO : rename, used for ADS too
    uint16 id;
    uint32 offset;
};

struct TTtmThread {
    struct TTtmSlot   *ttmSlot;
    int    isRunning;
    uint16 sceneSlot;
    uint16 sceneTag;
    short  sceneTimer;
    uint16 sceneIterations;
    uint32 ip;
    uint16 delay;
    uint16 timer;
    uint32 nextGotoOffset;
    uint8  selectedBmpSlot;
    uint8  fgColor;
    uint8  bgColor;
    SDL_Surface *ttmLayer;
};

extern SDL_Surface *grBackgroundSfc;

extern int grDx;
extern int grDy;


void graphicsInit(int fullScreen);
void graphicsEnd();
void graphicsRefreshWindow();
void graphicsUpdate(struct TTtmThread *ttmBackgroundThread,
                    struct TTtmThread *ttmThreads,
                    struct TTtmThread *ttmHolidayThreads);

SDL_Surface *grNewEmptyBackground();
SDL_Surface *grNewLayer();
void grFreeLayer(SDL_Surface *sfc);

void grLoadBmp(struct TTtmSlot *ttmSlot, uint16 slotNo, char *strArg);
void grReleaseBmp(struct TTtmSlot *ttmSlot, uint16 bmpSlotNo);

void grSetClipZone(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2);
void grSaveImage0(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grSaveImage1(SDL_Surface *sfc, uint16 arg0, uint16 arg1, uint16 arg2, uint16 arg3);
void grDrawLine(SDL_Surface *sfc, sint16 x1, sint16 y1, sint16 x2, sint16 y2, uint8 color);
void grDrawRect(SDL_Surface *sfc, sint16 x, sint16 y, uint16 width, uint16 height, uint8 color);
void grDrawCircle(SDL_Surface *sfc, sint16 x1, sint16 y1, uint16 width, uint16 height, uint8 fgColor, uint8 bgColor);
void grDrawSprite(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo);
void grDrawSpriteFlip(SDL_Surface *sfc, struct TTtmSlot *ttmSlot, sint16 x, sint16 y, uint16 spriteNo, uint16 imageNo);
void grInitEmptyBackground();
void grClearScreen(SDL_Surface *sfc);
void grFadeOut();

void grLoadPalette();   // TODO
void grLoadScreen(char *strArg);


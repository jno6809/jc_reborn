
CFLAGS=-Wall -Wpedantic -std=c99

LDLIBS=-lSDL2

EXE =   jc_reborn

OBJ =   jc_reborn.o \
        utils.o \
        uncompress.o \
        resource.o \
        dump.o \
        story.o \
        walk.o \
        calcpath.o \
        ads.o \
        ttm.o \
        island.o \
        bench.o \
        graphics.o \
        sound.o \
        events.o \
        config.o


all: $(EXE)

$(EXE): $(OBJ)

clean:
	rm -v $(OBJ) $(EXE) 2> /dev/null ; true


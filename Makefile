
CFLAGS=-Wall -Wpedantic

LDLIBS=-lSDL2

EXE =   myscrantic

OBJ =   myscrantic.o \
        utils.o \
        uncompress.o \
        resource.o \
        dump.o \
        story.o \
        walk.o \
        ads.o \
        ttm.o \
        island.o \
        bench.o \
        graphics.o \
        ticks.o


all: $(EXE)

$(EXE): $(OBJ)

clean:
	rm -v $(OBJ) $(EXE) 2> /dev/null ; true


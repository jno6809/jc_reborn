CFLAGS=-Wall -Wpedantic -std=c99

LDLIBS=-lSDL2

BASE_EXE = jc_reborn

ifdef EMSCRIPTEN
EXE =   $(BASE_EXE).js
CFLAGS+=-sUSE_SDL=2 -Os
LDFLAGS+=--embed-file data -sASYNCIFY -sALLOW_MEMORY_GROWTH -sSTANDALONE_WASM=0 -sEXIT_RUNTIME
else
EXE =   $(BASE_EXE)
endif

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
ifdef EMSCRIPTEN
	emcc $(OBJ) -o $(EXE) $(LDLIBS) $(LDFLAGS)
endif

clean:
	rm -v $(OBJ) $(BASE_EXE) $(BASE_EXE).js $(BASE_EXE).wasm 2> /dev/null ; true

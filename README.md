# Johnny Reborn

Johnny Reborn is an open source engine for the classic Johnny Castaway screen saver, developed by Dynamix for Windows 3.x and published by Sierra, back in 1992.

It is written in C using the SDL2 library, and was successfully compiled and tested on Linux, on MacOSX, on Chrome and FireFox via Emscripten, as well as Windows (MinGW), both 32 and 64 bits.


## How to install

For the screen saver to work, you'll need the three data files from the original
software: `RESOURCE.MAP`, `RESOURCE.001` and `SCRANTIC.SCR` and save it under a `data` folder in the root directory.

Use `extract_sound` to dump the audio files from `SCRANTIC.SCR` file.

> cd tools

> make -f Makefile.sound

> ./extract_sound


## How to run

> make

> ./jc_reborn window


By default, the engine runs full screen and plays the life of Johnny on his island, as the original did.

But, if you're curious about how the engine works, you may type `jc_reborn help` and see the different options available. Feel free to try them and explore the inner workings of Screen Antics !


## Current status

### Short version
Currently, Johnny reborn is in "work in progress" state. Even so, as of January 2020, every scene works with only some inaccuracies here and there.

That means that the engine globally works in an acceptable way, but more needs to be done to fully understand a number of details and faithfully reproduce the behaviour of the original engine.

### Long version

Some great work was already made by Hans Milling for "Johnny Castaway Open Source" (JCOS), back in 2015. This includes:
  - the parsing and decoding of all the data files
  - the understanding of many instructions which constitute TTM and ADS scripts

What Johnny Reborn brings is:
  - the understanding of nearly every TTM and ADS instruction, and their parameters
  - the algorithm for Johnny to transitionnaly walk from scene to scene was implemented. Accuracy is not bad though not perfect.
  - the same can be said about the algorithm which randomly chooses scenes to be played.
  - as well as the algorithm which draws the island at a random place, clouds, etc.
  - all the work was made by observing the behaviour of the original software and trying to reproduce it as accurately as possible. For a better result, a complete disassembly of the original exe may be necessary - but wasn't done to this point.


## Thanks

I never would have been able to write Johnny Reborn without, directly or indirectly, all the people listed below. Many thanks to them.

  - Hans Milling aka nivs1978, author of the JCOS project - my main source of info for my first lines of the Johnny Reborn code
    - https://github.com/nivs1978/Johnny-Castaway-Open-Source
    - http://nivs.dk/jc/
  - Alexandre Fontoura aka xesf, author of castaway project - similar to Johnny Reborn, but in Javascript
    - https://github.com/xesf/castaway
    - https://castaway.xesf.net/viewer/
  - The Sierra Chest website, which has a nice section about Johnny Castaway, with many screenshots and video captures. Those turned out to be quite helpful:
    - http://sierrachest.com/index.php?a=games&id=255&title=johnny-castaway

Hans Milling thanks a number of people for giving him (or helping him find) some info about the original engine. They should not be forgotten, and I thank them - indirectly - as well:

  - Jeff Tunnel - For helping getting in contact with some of the original developers
  - Kevin and Liam Ryan - Assisting with information about the resource files
  - Jaap - Helping in finding the format of the resource files
  - Gregori - Assisting with the Lempel-Ziv decompression
  - Guido - The author of the xBaK project that led to understanding the TTM and ADS commands.



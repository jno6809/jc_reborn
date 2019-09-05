#!/bin/awk -f

# Better formatting/indentation of ADS scripts


function iprint(string)
{
    for (i=0; i<indent; i++)
        printf "    "
    printf "%s\n", string
}

BEGIN {
    indent    = 0;
    condblock = 0;
}

/(IF_UNKNOWN_1|IF_LASTPLAYED |IF_IS_RUNNING|IF_NOT_RUNNING)/ {
    if (condblock == 1) {
        iprint("{");
        indent++;
    }
    condblock = 1;
    iprint($0);
    next;
}

/^(OR|AND)$/ {
    iprint($0);
    condblock = 2;
    next;
}

{ if (condblock == 1) {
    iprint("{");
    indent++;
    condblock=0;
  }
}

/RANDOM_START/ {
    iprint("RANDOM");
    iprint("{");
    indent++;
    next;
}

/RANDOM_END/ {
    indent--;
    iprint("}.");
    next;
}

/PLAY_SCENE/ {
    indent--;
    iprint("}");
    next;
}

{ iprint($0);
}


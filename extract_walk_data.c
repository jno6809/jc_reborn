#include <stdio.h>
#include <stdlib.h>

int main()
{
    FILE *f;
    unsigned short data[4];
    unsigned long a;

    f = fopen("walkdata.ttm","r");

    fseek(f, 0x188ea, SEEK_SET);

    for (int j=0; a != 0x019456; j++) {

        a = ftell(f);

        for (int i=0; i < 3; i++) {
            data[i]  = fgetc(f);
            data[i] |= fgetc(f) << 8;
        }

//        printf("%06lx %04x %04x %04x  %04d %04d %04d ",
//            a,
//            data[0], data[1], data[2],
//            data[0], data[1], data[2]);

        printf("    { %d, %3d, %3d, %2d },\n",
            data[0] >> 15,
            data[1],
            data[2],
            data[0] & 0x7fff);
    }
}


//
// utils.h implementation
//
#include "../include/utils.h"
#include <stdio.h>

static void dump_hexes(byte *bytes, short amount)
{
    for (short i = 0; i < amount; i++) {
        printf("%02hhx", bytes[i]);
        if (i % 2 != 0)
            printf(" ");
    }
}

void util_hexdump(byte *bytes, uint amount)
{
    uint lines = (amount - (amount % 16)) / 16;
    uint rest = amount % 16;

    if (lines == 0) {
        dump_hexes(bytes, rest);
        return;
    }

    for (uint i = 0; i < lines; i++) {
        dump_hexes(bytes + (i * 16), 16);
        printf("\n");
    }

    dump_hexes(bytes + (amount - rest), rest);
    printf("\n");

}


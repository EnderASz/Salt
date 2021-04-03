/**
 * utils.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/utils.h"

#include <stdio.h>

static void dump_hexes(u8 *bytes, short amount)
{
    for (short i = 0; i < amount; i++) {
        printf("%02hx", bytes[i]);
        if (i % 2 != 0)
            printf(" ");
    }
    if (amount)
        printf("\n");
}

void util_hexdump(u8 *bytes, u32 amount)
{
    u32 lines = (amount - (amount % 16)) / 16;
    u32 rest = amount % 16;

    if (lines == 0) {
        dump_hexes(bytes, rest);
        return;
    }

    for (u32 i = 0; i < lines; i++) {
        dump_hexes(bytes + (i * 16), 16);
    }
    dump_hexes(bytes + (amount - rest), rest);
}

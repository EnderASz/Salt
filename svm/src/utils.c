/**
 * Salt Virtual Machine
 * 
 * Copyright (C) 2021  The Salt Programming Language Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * END OF COPYRIGHT NOTICE
 *
 * utils.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/utils.h"

#include <stdio.h>


/* A bit of compiler compaibilty: if we're using clang, we get the format
   specifier warning, which tells us to use hhx instead of hx for bytes, 
   but if we're using mingw instead, it tell us to NOT use hhx. \o/ */

#if defined(_WIN32)
#define SVM_UTIL_DUMP_BYTE_FORMAT "%02hx"

#elif defined(__clang__) && !defined(_WIN32)
#define SVM_UTIL_DUMP_BYTE_FORMAT "%02hhx"

#else 
#define SVM_UTIL_DUMP_BYTE_FORMAT "%02hhx"

#endif


static void dump_hexes(u8 *bytes, u32 amount)
{
    for (u32 i = 0; i < amount; i++) {
        printf(SVM_UTIL_DUMP_BYTE_FORMAT, bytes[i]);
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

void util_bitdump(u8 value)
{
    u8 max_byte = 1 << 7; 
    
    printf(SVM_UTIL_DUMP_BYTE_FORMAT" ", value);
    for (u32 j = 0; j < 8; j++) {

        /* Print one bit, and then move to the next one */
        printf("%u", value & max_byte ? 1 : 0); 
        value <<= 1;
    }
    printf("\n");
}


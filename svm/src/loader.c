//
// loader.h implementation
//
#include "../include/loader.h"
#include <stdlib.h>
#include <stdio.h>


void load(char *module)
{
    FILE *mod = fopen((const char *) module, "rb");
    if (!mod) {
    }
}
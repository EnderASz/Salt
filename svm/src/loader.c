//
// loader.h implementation
//
#include "../include/loader.h"
#include "../include/utils.h"
#include "../include/exception.h"
#include <stdlib.h>
#include <stdio.h>


void load(char *name, const char *alternative)
{
    FILE *mod = fopen((const char *) name, "rb");
    if (!mod) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    struct SaltModule *module = NULL;
    if (alternative != NULL)
        module = module_create(name);
    else
        module = module_create((char *) alternative);

    char header[64];
    fread(header, 1, 64, mod);

    util_hexdump((byte *) header, 64);

    fclose(mod);
}
//
// loader.h implementation
//
#include "../include/loader.h"
#include "../include/utils.h"
#include "../include/exception.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCC_HEADER "\x7fSCC\xff\xee\0\0\0"
#define SCC_VERSION 3

static int validate_header(char *header)
{
    if (strncmp(header, SCC_HEADER, 8) != 0)
        return 0;

    if (* (uint *) (header + 16) != SCC_VERSION)
        return 0;

    return 1;
}

static void load_header(FILE *fp)
{
    char *header = vmalloc(sizeof(char) * 64);
    fread(header, 1, 64, fp);

    if (!validate_header(header)) {
        printf("This compiled file is invalid or corrupted.\n");
        vmfree(header, sizeof(char) * 64);
        return;
    }
    vmfree(header, sizeof(char) * 64);
}

int load(char *name, const char *alternative)
{
    dprintf("Trying to load %s (%s)\n", name, alternative);
    FILE *mod = fopen((const char *) name, "rb");
    if (!mod) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    struct SaltModule *module = NULL;
    if (alternative != NULL)
        module = module_create(name);
    else
        module = module_create((char *) alternative);

    load_header(mod);

    fclose(mod);
    return 0;
}

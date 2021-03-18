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

    if (* (uint *) (header + 8) != SCC_VERSION)
        return 0;

    return 1;
}

static int load_header(FILE *fp)
{
    char *header = vmalloc(sizeof(char) * 64);
    fread(header, 1, 64, fp);

    if (!validate_header(header)) {
        vmfree(header, sizeof(char) * 64);
        exception_throw(EXCEPTION_RUNTIME, "Salt module is either invalid or corrupted");
    }

    int result = * (uint *) (header + 16);
    vmfree(header, sizeof(char) * 64);
    return result;
}

static void read_instruction(struct SaltInstruction *ins, FILE *fp)
{
    int width = 1;
    while (fgetc(fp) != '\n')
        width++;

    ins->size = width;
    ins->content = vmalloc(sizeof(char) * (ins->size));
    fseek(fp, -width, SEEK_CUR);
    fread(ins->content, 1, width - 1, fp);
    fgetc(fp);
    dprintf("[%d] {%s}\n", width, ins->content);
}

static void load_instructions(struct SaltModule *module, FILE *fp, int instructions)
{
    dprintf("Loading instructions for '%s'\n", module->name);

    module->instructions = vmalloc(sizeof(struct SaltInstruction) * instructions);
    module->instruction_amount = instructions;

    for (uint i = 0; i < instructions; i++)
        read_instruction(&module->instructions[i], fp);

    for (uint i = 0; i < instructions; i++) {

    }

}

struct SaltModule *load(char *name)
{
    dprintf("Trying to load %s\n", name);

    // File

    char *filename = vmalloc(sizeof(char) * strlen(name));
    if (strcmp(name + (strlen(name) - 4), ".scc") != 0) {
        strcpy(filename, name);
        strcat(filename, ".scc");
    }
    else {
        strcpy(filename, name);
    }

    dprintf("Loading %s\n", filename);

    FILE *mod = fopen(filename, "rb");
    vmfree(filename, sizeof(char) * strlen(name));
    if (!mod) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot find module");
    }

    name[strlen(name) - 4] = 0;

    // Module

    struct SaltModule *module = module_create(name);
    int instructions = load_header(mod);

    load_instructions(module, mod, instructions);

    fclose(mod);
    return module;
}

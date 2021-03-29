/**
 * loader.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/loader.h"
#include "../include/utils.h"
#include "../include/exception.h"
#include "../include/exec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SCC_HEADER "\x7fSCC\xff\xee\0\0\0"
#define SCC_VERSION 3

/**
 * The header data container stores all the meta data read from the SCC
 * header.
 *
 * @a instructions  amount of instructions
 * @a registers     amount of registers
 */
struct LoaderHeaderData {

    uint instructions;
    uint8_t registers;

};


static int validate_header(char *header)
{
    if (strncmp(header, SCC_HEADER, 8) != 0)
        return 0;

    if (* (uint *) (header + 8) != SCC_VERSION)
        return 0;

    return 1;
}

static struct LoaderHeaderData load_header(FILE *fp)
{
    char *header = vmalloc(sizeof(char) * 64);
    fread(header, 1, 64, fp);

    if (!validate_header(header)) {
        vmfree(header, sizeof(char) * 64);
        exception_throw(EXCEPTION_RUNTIME, "Salt module is either invalid or corrupted");
    }

    struct LoaderHeaderData data;
    data.instructions = * (uint *) (header + 16);
    data.registers    = * (uint8_t *) (header + 24); 

    vmfree(header, sizeof(char) * 64);
    return data;
}

static void read_instruction(String *ins, FILE *fp)
{
    // Check for label
    char label_char = fgetc(fp);
    int width = 0;

    // If it's a label, do a normal read
    if (label_char == '@') {
        while (fgetc(fp) != '\n')
            width++;
        fseek(fp, -width - 2, SEEK_CUR);
        width += 1;
    }
    // Else add the proper amount of initial padding, which it the amount
    // of bytes that should be ignored when looking for the 0x0a byte,
    // splitting instructions.
    // Before doing anything, prepare a 5 byte buffer and move the FILE cursor
    // back by one char.
    else {

        fseek(fp, -1, SEEK_CUR);
        char buf[6] = {0};
        fread(buf, 1, 5, fp);

        int i = 0;
        for (; (uint) i < g_exec_amount; i++) {
            if (strncmp(buf, g_execs[i].instruction, 5) == 0)
                break;
        }

        if ((uint) i >= g_exec_amount)
            exception_throw(EXCEPTION_RUNTIME,"Cannot load instruction. "
                            "It's either not supported or broken");

        width += 5;

        // Read [pad] bytes before checking for the newline
        for (int j = 0; j < g_execs[i].pad; j++) {
            fgetc(fp);
            width++;
        }

        while (fgetc(fp) != '\n')
            width++;

        fseek(fp, -width - 1, SEEK_CUR);
    }

    ins->size = width + 1;
    ins->content = vmalloc(sizeof(char) * (width + 1));
    ins->content[width] = 0;
    fread(ins->content, 1, width, fp);

    dprintf("Read [%d+1]<%.5s...>\n", width, ins->content);
    fgetc(fp);
}

static void load_instructions(struct SaltModule *module, FILE *fp, int instructions)
{
    dprintf("Loading instructions for '%s'\n", module->name);

    module->instructions = vmalloc(sizeof(String) * instructions);
    module->instruction_amount = instructions;

    for (int i = 0; i < instructions; i++) {
        read_instruction(&module->instructions[i], fp);
    }
}

static void load_labels(struct SaltModule *module)
{
    for (uint i = 0; i < module->instruction_amount; i++) {
        if (module->instructions[i].content[0] == '@')
            module->label_amount++;
    }

    module->labels = vmalloc(sizeof(uint) * module->label_amount);
    int curlabel = 0;
    for (uint i = 0; i < module->instruction_amount; i++) {
        if (module->instructions[i].content[0] == '@') {
            module->labels[curlabel] = i;
            curlabel++;
        }
    }
    dprintf("Found %d labels in '%s'\n", module->label_amount, module->name);
}

struct SaltModule *load(char *name)
{
    dprintf("Trying to load %s\n", name);

    // File
    int size = sizeof(char) * (strlen(name) + 5);
    
    char *filename = vmalloc(size);
    memset(filename, 0, size);

    strncpy(filename, name, strlen(name));

    FILE *mod = fopen(filename, "rb");
    if (!mod) {
        vmfree(filename, size);
        exception_throw(EXCEPTION_RUNTIME, "Cannot find module");
    }
    vmfree(filename, size);
    
    name[strlen(name) - 4] = 0;

    // Module

    struct SaltModule *module = module_create(name);
    struct LoaderHeaderData instructions = load_header(mod);

    load_instructions(module, mod, instructions.instructions);
    load_labels(module);

    register_control(instructions.registers);

    dprintf("Loaded module %s\n", module->name);

    fclose(mod);
    return module;
}

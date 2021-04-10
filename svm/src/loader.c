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
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 *
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

    u32 instructions;
    u8 registers;

};


static i32 validate_header(char *header)
{
    if (strncmp(header, SCC_HEADER, 8) != 0)
        return 0;

    if (* (u32 *) (header + 8) != SCC_VERSION)
        return 0;

    return 1;
}

static struct LoaderHeaderData load_header(SVMRuntime *_rt, FILE *fp)
{
    char *header = vmalloc(sizeof(char) * 64);
    fread(header, 1, 64, fp);

    if (!validate_header(header)) {
        vmfree(header, sizeof(char) * 64);
        exception_throw(_rt, EXCEPTION_RUNTIME, "Salt module is either invalid or corrupted");
    }

    struct LoaderHeaderData data;
    data.instructions = * (u32 *) (header + 16);
    data.registers    = * (u8  *) (header + 24); 

    vmfree(header, sizeof(char) * 64);
    return data;
}

static void read_instruction(SVMRuntime *_rt, String *ins, FILE *fp)
{
    // Check for label
    char label_char = fgetc(fp);
    i32 width = 0;

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

        i32 i = 0;
        for (; (u32) i < g_exec_amount; i++) {
            if (strncmp(buf, g_execs[i].instruction, 5) == 0)
                break;
        }

        if ((u32) i >= g_exec_amount)
            exception_throw(_rt, EXCEPTION_RUNTIME,"Cannot load instruction. "
                            "It's either not supported or broken");

        width += 5;

        // Read [pad] bytes before checking for the newline
        for (i32 j = 0; j < g_execs[i].pad; j++) {
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

    dprintf("Read [%d+1] %.5s", width, ins->content);
    fgetc(fp);
}


static void load_instructions(SVMRuntime *_rt, struct SaltModule *module, 
            FILE *fp, i32 instructions)
{
    dprintf("Loading instructions for '%s'", module->name);

    module->instructions = vmalloc(sizeof(String) * instructions);
    module->instruction_amount = instructions;

    for (i32 i = 0; i < instructions; i++) {
        read_instruction(_rt, &module->instructions[i], fp);
    }
}

static void load_labels(SVMRuntime *_rt, struct SaltModule *module)
{
    for (u32 i = 0; i < module->instruction_amount; i++) {
        if (module->instructions[i].content[0] == '@')
            module->label_amount++;
    }

    module->labels = vmalloc(sizeof(u32) * module->label_amount);
    i32 curlabel = 0;
    for (u32 i = 0; i < module->instruction_amount; i++) {
        if (module->instructions[i].content[0] == '@') {
            module->labels[curlabel] = i;
            curlabel++;
        }
    }
    dprintf("Found %d labels in '%s'", module->label_amount, module->name);
}

struct SaltModule *load(SVMRuntime *_rt, char *name)
{
    dprintf("Trying to load %s", name);

    // File
    i32 size = sizeof(char) * (strlen(name) + 5);
    
    char *filename = vmalloc(size);
    memset(filename, 0, size);

    strncpy(filename, name, strlen(name));

    FILE *mod = fopen(filename, "rb");
    if (!mod) {
        vmfree(filename, size);
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot find module");
    }
    vmfree(filename, size);
    
    name[strlen(name) - 4] = 0;

    // Module

    struct SaltModule *module = module_create(_rt, name);
    struct LoaderHeaderData instructions = load_header(_rt, mod);

    load_instructions(_rt, module, mod, instructions.instructions);
    load_labels(_rt, module);

    register_control(_rt, instructions.registers);

    dprintf("Loaded module %s", module->name);

    fclose(mod);
    return module;
}

static String *get_module_path(SVMRuntime *_rt, char *name)
{
    String *str = vmalloc(sizeof(String));
    str->size = 256;
    str->content = vmalloc(sizeof(char) * 256);
    memset(str->content, 0, 256);
    
    // Try local /ext

    snprintf(str->content, 255, "ext/%s.scc", name);
    FILE *fp = fopen(str->content, "r");
    if (fp) {
        fclose(fp);
        return str; 
    }

    // Try salt home /ext
    
#ifdef _WIN32
    const char *ext = getenv("SaltHome");
#else
    const char *ext = getenv("SALT_HOME");
#endif

    memset(str->content, 0, 256);
    snprintf(str->content, 255, "%s/ext/%s.scc", ext, name);
    FILE *efp = fopen(str->content, "r");
    if (efp) {
        fclose(efp);
        return str;
    }

    vmfree(str->content, str->size);
    vmfree(str, sizeof(String));
    exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot find '%s' module", name);

    return NULL;
}

struct SaltModule *ext_load(SVMRuntime *_rt, char *name)
{
    String *path = get_module_path(_rt, name);
    FILE *fp = fopen(path->content, "r");
    
    vmfree(path->content, path->size);
    vmfree(path, sizeof(String));

    struct SaltModule *module = module_create(_rt, name);
    struct LoaderHeaderData ins = load_header(_rt, fp);

    load_instructions(_rt, module, fp, ins.instructions);
    load_labels(_rt, module);

    register_control(_rt, ins.registers);

    dprintf("Loaded module '%s'", module->name);

    fclose(fp);
    return module;
}




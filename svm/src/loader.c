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
 * loader.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/loader.h"
#include "../include/utils.h"
#include "../include/exception.h"
#include "../include/exec.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define SVM_SCC_HEADER "\x7fSCC\xff\xee\0\0\0"
#define SVM_SCC_VERSION 3


i32 loader_validate_scc3_header(struct SCC3_Header *header)
{
    if (header->header != * (u64 *) SVM_SCC_HEADER)
        return 0;

    if (header->version != SVM_SCC_VERSION)
        return 0;

    return 1;
}

struct SCC3_Header loader_read_scc3_header(SVMRuntime *_rt, FILE *fp)
{
    char *header = vmalloc(sizeof(char) * 64);
    fread(header, 1, 64, fp);

    struct SCC3_Header data;
    data.header       = * (u64 *) (header + 0);
    data.version      = * (u32 *) (header + 8);
    data.instructions = * (u32 *) (header + 16);
    data.registers    = * (u8  *) (header + 24); 

    if (!loader_validate_scc3_header(&data)) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot read Salt Module. It's "
                "either the incorrect version or it's corrupted.");
    }

    vmfree(header, 64);
    return data;
}

static void read_instruction(SVMRuntime *_rt, String *ins, FILE *fp)
{
    char label_char = fgetc(fp);
    i32 width = 0;

    /* If it's a label, consume the whole thing. */
    if (label_char == '@') {
        while (fgetc(fp) != '\n')
            width++;
        fseek(fp, -width - 2, SEEK_CUR);
        width += 1;
    }

    /* Otherwise add the amount of initial padding, which it the amount
       of bytes that should be ignored when looking for the 0x0a byte,
       splitting instructions. Before doing anything, prepare a 5 byte 
       buffer and move the FILE cursor back by one char. */

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

        /* Read pad amount of chars before checking for the newline. */
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

    i32 size = sizeof(char) * (strlen(name) + 5);
    
    char *filename = vmalloc(size);
    memset(filename, 0, size);

#if !defined(__clang__)
    /* I know strcpy is bad, but mingw will not shut up aboout it and it
       doesn't trust me that the length of name will always be smaller 
       than the filename string buffer. I decided to fix this by giving the
       unsafe code just to mingw. */
    strcpy(filename, name);

#else
    /* But we'll provide the safe version for gcc & clang... */
    strncpy(filename, name, size - 5);

#endif

    FILE *mod = fopen(filename, "rb");
    if (!mod) {
        vmfree(filename, size);
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot find module");
    }
    vmfree(filename, size);
    
    name[strlen(name) - 4] = 0;

    struct SaltModule *module = module_create(_rt, name);
    struct SCC3_Header instructions = loader_read_scc3_header(_rt, mod);

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
    
    /* First, try finding the module in a local ext folder. */

    snprintf(str->content, 255, "ext/%s.scc", name);
    FILE *fp = fopen(str->content, "r");
    if (fp) {
        fclose(fp);
        return str; 
    }

    /* Otherwise, find the module in the salt home directory ext. */

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
    struct SCC3_Header ins = loader_read_scc3_header(_rt, fp);

    load_instructions(_rt, module, fp, ins.instructions);
    load_labels(_rt, module);

    register_control(_rt, ins.registers);

    dprintf("Loaded module '%s'", module->name);

    fclose(fp);
    return module;
}


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
 * @author bellrise
 */
#include <svm/svm.h>
#include <svm/loader.h>
#include <svm/utils.h>

#include <svm/dis/dis.h>
#include <svm/dis/rules.h>

#include <stdio.h>
#include <stdarg.h>
#include <string.h>


// CONST RULE TABLE

static const u32 rule_amount = 12;
static const struct DissasemblyRule rules[] = {
    {"CALLF", dis_rule_str},
    {"CXXEQ", dis_rule_id2},
    {"CXXLT", dis_rule_id2},
    {"EXTLD", dis_rule_str},
    {"IVADD", dis_rule_ivadd},
    {"IVSUB", dis_rule_ivadd},
    {"JMPFL", dis_rule_str},
    {"JMPNF", dis_rule_str},
    {"JMPTO", dis_rule_str},
    {"OBJMK", dis_rule_objmk},
    {"OBJDL", dis_rule_id},
    {"PRINT", dis_rule_id}
};

// UTIL FUNCTIONS

static void error(SVMRuntime *_rt, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    core_exit(_rt);
}


static void parse_label(SVMRuntime *_rt, u32 pos, String *ins)
{
    dprintf("Parsing: '%s'", ins->content + 1);

    /* If we find a dot in the label name, it means it's a jump-to label,
       not a function label. */

    // TODO: This should be smarter and possibly use regex or something.

    for (u64 i = 0; i < ins->size; i++) {
        if (ins->content[i] == '.') {
            while (pos >= 10000)
                pos -= 10000;
            printf("     %05u\t     @\t\t%s\n", pos, ins->content + 1);
            return;
        }
    }

    if (ins->content[1] == '%') {
        printf("\n%010u: private %s (...)\n", pos, ins->content + 2);
    }
    else {
        printf("\n%010u: public %s (...)\n", pos, ins->content + 1);
    }

}

static void parse_instruction(SVMRuntime *_rt, u32 pos, String *ins)
{
    dprintf("Parsing: '%.5s'", ins->content);

    // Remove trailing number from instruction position
    while (pos >= 10000)
        pos -= 10000;

    String info = {0, NULL};

    // If a rule exists for this instruction, use it.
    for (u32 i = 0; i < rule_amount; i++) {
        if (strncmp(rules[i].instruction, ins->content, 5) == 0) {
            info = rules[i].dis_rule(_rt, ins->content + 5);
            break;
        }
    }

    // Assign empty string if instruction is unknown.
    if (!info.size) {
        info.content = vmalloc(sizeof(char));
        *info.content = 0;
        info.size = 1;
    }

    printf("     %05u\t %.5s\t\t%s\n", pos, ins->content, info.content);
    vmfree(info.content, info.size);
}

i32 disassemble(SVMRuntime *_rt, char *filename)
{
    char *path = vmalloc(strlen(filename) + 4);
    snprintf(path, strlen(filename) + 4, "%s.scc", filename);

    dprintf("Trying to open '%s'", path);

    FILE *fp = fopen(filename, "r");
    vmfree(path, strlen(filename) + 4);

    if (!fp) {
        error(_rt, "Cannot open '%s' for disassembling\n", filename);
    }

    /* If we manage to open the file, read & validate the SCC Header.
       When this operation succeeds, load() the compiled code and continue
       with the disassembling process. */

    struct SCC3_Header header = loader_read_scc3_header(_rt, fp);
    if (!loader_validate_scc3_header(&header)) {
        error(_rt, "Cannot load SCC file, it's either invalid or corrupted\n");
    }

    struct SaltModule *module = load(_rt, filename);

    /* Print some meta data onto the screen. */

    printf("Disassembling %s.scc", filename);

    printf("\nHeader:\n");

    printf("\tVersion:\t%ld\n", header.h_version);
    printf("\tRegisters:\t%d\n", header.m_reg);
    printf("\tInstructions:\t%d\n", header.m_ins);

    /* Parse code section. */

    for (u32 i = 0; i < module->instruction_amount; i++) {
        if (module->instructions[i].content[0] == '@') {
            parse_label(_rt, i, &module->instructions[i]);
        }
        else {
            parse_instruction(_rt, i, &module->instructions[i]);
        }
    }

    return 0;
}

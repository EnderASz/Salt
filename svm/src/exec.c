/**
 * exec.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/exec.h"
#include "../include/exception.h"
#include "../include/callstack.h"
#include "../include/utils.h"
#include "../include/module.h"
#include "../include/object.h"

#include <string.h>

/* killx is the last instruction if no other is found. */
static struct SVMCall g_execs[] = {
        
    {"KILLX", exec_killx},
    {"REGMV", exec_regmv},
    {"REGST", exec_regst},
    {"EXITE", exec_exite},
    {"CALLF", exec_callf},
    {"EXTLD", exec_extld},
    {"OBJMK", exec_objmk},
    {"OBJDL", exec_objdl},
    {"PRINT", exec_print},
    {"REGDP", exec_regdp},
    {"RETRN", exec_retrn},
    {"REGNL", exec_regnl}

};

static const uint g_exec_amount = 12;

/* global registers */
static SaltObject *g_registers[8] = {
        NULL, NULL, NULL, NULL,
        NULL, NULL, NULL, NULL
};

static const uint g_register_amount = 8;


static uint exec_find_end(struct SaltModule *module)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strncmp(content, "@$__END__", 9) == 0)
            return module->labels[i];
    }
    exception_throw(EXCEPTION_RUNTIME, "Cannot find end label");
    return 0;
}


static uint preload(struct SaltModule *main)
{
    uint i = 0;
    for (uint j = 0; j < main->label_amount; j++) {
        if (strncmp(main->instructions[main->labels[j]].content, "@main", 5) == 0) {
            dprintf("Found main function at [%d]\n", main->labels[j]);
            i = main->labels[j];
        }
    }

    if (i == 0)
        exception_throw(EXCEPTION_RUNTIME, "main function not found");

    return i;
}

/* main exec */

int exec(struct SaltModule *main)
{
    dprintf("Executing '%s'\n", main->name);

    uint i = preload(main);

    // "Call" the main instruction
    callstack_push(exec_find_end(main), main->name, "main");

    for (; i < main->instruction_amount;) {

        if (*main->instructions[i].content == '@') {
            i++;
            continue;
        }

        dprintf("[%d] < \033[95m%s\033[0m >\n", i, main->instructions[i].name);

        struct SVMCall *exec = exec_get(main->instructions[i].name);
        i = exec->f_exec(main, (byte *) main->instructions[i].content + 5, i);
    }
    return 0;
}

void register_clear()
{
    dprintf("Clearing registers\n");
    for (uint i = 0; i < g_register_amount; i++) {

        if (g_registers[i] != NULL) {
            vmfree(g_registers[i]->value, g_registers[i]->size);
            vmfree(g_registers[i], sizeof(SaltObject));
            g_registers[i] = NULL;
        }

    }
}

struct SVMCall *exec_get(char *title)
{
    for (uint i = 0; i < g_exec_amount; i++) {
        if (strncmp(g_execs[i].instruction, title, 5) == 0)
            return &g_execs[i];
    }
    return &g_execs[0];
}

uint exec_callf(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    uint strl = * (uint * ) payload;
    char *name = vmalloc(sizeof(char) * strl + 1);
    strncpy(name, (char *) payload + 4, strl);
    name[strl - 1] = 0;

    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strcmp(content + 1, name) == 0) {
            callstack_push(pos, module->name, name);
            vmfree(name, strl + 1);
            return module->labels[i];
        }
    }

    vmfree(name, strl + 1);
    exception_throw(EXCEPTION_LABEL, "Cannot find function");
    return 0;
}

uint exec_exite(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    return exec_find_end(module);
}

uint exec_extld(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    exception_throw(EXCEPTION_RUNTIME, "EXTLD is not implemented yet");
    return ++pos;
}

uint exec_killx(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    core_exit();
    return ++pos;
}

uint exec_objmk(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    SaltObject *obj = module_object_acquire(module);
    salt_object_define(obj, payload);
    return ++pos;
}

uint exec_objdl(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    module_object_delete(module, * (uint *) payload);
    return ++pos;
}

uint exec_print(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    SaltObject *obj = module_object_find(module, * (uint *) payload);
    if (obj == NULL) {
        exception_throw(EXCEPTION_NULLPTR, "Cannot find object %d", 
                        * (uint *) payload);
    }

    salt_object_print(obj);
    return ++pos;
}

uint exec_retrn(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    struct StackFrame *frame = callstack_peek();
    if (frame == NULL) {
        pos = exec_find_end(module);
    }
    else {
        pos = frame->line + 1;
        if (module->instruction_amount <= pos - 1) {
            pos--;
        }
    }
    dprintf("Jumping back to [%d]\n", pos);
    callstack_pop();
    return ++pos;
}

uint exec_regdp(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    exception_throw(EXCEPTION_RUNTIME, "REGDP is not implemented yet");
    return ++pos;
}

uint exec_regmv(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    exception_throw(EXCEPTION_RUNTIME, "REGMV is not implemented yet");
    return ++pos;
}

uint exec_regnl(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    register_clear();
    return ++pos;
}

uint exec_regst(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    exception_throw(EXCEPTION_RUNTIME, "REGST is not implemented yet");
    return ++pos;
}

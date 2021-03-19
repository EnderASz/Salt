//
// Created by bellrise on 18.03.2021.
//
#include "../include/exec.h"
#include "../include/exception.h"
#include "../include/callstack.h"
#include <string.h>

/* killx is the last instruction if no other is found. */
static struct SVMCall g_execs[] = {
    {"KILLX", exec_killx},
    {"EXITE", exec_exite},
    {"EXTLD", exec_extld},
    {"OBJMK", exec_objmk},
    {"OBJDL", exec_objdl},
    {"PRINT", exec_print},
    {"RETRN", exec_retrn},
};

static uint g_exec_amount = 7;

static uint preload(struct SaltModule *main)
{
    uint i = 0;
    for (uint j = 0; j < main->label_amount; j++) {
        if (strcmp(main->instructions[main->labels[j]].content, "@main") == 0) {
            dprintf("Found main function at [%d]\n", main->labels[j]);
            i = main->labels[j];
        }
    }

    if (i == -1)
        exception_throw(EXCEPTION_RUNTIME, "main function not found");

    return i;
}

/* main exec */

int exec(struct SaltModule *main)
{
    dprintf("Executing '%s'\n", main->name);

    uint i = preload(main);

    // "Call" the main instruction
    callstack_push(i, main->name, "main");
    callstack_push(i, main->name, "main2");
    callstack_push(i, main->name, "main3");

    for (; i < main->instruction_amount; ) {

        if (*main->instructions[i].content == '@') {
            i++;
            continue;
        }

        dprintf("[%d] Trying to execute '%s'\n", i, main->instructions[i].name);

        struct SVMCall *exec = exec_get(main->instructions[i].name);
        if (exec == NULL)
            continue;

        i = exec->f_exec(main, (byte *) main->instructions[i].content + 5, i);
    }
    return 0;
}


struct SVMCall *exec_get(char *title)
{
    for (uint i = 0; i < g_exec_amount; i++) {
        if (strncmp(g_execs[i].instruction, title, 5) == 0)
            return &g_execs[i];
    }
    return &g_execs[0];
}

int exec_exite(struct SaltModule *module, byte *payload, int pos)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strcmp(content, "@$__END__") == 0)
            return i;
    }
    exception_throw(EXCEPTION_RUNTIME, "Cannot find end label");
    return ++pos;
}

int exec_extld(struct SaltModule *module, byte *payload, int pos)
{
    exception_throw(EXCEPTION_RUNTIME, "EXTLD is not implemented yet");
    return ++pos;
}

int exec_killx(struct SaltModule *module, byte *payload, int pos)
{
    core_exit();
    return ++pos;
}

int exec_objmk(struct SaltModule *module, byte *payload, int pos)
{
    SaltObject *obj = module_object_acquire(module);
    salt_object_define(obj, payload);
    return ++pos;
}

int exec_objdl(struct SaltModule *module, byte *payload, int pos)
{
    exception_throw(EXCEPTION_RUNTIME, "OBJDL is not implemented yet");
    return ++pos;
}

int exec_print(struct SaltModule *module, byte *payload, int pos)
{
    SaltObject *obj = module_object_find(module, * (uint *) payload);
    salt_object_print(obj);
    return ++pos;
}

int exec_retrn(struct SaltModule *module, byte *payload, int pos)
{
    exception_throw(EXCEPTION_RUNTIME, "RETRN is not implemented yet");
    return ++pos;
}

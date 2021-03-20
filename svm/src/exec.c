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
    {"CALLF", exec_callf},
    {"EXTLD", exec_extld},
    {"OBJMK", exec_objmk},
    {"OBJDL", exec_objdl},
    {"PRINT", exec_print},
    {"RETRN", exec_retrn},
};

static uint g_exec_amount = 8;

static int exec_find_end(struct SaltModule *module)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strcmp(content, "@$__END__") == 0)
            return module->labels[i];
    }
    exception_throw(EXCEPTION_RUNTIME, "Cannot find end label");
    return -1;
}


static uint preload(struct SaltModule *main)
{
    uint i = 0;
    for (uint j = 0; j < main->label_amount; j++) {
        if (strcmp(main->instructions[main->labels[j]].content, "@main") == 0) {
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

    for (; i < main->instruction_amount; ) {

        if (*main->instructions[i].content == '@') {
            i++;
            continue;
        }

        dprintf("[%d] Trying to execute '%s'\n", i, main->instructions[i].name);

        struct SVMCall *exec = exec_get(main->instructions[i].name);
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

int exec_callf(struct SaltModule *module, byte *payload, int pos)
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

int exec_exite(struct SaltModule *module, byte *payload, int pos)
{
    return exec_find_end(module);
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
    module_object_delete(module, * (uint *) payload);
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
    return pos;
}

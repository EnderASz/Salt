//
// Created by bellrise on 18.03.2021.
//
#include "../include/exec.h"
#include "../include/exception.h"
#include <string.h>

static struct SVMCall g_execs[] = {
    {"EXITE", exec_exite},
    {"EXTLD", exec_extld},
    {"OBJMK", exec_objmk},
    {"OBJDL", exec_objdl},
    {"PRINT", exec_print},
    {"RETRN", exec_retrn}
};

static uint g_exec_amount = 6;

struct SVMCall *exec_get(char *title)
{
    for (uint i = 0; i < g_exec_amount; i++) {
        if (strncmp(g_execs[i].instruction, title, 5) == 0)
            return &g_execs[i];
    }
    return NULL;
}

int exec_exite(struct SaltModule *module, int pos)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strcmp(content, "@$__END__") == 0)
            return i;
    }
    exception_throw(EXCEPTION_RUNTIME, "Cannot find end label");
    return 0;
}

int exec_extld(struct SaltModule *module, int pos)
{

}

int exec_objmk(struct SaltModule *module, int pos)
{

}

int exec_objdl(struct SaltModule *module, int pos)
{

}

int exec_print(struct SaltModule *module, int pos)
{

}

int exec_retrn(struct SaltModule *module, int pos)
{

}

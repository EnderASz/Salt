//
// module.h implementation
//
#include "../include/module.h"
#include "../include/exception.h"
#include <string.h>

static uint g_module_size = 0;
static uint g_module_space = 0;
static struct SaltModule* g_modules = NULL;

static struct SaltModule *module_aquire_new()
{
    if (g_module_space == 0) {
        g_module_space++;
        g_modules = vmalloc(sizeof(struct SaltModule));
    }

    if (g_module_size >= g_module_space) {
        g_module_space++;
        g_modules = vmrealloc(g_modules, sizeof(struct SaltModule)
                * g_module_space - 1, sizeof(struct SaltModule)
                * g_module_space);
    }

    g_module_size++;
    return &g_modules[g_module_size - 1];
}

struct SaltModule* module_create(char *name)
{
    struct SaltModule *mod = module_aquire_new();
    mod->open_byte = 0x01;

    if (strlen(name) > 62) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    mod->object_amount = 0;
    mod->objects = NULL;

    mod->instruction_amount = 0;
    mod->instructions = NULL;

    mod->function_ptr_amount = 0;
    mod->function_ptr = NULL;

    return mod;
}

static void module_deallocate(struct SaltModule *module)
{
    // Functions pointers
    vmfree(module->function_ptr, module->function_ptr_amount * sizeof(uint));

    // Instructions
    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(struct SaltInstruction));

    // Objects
    for (uint i = 0; i < module->object_amount; i++) {
        module->objects[i].destructor(&module->objects[i]);
    }
    vmfree(module->objects, module->object_amount * sizeof(struct SaltObject));
}

void module_delete_all()
{
    for (uint i = 0; i < g_module_size; i++) {
        module_deallocate(&g_modules[i]);
    }

    vmfree(g_modules, g_module_space * sizeof(struct SaltModule));
}
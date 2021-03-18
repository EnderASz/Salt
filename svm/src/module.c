//
// module.h implementation
//
#include "../include/module.h"
#include "../include/exception.h"
#include <string.h>

static uint g_module_size = 0;
static uint g_module_space = 0;
static struct SaltModule* g_modules = NULL;

static struct SaltModule *module_acquire_new()
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
    struct SaltModule *mod = module_acquire_new();

    if (strlen(name) > 62) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    strncpy(mod->name, name, strlen(name));

    mod->objects_size = 0;
    mod->objects_locked = 0;
    mod->objects_space = 0;
    mod->objects = NULL;

    mod->instruction_amount = 0;
    mod->instructions = NULL;

    mod->label_amount = 0;
    mod->labels = NULL;

    return mod;
}

static void compress_object_container(struct SaltModule *module)
{
    // todo: container compression
    // dear future me, you wanted to compress the module.objects array by moving
    // over the pointers to places where there are 'locked' objects, but then you
    // realised you need to copy over the whole object because the memory where
    // the objects were before is going to be free'd so a segfault will happen.
    // I don't have the brain power now at 20 past midnight, but I know you will
    // later.
    //
    // Also, be sure to not leave any null pointers inside the list, because
    // there are functions checking each element and the pointers can't point
    // to nothing right?
    //                                                   Have fun, bellrise <3
}

static void control_allocation(struct SaltModule *module)
{
    if (module->objects_size >= module->objects_space) {
        uint new_size = MODULE_OBJECT_SPACE + module->objects_space;
        module->objects = vmrealloc(
            module->objects,
            sizeof(SaltObject) * module->objects_space,
            sizeof(SaltObject) * new_size
        );
        module->objects_space = new_size;
    }

    if (module->objects_space - MODULE_OBJECT_SPACE >= module->objects_size) {
        compress_object_container(module);
    }
}

SaltObject *module_acquire_new_object(struct SaltModule *module)
{
    // todo: dont return null?
    return NULL;
}

void module_delete_object(struct SaltModule *module, uint id)
{
    for (uint i = 0; i < module->objects_size; i++) {
        if (module->objects[i].id == id && !module->objects[i].locked) {
            module->objects[i].locked = 1;
            module->objects_locked++;
            return;
        }
    }
}

static void module_deallocate(struct SaltModule *module)
{
    // Labels
    vmfree(module->labels, module->label_amount * sizeof(uint));

    // Instructions
    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(struct SaltInstruction));

    // Objects
    for (uint i = 0; i < module->objects_size; i++) {
        module->objects[i].destructor(&module->objects[i]);
    }
    vmfree(module->objects, module->objects_space * sizeof(struct SaltObject));
}

void module_delete_all()
{
    for (uint i = 0; i < g_module_size; i++) {
        dprintf("Removed module: %s\n", g_modules[i].name);
        module_deallocate(&g_modules[i]);
    }

    vmfree(g_modules, g_module_space * sizeof(struct SaltModule));
}

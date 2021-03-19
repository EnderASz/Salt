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

static void collapse_object_nodes(struct SaltObjectNode *node)
{
    // If the current node is null, it means there is no objects to be collapsed.
    // This needs to be returned early in this case, because there is the while
    // loop which depends on the next element in the node.
    if (node == NULL)
        return;

    // This collapses each node from the back, making this a very intensive
    // operation which should be done only at the end when removing the module.
    // I'll explain how it works because the next time you'll look at this code
    // you'll have no idea what this does: This finds the last node in the
    // linked list and then removes it from memory, setting the pointer to the
    // last object from the previous object to NULL. This is done until every
    // element from the list is removed.

    struct SaltObjectNode *current_node  = node;
    struct SaltObjectNode *previous_node = NULL;

    while (node != NULL) {

        while (1) {
            if (current_node->next == NULL)
                break;
            previous_node = current_node;
            current_node = current_node->next;
        }

        // Unlink
        previous_node->next = NULL;
        current_node->object.destructor(&current_node->object);
        vmfree(current_node, sizeof(struct SaltObjectNode));

        // Reset data
        current_node  = node;
        previous_node = NULL;
    }
}

struct SaltModule* module_create(char *name)
{
    struct SaltModule *mod = module_acquire_new();

    if (strlen(name) > 62) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    strncpy(mod->name, name, strlen(name));

    mod->objects = NULL;

    mod->instruction_amount = 0;
    mod->instructions = NULL;

    mod->label_amount = 0;
    mod->labels = NULL;

    return mod;
}

SaltObject *module_object_acquire(struct SaltModule *module)
{
    // Find last in linked list
    struct SaltObjectNode *node = NULL;
    do {
        node = node->next;
    } while (node != NULL);

    node = vmalloc(sizeof(struct SaltObjectNode));
    node->next = NULL;

    node->object.id = 0;
    node->object.type = OBJECT_TYPE_NULL;
    node->object.access = ACCESS_PUBLIC;
    node->object.readonly = READONLY_FALSE;
    node->object.mutex_aquired = 0;

    node->object.value = NULL;
    node->object.size = 0;

    node->object.destructor = salt_object_destructor;
    node->object.constructor = NULL;

    return node;
}

void module_object_delete(struct SaltModule *module, uint id)
{
    // todo: remove objects by relinking
}

static void module_deallocate(struct SaltModule *module)
{
    vmfree(module->labels, module->label_amount * sizeof(uint));

    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(struct SaltInstruction));

    collapse_object_nodes(module->objects);
}

void module_delete_all()
{
    for (uint i = 0; i < g_module_size; i++) {
        dprintf("Removed module: %s\n", g_modules[i].name);
        module_deallocate(&g_modules[i]);
    }

    vmfree(g_modules, g_module_space * sizeof(struct SaltModule));
}

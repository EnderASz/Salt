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

// Node operations

static void nodes_collapse(struct SaltModule *module)
{
    struct SaltObjectNode *node = module->object_first;
    while (node->next != NULL) {
        node = node->next;
        dprintf("Collapsing %p : %d : %p\n", node->previous, node->data.id, node->next);
        node->previous->data.destructor(&node->previous->data);
        vmfree(node->previous, sizeof(struct SaltObjectNode));
        node->previous = NULL;
    }
    // Free self
    node->data.destructor(&node->data);
    vmfree(node, sizeof(struct SaltObjectNode));
}

SaltObject *module_object_acquire(struct SaltModule *module)
{
    struct SaltObjectNode *new_node = vmalloc(sizeof(struct SaltObjectNode));
    new_node->next = NULL;
    new_node->previous = module->object_last;

    module->object_last->next = new_node;
    module->object_last = new_node;

    salt_object_init(&new_node->data);
    return &new_node->data;
}

SaltObject *module_object_find(struct SaltModule *module, uint id)
{
    struct SaltObjectNode *node = module->object_first;
    while (node != NULL) {
        if (node->data.id == id)
            return &node->data;
        node = node->next;
    }
    // Return null value otherwise
    return &module->object_first->data;
}

void module_object_delete(struct SaltModule *module, uint id)
{
    if (id == 0)
        exception_throw(EXCEPTION_RUNTIME, "Cannot remove object of ID 0");

    struct SaltObjectNode *node = module->object_first;
    while (node != NULL) {
        if (node->data.id == id) {

            // Last element in object list
            if (node->next == NULL) {
                dprintf("Unlinking object {%d} where next is NULL\n", id);
                node->previous->next = NULL;
            }

            // Somewhere in the middle
            else {
                dprintf("Unlinking object {%d} where next points to other\n", id);
                node->previous->next = node->next;
                node->next->previous = node->previous;
            }

            node->data.destructor(&node->data);
            vmfree(node, sizeof(struct SaltObjectNode));

        }
        node = node->next;
    }
}


// Module operations

struct SaltModule* module_create(char *name)
{
    struct SaltModule *mod = module_acquire_new();

    if (strlen(name) > 62) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot open module");
    }

    strncpy(mod->name, name, strlen(name));

    mod->instruction_amount = 0;
    mod->instructions = NULL;

    mod->label_amount = 0;
    mod->labels = NULL;

    // Linked list requires to have at least one element, in this case
    // the default object at location one is the NULLPTR object (id 0)
    struct SaltObjectNode *node = vmalloc(sizeof(struct SaltObjectNode));
    node->next = NULL;
    node->previous = NULL;

    salt_object_init(&node->data);

    mod->object_first = node;
    mod->object_last = node;

    return mod;
}

static void module_deallocate(struct SaltModule *module)
{
    vmfree(module->labels, module->label_amount * sizeof(uint));

    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(struct SaltInstruction));

    nodes_collapse(module);
}

void module_delete_all()
{
    for (uint i = 0; i < g_module_size; i++) {
        dprintf("Removed module: %s\n", g_modules[i].name);
        module_deallocate(&g_modules[i]);
    }

    vmfree(g_modules, g_module_space * sizeof(struct SaltModule));
}

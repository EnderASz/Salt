/**
 * module.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/module.h"
#include "../include/exception.h"
#include "../include/utils.h"

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
    dprintf("Collapsing nodes from the top\n");

    struct SaltObjectNode *node = module->head;
    struct SaltObjectNode *hook = NULL;

    if (node == NULL)
        return;

    while (1) {
        dprintf("Collapsing : %p : {%d}\n", (void *) node, node->data.id);

        // Assign the pointer to the next element to the hook and check if it's
        // not null. This is used because we are free-ing memory right under
        // out feet, and we still need to know the location of the next object.
        hook = node->next;
        if (hook == NULL)
            break;

        vmfree(node->data.value, node->data.size);
        vmfree(node, sizeof(struct SaltObjectNode));

        // And set the pointer to the current work node to the hook to the next
        // object.
        node = hook;
    }

    // Delete ourself when we reach the last object
    vmfree(node->data.value, node->data.size);
    vmfree(node, sizeof(struct SaltObjectNode));
}

SaltObject *module_object_acquire(struct SaltModule *module)
{
    dprintf("Acquiring new object\n");
    struct SaltObjectNode *new_node = vmalloc(sizeof(struct SaltObjectNode));

    // Setup the new node
    new_node->next = module->head;
    new_node->previous = NULL;
    salt_object_init(&new_node->data);

    // Make the next node point to us but only if we're not the only ones 
    // in the list.
    if (new_node->next != NULL)
        new_node->next->previous = new_node;

    // Make the head point to the new node
    module->head = new_node;

    return &new_node->data;
}

Nullable SaltObject *module_object_find(struct SaltModule *module, uint id)
{
    struct SaltObjectNode *node = module->head;
    while (node != NULL) {
        if (node->data.id == id)
            return &node->data;
        node = node->next;
    }
    // Return null value otherwise
    return NULL;
}

void module_object_delete(struct SaltModule *module, uint id)
{
    if (id == 0)
        exception_throw(EXCEPTION_RUNTIME, "Cannot remove object of ID 0");

    struct SaltObjectNode *node = module->head;
    int8_t nf_flag = 1;

    while (1) {

        if (node == NULL)
            break;

        // When the object is found, unlink & free it
        if (node->data.id == id) {
            
            if (node->previous == NULL) {
                // If it's the first element of this list
                dprintf("Removing {%d} at the beginning\n", node->data.id);
                module->head = module->head->next;
                if (module->head != NULL)
                    module->head->previous = NULL;
            }
            else if (node->next == NULL) {
                // If it's the last object in the list, set the pointer to the
                // next node in the previous one to null.
                dprintf("Removing {%d} at the end\n", node->data.id);
                node->previous->next = NULL;
            }
            else {
                // If it's somewhere in the middle...
                dprintf("Removing {%d} in the middle\n", node->data.id);
                node->previous->next = node->next;
                node->next->previous = node->previous;    
            }

            nf_flag = 0;
            vmfree(node->data.value, node->data.size);
            vmfree(node, sizeof(struct SaltObjectNode));

            break;
        }

        node = node->next;
    }

    if (nf_flag)
        exception_throw(EXCEPTION_RUNTIME, "Cannot find object of ID %d\n", id);
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

    mod->head = NULL;

    return mod;
}

static void module_deallocate(struct SaltModule *module)
{
    dprintf("Clearing module '%s'\n", module->name);
    dprintf("Deallocating label list\n");
    vmfree(module->labels, module->label_amount * sizeof(uint));

    dprintf("Deallocating instructions\n");
    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(String));

    nodes_collapse(module);
}

void module_delete_all()
{
    for (uint i = 0; i < g_module_size; i++)
        module_deallocate(&g_modules[i]);

    vmfree(g_modules, g_module_space * sizeof(struct SaltModule));
}

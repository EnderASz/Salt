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
 * module.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/module.h"
#include "../include/exception.h"
#include "../include/utils.h"

#include <string.h>


static struct SaltModule *module_acquire_new(SVMRuntime *_rt)
{
    dprintf("Acquiring new module (%d)", _rt->module_size + 1);
    _rt->modules = vmrealloc(
        _rt->modules, 
        sizeof(struct SaltModule *) * (_rt->module_size),
        sizeof(struct SaltModule *) * (_rt->module_size + 1)
    );
    _rt->module_size++;
    
    _rt->modules[_rt->module_size - 1] = vmalloc(sizeof(struct SaltModule));
    return _rt->modules[_rt->module_size - 1];
}

static void nodes_collapse(SVMRuntime *_rt, struct SaltModule *module)
{
    dprintf("Collapsing nodes from the top");

    struct SaltObjectNode *node = module->head;
    struct SaltObjectNode *hook = NULL;

    if (node == NULL)
        return;

    while (1) {
        dprintf("Collapsing : %p : {%d}", (void *) node, node->data.id);

        /* Assign the pointer to the next element to the hook and check if it's
           not null. This is used because we are free-ing memory right under
           our feet, and we still need to know the location of the next object.
           This way we can determine when to exit. */

        hook = node->next;
        if (hook == NULL)
            break;

        vmfree(node->data.value, node->data.size);
        vmfree(node, sizeof(struct SaltObjectNode));

        /* Move the node one forward. */

        node = hook;
    }

    /* Free the last object in the array. */

    vmfree(node->data.value, node->data.size);
    vmfree(node, sizeof(struct SaltObjectNode));
}

SaltObject *module_object_acquire(SVMRuntime *_rt, struct SaltModule *module)
{
    dprintf("Acquiring new object");
    struct SaltObjectNode *new_node = vmalloc(sizeof(struct SaltObjectNode));

    /* Setup the new node first to not get any casual segfaults. */
    
    new_node->next = module->head;
    new_node->previous = NULL;
    
    new_node->data.ctor = salt_object_ctor;
    new_node->data.dtor = salt_object_dtor;
    
    new_node->data.ctor(_rt, &new_node->data);

    if (new_node->next != NULL)
        new_node->next->previous = new_node;

    /* Move the head pointer to the newly created node, "moving it back by 
       one element". */

    module->head = new_node;

    return &new_node->data;
}

SaltObject *module_object_find(struct SaltModule *module, u32 id) Nullable
{
    struct SaltObjectNode *node = module->head;
    while (node != NULL) {
        if (node->data.id == id)
            return &node->data;
        node = node->next;
    }
    return NULL;
}

void module_object_delete(SVMRuntime *_rt, struct SaltModule *module, u32 id)
{
    if (id == 0)
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot remove object of ID 0");

    struct SaltObjectNode *node = module->head;
    i8 nf_flag = 1;

    while (1) {

        if (node == NULL)
            break;

        if (node->data.id == id) {
            
            /* There are 3 cases here, depending where we are in the list. We
               don't want to segfault here! */

            if (node->previous == NULL) {
                dprintf("Removing {%d} at the beginning", node->data.id);
                module->head = module->head->next;
                if (module->head != NULL)
                    module->head->previous = NULL;
            }
            else if (node->next == NULL) {
                dprintf("Removing {%d} at the end", node->data.id);
                node->previous->next = NULL;
            }
            else {
                dprintf("Removing {%d} in the middle", node->data.id);
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
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot find object of ID %d", id);
}

struct SaltModule* module_create(SVMRuntime *_rt, char *name)
{
    struct SaltModule *mod = module_acquire_new(_rt);

    if (strlen(name) > 62) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot open module");
    }

    strncpy(mod->name, name, strlen(name));

    /* Again, we don't want to segfault. */

    mod->instruction_amount = 0;
    mod->instructions = NULL;

    mod->module_amount = 0;
    mod->modules = NULL;

    mod->label_amount = 0;
    mod->labels = NULL;

    mod->head = NULL;

    return mod;
}

static void module_deallocate(SVMRuntime *_rt, struct SaltModule *module)
{
    /* We want to deallocate everything in one place, to be sure it happened. */

    dprintf("Clearing module '%s'", module->name);
    dprintf("Deallocating label list");
    vmfree(module->labels, module->label_amount * sizeof(u32));

    dprintf("Deallocating instructions");
    for (u32 i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(String));

    dprintf("Deallocting module pointer array");
    vmfree(module->modules, sizeof(struct SaltModule *) * module->module_amount);  

    nodes_collapse(_rt, module);
    vmfree(module, sizeof(struct SaltModule));
}

void module_delete_all(SVMRuntime *_rt)
{
    for (u32 i = 0; i < _rt->module_size; i++)
        module_deallocate(_rt, _rt->modules[i]);

    vmfree(_rt->modules, _rt->module_size * sizeof(struct SaltModule *));
}


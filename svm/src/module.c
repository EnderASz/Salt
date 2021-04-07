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
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 *
 * module.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/module.h"
#include "../include/exception.h"
#include "../include/utils.h"

#include <string.h>

static struct SaltModule *module_acquire_new(SVMRuntime *_rt)
{
    dprintf("Acquiring new module (%d)\n", _rt->module_size + 1);
    _rt->modules = vmrealloc(
        _rt->modules, 
        sizeof(struct SaltModule *) * (_rt->module_size),
        sizeof(struct SaltModule *) * (_rt->module_size + 1)
    );
    _rt->module_size++;
    
    _rt->modules[_rt->module_size - 1] = vmalloc(sizeof(struct SaltModule));
    return _rt->modules[_rt->module_size - 1];
}

// Node operations

static void nodes_collapse(SVMRuntime *_rt, struct SaltModule *module)
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

SaltObject *module_object_acquire(SVMRuntime *_rt, struct SaltModule *module)
{
    dprintf("Acquiring new object\n");
    struct SaltObjectNode *new_node = vmalloc(sizeof(struct SaltObjectNode));

    // Setup the new node
    new_node->next = module->head;
    new_node->previous = NULL;
    
    new_node->data.ctor = salt_object_ctor;
    new_node->data.dtor = salt_object_dtor;
    
    new_node->data.ctor(_rt, &new_node->data);

    // Make the next node point to us but only if we're not the only ones 
    // in the list.
    if (new_node->next != NULL)
        new_node->next->previous = new_node;

    // Make the head point to the new node
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
    // Return null value otherwise
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
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot find object of ID %d\n", id);
}


// Module operations

struct SaltModule* module_create(SVMRuntime *_rt, char *name)
{
    struct SaltModule *mod = module_acquire_new(_rt);

    if (strlen(name) > 62) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot open module");
    }

    strncpy(mod->name, name, strlen(name));

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
    dprintf("Clearing module '%s'\n", module->name);
    dprintf("Deallocating label list\n");
    vmfree(module->labels, module->label_amount * sizeof(u32));

    dprintf("Deallocating instructions\n");
    for (u32 i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size);
    }
    vmfree(module->instructions, module->instruction_amount * sizeof(String));

    dprintf("Deallocting module pointer array\n");
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



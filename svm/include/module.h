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
 * This module provides the SaltModule structure and the related functions
 * for creating and modifying such salt structure.
 *
 * @author  bellrise, 2021
 */
#ifndef SVM_MODULE_H
#define SVM_MODULE_H

#include "core.h"

#define MODULE_OBJECT_SPACE 32

/**
 * Objects in the SaltModule are stored in a linked list for greater efficiency
 * when removing objects, which do not require any data movement when stored in
 * a linked list. 
 *
 * @a previous  pointer to previous node
 * @a next      pointer to next node
 * @a data      the actual data
 */
struct SaltObjectNode {

    struct SaltObjectNode *previous;
    struct SaltObjectNode *next;

    SaltObject data;

};

/** 
 * This container stores a single loaded salt module. One module translates
 * one compiled .scc file. The big difference between SCC format 1 and format 3
 * is the salt modules. Because there would be namespace clashes for externally
 * loaded modules, every module loaded with EXTLD is put into its own Salt
 * module, and then linked live during execution. 
 *
 * @a name              name of the module; can only be 63 chars long, this
 *                      should be controlled by the compiler to raise an 
 *                      error if the user tries to do so
 * @a object_first      pointer to the first object
 * @a module_amount     size of module pointer array
 * @a modules           this is an array of pointers to modules; in order to 
 *                      not allow global imports, modules have to be arranged 
 *                      in a tree style fashion. This is done by storing an 
 *                      array of pointers to the modules, which are all stored
 *                      in the runtime module array. This way you can only 
 *                      access modules which you have imported, but a module
 *                      is never imported twice.
 * @a instruction_amount  size of instruction array
 * @a instructions      array of instructions; each instruction is represented
 *                      as a SaltInstruction structure
 * @a label_amount      size of label array
 * @a labels            array of labels; each label is just represented with 
 *                      the line number it's located on; this allows the exec
 *                      to only look at label lines when looking up a function
 *                      to jump to instead of all the instructions
 */
struct SaltModule {

    char    name[64];

    struct  SaltObjectNode *head;

    u32     module_amount;
    struct SaltModule **modules;

    u32     instruction_amount;
    String *instructions;

    u32     label_amount;
    u32    *labels;

};

/**
 * Create a new SaltModule and put it on the global list.
 *
 * @param   name  name of the object
 * @return  heap-allocated SaltModule.
 */
struct SaltModule *module_create(SVMRuntime *_rt, char *name);

/**
 * Delete the module from the array.
 *
 * @param name
 */
void module_delete(SVMRuntime *_rt, char *name);

/**
 * Acquire a new object pointer. This allocates the needed memory.
 *
 * @param   module  the module to acquire the new object in
 * @return  brand new object
 */
SaltObject *module_object_acquire(SVMRuntime *_rt, struct SaltModule *module);

/**
 * Return the pointer to the given object which matches the ID.
 *
 * @param   module  the module to find the object in
 * @param   id      ID of the object
 * @return  pointer to object, may return NULL
 */
SaltObject *module_object_find(struct SaltModule *module, u32 id) Nullable;

/**
 * Delete the given object from the module (by ID)
 *
 * @param   module  the module to delete the object in
 * @param   id      ID of the object that will be unlinked.
 */
void module_object_delete(SVMRuntime *_rt, struct SaltModule *module, u32 id);

/**
 * Remove all modules from memory.
 */
void module_delete_all(SVMRuntime *_rt);

#endif // SVM_MODULE_H

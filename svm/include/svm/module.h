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
 * @author bellrise
 */
#ifndef SVM_MODULE_H
#define SVM_MODULE_H

/* The module library provides the SaltModule structure and functions for
   creation, modification & deletion of modules. */

#include <svm/svm.h>


/**
 * Objects in the SaltModule are stored in a linked list for greater efficiency
 * when removing objects, which do not require any data movement when stored in
 * a linked list.
 */
struct SaltObjectNode
{
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
 */
struct SaltModule
{
    /* Name of the module. Can only be 63 chars long: this should be controlled
       by the compiler, and should throw an error if the user tries to compile
       such a module. If the user renames the file to a name longer than 63
       chars, SVM will just not find it. */

    char    name[64];

    /* Pointer to the first object node in the dynamic linked object list. */

    struct  SaltObjectNode *objects_head;

    /* These are pointers to other modules, which are managed by the EXTLD
       instruction. There is only one global array of modules in the SVM
       runtime which are actually loaded, in order to minimize memory usage. */

    u32     module_amount;
    struct  SaltModule **modules;

    /* Module instructions. Each of them is a String with a specified length. */

    u32     instruction_amount;
    String *instructions;

    /* To achieve faster lookup speeds, the "pointers" to labels are stored in
       a different array. This allows the lookup function to visit only places
       which are actually labels, omiting executable instructions. */

    u32     label_amount;
    u32    *labels;
};

/**
 * Create a new SaltModule and put it on the global list.
 *
 * @param   name name of the object
 * @return  heap-allocated SaltModule.
 */
struct SaltModule *module_create(SVMRuntime *_rt, char *name);

/**
 * Delete the module from the array.
 */
void module_delete(SVMRuntime *_rt, char *name);

/**
 * Acquire a new object pointer. This allocates the needed memory.
 *
 * @param   module the module to acquire the new object in
 * @return  brand new object
 */
SaltObject *module_object_acquire(SVMRuntime *_rt, struct SaltModule *module);

/**
 * Return the pointer to the given object which matches the ID.
 *
 * @param   module the module to find the object in
 * @param   id ID of the object
 * @return  pointer to object, may return NULL
 */
SaltObject *module_object_find(struct SaltModule *module, u32 id) Nullable;

/**
 * Delete the given object from the module (by ID)
 *
 * @param   module the module to delete the object in
 * @param   id ID of the object that will be unlinked.
 */
void module_object_delete(SVMRuntime *_rt, struct SaltModule *module, u32 id);

/**
 * Remove all modules from memory.
 */
void module_delete_all(SVMRuntime *_rt);


#endif /* SVM_MODULE_H */

/**
 * This module provides the SaltModule structure and the related functions
 * for creating and modifying such salt structure.
 *
 * @author  bellrise
 */
#ifndef SVM_MODULE_H
#define SVM_MODULE_H

#include "core.h"
#include "object.h"

#define MODULE_OBJECT_SPACE 32

/* This container stores a single loaded salt module. One module translates
 one compiled .scc file. The big difference between SCC format 1 and format 3
 is the salt modules. Because there would be namespace clashes for externally
 loaded modules, every module loaded with EXTLD is put into its own SaltModule,
 and then linked live during execution. */
struct SaltModule {

    /* the maximum size of a module "namespace" is 63 chars (the compiler should
     not compile modules with names larger than 63 chars. ) */
    char name[64];

    /* local objects */
    uint objects_size;
    uint objects_locked;
    uint objects_space;
    SaltObject *objects;

    /* imports */
    uint import_amount;
    struct SaltModule *imports;

    /* bytecode */
    uint instruction_amount;
    struct SaltInstruction *instructions;

    /* functions */
    uint  label_amount;
    uint *labels;

};

/**
 * Create a new SaltModule and put it on the global list.
 *
 * @param name  name of the object
 * @return      heap-allocated SaltModule.
 */
struct SaltModule *module_create(char *name);

/**
 * Delete the module from the array.
 *
 * @param name
 */
void module_delete(char *name);

/**
 * Acquire a new object pointer. This allocates the needed memory.
 *
 * @return brand new object
 */
SaltObject *module_acquire_new_object(struct SaltModule *module);

/**
 * Delete the given object from the module (by ID)
 *
 * @param  id  ID of the object that will be unlinked.
 */
void module_delete_object(struct SaltModule *module, uint id);

/**
 * Remove all modules from memory.
 */
void module_delete_all();

#endif // SVM_MODULE_H

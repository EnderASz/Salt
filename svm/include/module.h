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

/* This container stores a single loaded salt module. One module translates
 one compiled .scc file. The big difference between SCC format 1 and format 3
 is the salt modules. Because there would be namespace clashes for externally
 loaded modules, every module loaded with EXTLD is put into its own SaltModule,
 and then linked live during execution. */
struct SaltModule {

    byte open_byte;

    /* the maximum size of a module "namespace" is 62 chars (the compiler should
     not compile modules with names larger than 62 chars. ) */
    char name[63];

    /* local objects */
    uint object_amount;
    SaltObject *objects;

    /* imports */
    uint import_amount;
    struct SaltModule *imports;

    /* bytecode */
    uint instruction_amount;
    struct SaltInstruction *instructions;

    /* functions */
    uint  function_ptr_amount;
    uint *function_ptr;

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
 * Remove all modules from memory.
 */
void module_delete_all();

#endif // SVM_MODULE_H

/**
 * The exec library is responsible for executing code.
 *
 * @author  bellrise
 */
#ifndef SVM_EXEC_H
#define SVM_EXEC_H

#include "core.h"

/* This structure is used for storing a single svm instruction. */
struct SVMCall {

    char instruction[6];
    int ( *f_exec ) (struct SaltModule *module, byte *payload, int pos);

};

/**
 * This is the code execution loop function, which executed the passed module
 * structure. Here is a short explanation on what happens:
 *
 * During: if the current instruction does not start with a @, indicating that
 * it is not a label, it finds the matching SVMCall and executes it.
 *
 * After: after exiting the main execution loop, it deallocates all used heap
 * memory and safely exits the program.
 *
 * @param   main  main module (named __main__)
 * @return  return status of exec_ instructions.
 */
int exec(struct SaltModule* main);

/**
 * Return the pointer to the svm call.
 */
struct SVMCall *exec_get(char *title);

/**
 * Clear all registers.
 */
void register_clear();

/**
 * Call a different function and jump to it.
 */
int exec_callf(struct SaltModule *module, byte *payload, int pos);

/**
 * Exit the current executed module. This is the safe version of KILLX, because
 * it jumps to the $__END__ label and leaves exec to pop everything from the
 * stack and finish execution. This is different to said kill instruction,
 * which immediately collapses the module tapes and exits the program.
 */
int exec_exite(struct SaltModule *module, byte *payload, int pos);

/**
 * Load an external SCC file and add it to the global module register.
 */
int exec_extld(struct SaltModule *module, byte *payload, int pos);

/**
 * Kill the whole program on-the-spot. This tried to free any memory it can as
 * fast as possible, and this kills the whole program. Note that this almost
 * certainly produce unwanted memory leaks, so it's recommended not to use it.
 */
int exec_killx(struct SaltModule *module, byte *payload, int pos);

/**
 * Create a new object in the current module. This should be handled by
 * an external function in the compiler, to always get it right because
 * it's a quite complex instruction.
 */
int exec_objmk(struct SaltModule *module, byte *payload, int pos);

/**
 * Delete the object in the current module. This doesn't actually remove
 * the object from memory, but signs it as "inactive". The virtual machine
 * decides when to actually free the memory and remove the object permanently.
 */
int exec_objdl(struct SaltModule *module, byte *payload, int pos);

/**
 * Print the value of the object (without a newline). Most prints require
 * a newline anyway, so it is a good idea for the compiler to create a readonly
 * string with only a newline character, and chain the PRINT object & PRINT
 * const_newline.
 */
int exec_print(struct SaltModule *module, byte *payload, int pos);

/**
 * Load an external SCC file and add it to the global module register.
 */
int exec_retrn(struct SaltModule *module, byte *payload, int pos);

/**
 * Print the value at the register.
 */
int exec_regdp(struct SaltModule *module, byte *payload, int pos);

/**
 * Move the value from the register onto the module object list, making it
 * a constant object with a set ID. Note that this does not remove any previous
 * objects with the same IDs from the list, but adds a brand new object at the
 * end.
 */
int exec_regmv(struct SaltModule *module, byte *payload, int pos);

/**
 * Set all the registers to NULL.
 */
int exec_regnl(struct SaltModule *module, byte *payload, int pos);

/**
 * Set the value of the given register to the selected object by ID. Important
 * note: this removes the original object from the module object list,
 * assigning it only to the register. There are only 8 registers to use, with
 * the 0 register used for return values from a couple of different
 * instructions.
 */
int exec_regst(struct SaltModule *module, byte *payload, int pos);

#endif // SVM_EXEC_H

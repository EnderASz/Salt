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
 * The exec library is responsible for executing code. All exec functions and
 * register operations can be found here.
 *
 * @author  bellrise, 2021
 */
#ifndef SVM_EXEC_H
#define SVM_EXEC_H

#include "core.h"
#include "module.h"

/**
 * This structure is used for storing a single SVM instruction. An array of 
 * these is created in the exec.c source file called g_execs. 
 *
 * @a instruction   the 5 char opcode to the instruction
 * @a f_exec        pointer to function
 * @a pad           instruction padding
 */
struct SVMCall {

    char instruction[6];
    
    /* the exec function */
    uint ( * f_exec )
    (SVMRuntime *_rt, struct SaltModule *module, byte *payload, uint pos);

    int pad;
};

/* Exec functions */
extern const struct SVMCall g_execs[];
extern const uint g_exec_amount;

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
 * @param   main  main module, the first loaded module should be named __main__
 * @return  status of exec_ instructions.
 */
int exec(SVMRuntime *_rt, struct SaltModule* main);

/**
 * Return the pointer to the svm call. This will return
 *
 * @param   title  string to the title
 * @return  pointer to the SVMCall struct 
 */
const struct SVMCall *lookup_exec(char *title);

/**
 * Control the amount of registers currently allocated. This will allocate the
 * largest given value and will reallocate the g_registers array to fit the max.
 * This is called on every module load so it prepares the needed amount of 
 * registers for all modules, because registers are global. 
 *
 * @param   size  amount of registers. max 255
 */
void register_control(SVMRuntime *_rt, uint8_t size);

/**
 * Clear all registers.
 */
void register_clear(SVMRuntime *_rt);

/**
 * This comment is for all function below.
 *
 * All these functions are exec functions which get executed per instruction. 
 * Each function is registered in the g_execs static variable.
 *
 * @param   module  pointer to the module. restricted
 * @param   payload the instruction payload
 * @param   pos     current instruction number the execution is at
 * @return  instruction number to jump to
 */

/**
 * Call a different function and jump to it.
 */
uint exec_callf(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Compare two objects for an equal value, can compare bools, ints, floats and
 * strings. If true, sets the jump flag.
 */
uint exec_cxxeq(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Compare two object for an equal value, an compare bools, ints, floats and 
 * strings. If false sets the jump flag.
 */
uint exec_cxxne(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Exit the current executed module. This is the safe version of KILLX, because
 * it jumps to the last instruction and leaves exec to pop everything from the
 * stack and finish execution. This is different to said kill instruction,
 * which immediately collapses the module tapes and exits the program.
 */
uint exec_exite(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Load an external SCC file and add it to the global module register.
 */
uint exec_extld(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Add the value to the object of type int.
 */
uint exec_ivadd(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Subtract a given value from the object of type int.
 */
uint exec_ivsub(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Jump to the given symbol only if the jump flag is set.
 */
uint exec_jmpfl(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Jump to the given symbol only if the jump flag is NOT set.
 */
uint exec_jmpnf(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Jump to the passed label without creating a new entry on the callstack. 
 * This may be used in loops, because calling a label and putting it on the
 * callstack costs a lot of memory in the long run.
 */
uint exec_jmpto(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Kill the whole program on-the-spot. This tried to free any memory it can as
 * fast as possible, and this kills the whole program. Note that this almost
 * certainly produce unwanted memory leaks, so it's recommended not to use it.
 */
uint exec_killx(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Map the whole module list.
 */
uint exec_mlmap(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Create a new object in the current module. This should be handled by
 * an external function in the compiler, to always get it right because
 * it's a quite complex instruction.
 */
uint exec_objmk(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Delete the object in the current module. This doesn't actually remove
 * the object from memory, but signs it as "inactive". The virtual machine
 * decides when to actually free the memory and remove the object permanently.
 */
uint exec_objdl(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Print the value of the object (without a newline). Most prints require
 * a newline anyway, so it is a good idea for the compiler to create a readonly
 * string with only a newline character, and chain the PRINT object & PRINT
 * const_newline.
 */
uint exec_print(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);
/**
 * Print the object at the given register. This can be done in 3 instructions,
 * RGPOP to move it into an object, PRINT to print it and then move it back to
 * the register using RPUSH.
 */
uint exec_rdump(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Load an external SCC file and add it to the global module register.
 */
uint exec_retrn(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);
/**
 * Move the value from the register onto the module object list, making it an
 * object with a set ID. Note that this does not remove any previous objects 
 * with the same IDs from the list, but adds a brand new object at the beginning.
 */
uint exec_rgpop(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Clear all registers.
 */
uint exec_rnull(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

/**
 * Set the value of the given register to the selected object by ID. 
 * Important note: this removes the original object from the module object
 * list, assigning it only to the register.
 */
uint exec_rpush(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);


/**
 * Sleep the given amount of miliseconds.
 */
uint exec_sleep(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos);

#endif // SVM_EXEC_H

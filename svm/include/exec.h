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

#include "svm.h"
#include "module.h"


/* Because svmcalls are very long, I use this macro to automatically generate
   a function declaration for me in the place of the call. I don't think this
   decreases readability, only increases it because it's a simple __SVMCALL. */

#define __SVMCALL(NAME) u32 exec_##NAME(SVMRuntime *_rt, \
        struct SaltModule *module, u8 *payload, u32 pos)

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
    u32 ( * f_exec ) (SVMRuntime *_r, struct SaltModule *_m, u8 *_p, u32 _s);

    i32 pad;
};

/* Exec functions */
extern const struct SVMCall g_execs[];
extern const u32 g_exec_amount;

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
 * @param   main    main module, the first loaded module should be named __main__
 * @param   start   name of the function it should start executing from 
 * @return  status of exec_ instructions.
 */
i32 exec(SVMRuntime *_rt, struct SaltModule* main, const char *start);

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
void register_control(SVMRuntime *_rt, u8 size);

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

__SVMCALL (callf);
__SVMCALL (callx);
__SVMCALL (cxxeq);
__SVMCALL (cxxlt);
__SVMCALL (exite);
__SVMCALL (extld);
__SVMCALL (ivadd);
__SVMCALL (ivsub);
__SVMCALL (jmpfl);
__SVMCALL (jmpnf);
__SVMCALL (jmpto);
__SVMCALL (killx);
__SVMCALL (mlmap);
__SVMCALL (objmk);
__SVMCALL (objdl);
__SVMCALL (passl);
__SVMCALL (print);
__SVMCALL (rdump);
__SVMCALL (retrn);
__SVMCALL (rgpop);
__SVMCALL (rnull);
__SVMCALL (rpush);
__SVMCALL (sleep);
__SVMCALL (trace);

#endif // SVM_EXEC_H

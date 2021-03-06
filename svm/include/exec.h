/**
 * The main execution loop is located in this code.
 * 
 * @author bellrise
 */
#ifndef EXEC_H_
#define EXEC_H_

#include "../include/core.h"

/* Main execution loop, this is called when the whole file is read. This
 * function is created with extreme detail to achieve a high execution speed
 * for the Virtual Machine.
 */
extern int exec(char* _code[]);

/* Before execution each piece of bytecode is preloaded to find all the 
 * important labels, like $__INIT__, $__END__ and all the ELSE. This is done
 * to achieve better lookup speeds for the perticular label types as it can
 * be moved to a binary search algorithm. Fast lookup speeds mean quick jumps
 * to different code parts, increasing the speed.
 */
extern void preload(char* _code[]);

// SALT INSTRUCTIONS

/* This instruction is located at the end of every function, even if the user
 * did not define so. This sends a CSPOP signal to the virtual machine, popping
 * one element off the callstack and returning to the previous scope.
 */
extern void si_retrn(byte* _bytes);

/* This prints the constant string to standard out. The usual CALLX io print
 * is replaced with this as oftenly as possible by the compiler to reach better 
 * speeds.
 */
extern void si_prntr(byte* _bytes);

#endif // EXEC_H_
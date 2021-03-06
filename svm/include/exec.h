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

#endif // EXEC_H_
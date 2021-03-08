/**
 * The exec module is responsible for executing the code in the main execution
 * loop inside the creatively named exec() function.
 *
 * @author bellrise
 */
#ifndef EXEC_H_
#define EXEC_H_

#include "core.h"
#include "object.h"

/* The SaltExec structure holds the 5 character instruction and the pointer to
 the function is should execute. */
struct SaltExec {

    char instruction[6];
    void (*f_exec) (byte *);

};

/* Constant list of function pointers */
extern const struct SaltExec salt_execs[];

/* Locations of the start and finish labels */
extern uint exec_label_init;
extern uint exec_label_end;

/* The main execution loop. Executes the instructions passed in the bytecode
 * by reading the 5 first characters as the instruction and finding the correct
 * function in salt_execs.
 *
 * @code: loaded bytecode
 *
 * returns: return code
 */
int exec(char **code);

/* Preload the bytecode meaning: find the $__INIT__ and $__END__ labels, as well
 * as all else labels for faster access.
 *
 * @code: loaded bytecode
 */
void preload(char **code);

// EXECS

/* Print the constant string. 
 *
 * @data: the first 4 bytes indicate the constant string ID
 */
void exec_prntr(byte *data);

/* Sleep the given amount of miliseconds. 
 *
 * @data: the first 4 bytes are the time you want to sleep
 */
void exec_sleep(byte *data);

#endif // EXEC_H_
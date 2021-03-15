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

/* Signals that get sent to exec */
#define EXEC_SIGPASS    0x00
#define EXEC_SIGERR     0x01
#define EXEC_SIGKILL    0xFF

/* The SaltExec structure holds the 5 character instruction and the pointer to
 the function is should execute. */
struct SaltExec {

    char instruction[6];
    byte (*f_exec) (byte *);

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
 * @code  loaded bytecode
 *
 * returns: return code
 */
int exec(char **code);

/* Preload the bytecode meaning: find the $__INIT__ and $__END__ labels, as well
 * as all else labels for faster access.
 * 
 * @code  loaded bytecode
 */
void preload(char **code);

// EXECS

/* Print the ID of the object (without a newline).
 *
 * @data id
 */
byte exec_dumpi(byte *data);

/* Print the value of the object to standard out (without a newline). Note that
 * this is based on a switch case statement reading the type of the object to 
 * call the correct function, so it's a lot slower than just calling PRINT.
 *
 * @data id
 */
byte exec_dumpv(byte *data);

/* Brutally kill the program and all its threads stopping the execution and
 * immediately calling core_clean. Note that this is not the preffered way of
 * stopping code execution.
 *
 * @data
 */
byte exec_killx(byte *data);

/* Print a constant string to standard out. This is the preferred way of
 * printing constant strings, because it is much faster compared to calling 
 * DUMPV. 
 *
 * @data id
 */
byte exec_print(byte *data);

/* Create a new object with the specified destination id and copy the object
 * from the source id to it.
 *
 * @data ms
 */
byte exec_sleep(byte *data);

/* Create a new object with the specified destination id and copy the object
 * from the source id to it.
 *
 * @data dest src
 */
byte exec_rxcpy(byte *data);

/* Delete the object by freeing the allocated memory for the value and removing
 * the ID from the register.
 *
 * @data id
 */
byte exec_rxdel(byte *data);

/* Create a new object in the register.
 *
 * @data id const type val
 */
byte exec_rxnew(byte *data);

/* Assign a new value to the object.
 *
 * @data id type [strl] ...
 */
byte exec_rxset(byte *data);

#endif // EXEC_H_
/**
 * Core header containg the core definitions and functions used
 * by the whole Salt Virtual Machine.
 *
 * @author bellrise
 */
#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>

/* This is the amount of bytes that will be allocated for one statement/
 * instruction in load_bytecode. The default is 256, although it may increase
 * in the future if something happens. Note, keep the size alignable to one
 * page, so 4096 bytes.
 */
#define SVM_INSTRUCTION_WIDTH 128

/* This is the amount of instructions are allocated at the beginning in
 * load_bytecode. If more than SVM_INITIAL_CODE_S instructions are found, 
 * another SVM_INITIAL_CODE_S instructions are allocated on the heap.
 */
#define SVM_INITIAL_CODE_S 256

/* This is the initial amount of uints in the svm_else array. 
 * 
 */
#define SVM_INITIAL_ELSE_S 16

// Global types
typedef unsigned int  uint;
typedef char* mptr_t;

// Virtual Machine global variables

/* This 16-byte header should be located on top of each .scc file. The 
 * SCC_HEADER contains the "magic bytes" indicating that this is indeed a scc
 * (salt compiled code) file. The second 8 bytes, SCC_VERSION indicate the 
 * format of the compiled code. If the format does not match the virtual 
 * machine version, it will not execute.
 */
extern const char SCC_HEADER  [];
extern const char SCC_VERSION [];

/* uint max code pointer: the maximum amount cp can have before exiting
 * execution. This is set by load_bytecode when reading bytecode from the 
 * input .scc file.
 */
extern uint svm_mcp;

/* uint code pointer: points to the current location on the code array.
 * This is chosen rather than a simple for loop in order to have more control
 * over the execution flow, like making jumps.
 */
extern uint svm_cp;

/* These are the locations of the $__INIT__ and $__END__ labels defined in the
 * preload function. These are used for as the entrypoint and exit of the 
 * program. If exec matches svm_cp to svm_end, it exits the program execution. 
 */
extern uint svm_init;
extern uint svm_end;

/* Both of those variables store information about the ELSE labels found in
 * the bytecode. This is done to quickly and efficiently jump to the next 
 * section when the `if` statement returns false.
 */
extern uint* svm_else;
extern uint  svm_else_s;

/* Shows the help page and exits the whole program.
 */
extern void core_help();

/* Parse the passed arguments and set special flags defined depending on the 
 * arguments used when executing the program. This also returns the filename
 * that the user chose to execute.
 */
extern char* parse_args(int argc, char** argv);

/* Returns every line of the bytecode in a string array. The length of the
 * array is automatically set to the global uint mbp. The first list is omited
 * from the final string array because it contains the bytecode header which
 * contains some crucial information about the bytecode itself.
 */
extern char** load_bytecode(const char* _filen);

/* Load a single line from the file until it finds a newline. This has been
 * moved here instead of using fgets, because it needs to load bytes after
 * a null terminator. Note, the max _alloc size is by default set to 
 * SVM_INSTRUCTION_WIDTH - 1.
 */
extern int load_line(FILE* _fp, char* _alloc);

#endif // CORE_H_

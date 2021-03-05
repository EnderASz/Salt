/**
 * Core header containg the core definitions and functions used
 * by the whole Salt Virtual Machine.
 *
 * @author bellrise
 */
#ifndef CORE_H_
#define CORE_H_

/* This is the amount of bytes that will be allocated for one statement/
 * instruction in load_bytecode. The default is 256, although it may increase
 * in the future if something happens. Note, keep the size alignable to one
 * page, so 4096 bytes.
 */
#define SVM_INSTRUCTION_WIDTH 256

/* This is the amount of instructions are allocated at the beginning in
 * load_bytecode. If more than SVM_INITIAL_CODE_S instructions are found, 
 * another SVM_INITIAL_CODE_S instructions are allocated on the heap.
 */
#define SVM_INITIAL_CODE_S 256

// Global types
typedef unsigned int  uint;

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

/* Shows the help page and exits the whole program.
 */
void core_help();

/* Parse the passed arguments and set special flags defined depending on the 
 * arguments used when executing the program. This also returns the filename
 * that the user chose to execute.
 */
char* parse_args(int argc, char** argv);

/* Returns every line of the bytecode in a string array. The length of the
 * array is automatically set to the global uint mbp. The first list is omited
 * from the final string array because it contains the bytecode header which
 * contains some crucial information about the bytecode itself.
 */
char** load_bytecode(const char* _filen);

#endif // CORE_H_
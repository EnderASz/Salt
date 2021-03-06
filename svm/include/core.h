/**
 * Core header containg the core definitions and functions used
 * by the whole Salt Virtual Machine.
 *
 * @author bellrise
 */
#ifndef CORE_H_
#define CORE_H_

#include <stdio.h>

#ifdef _WIN32
 #define MPTR_SIZE 4
#elif defined(_WIN64) || defined(__linux__)
 #define MPTR_SIZE 8
#endif

/* This is the amount of bytes that will be allocated for one statement/
 * instruction in load_bytecode. The default is 128, although it may increase
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
 */
#define SVM_INITIAL_ELSE_S 16

// Global types
typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned char byte;
typedef byte* mptr_t;

/* This structure represents a single Salt instruction to execute. In the exec
 * function, the correct instruction with the maching id is found and then 
 * executed, passing the remaining bytes as the argument.
 */
typedef struct _si_st {

    char id[5];
    void (*f_exec)(byte* _bytes);

} SaltExec;

/* This unit structure is used for storing small amounts of data in an easy
 * and accessible way. The first byte of the structure is the type, and the 
 * next 15 bytes can store the actual value. This may be a wierd number, but
 * it's there so the structures can be perfectly aligned in memory (16-byte).
 */
typedef struct _su_st {

    byte type;
    byte value[15];

} SaltUnit;

/* This is the long salt unit type, which can store more data than you could
 * ever use in this language. This is used in strings for example, which are
 * usually longer than 15 bytes, the basic SaltUnit could store. The crazy C
 * preprocessor magic is there to _guarantee_ that this structure will always
 * be 16 bytes wide. This also happens to be exactly the same as SaltUnit, so
 * a single array of simple units can be created, casting the pointer to salt
 * long when needed.
 *
 * Exaplanation of the fields:
 * type - 1B stores the type
 * _pad - 3B padding
 * size - 4B (uint) number of additional elements
 * ptr  - 4/8B pointer to further memory
 * _pad4 - 4 bytes of padding, if svm is compiled for 32bits.
 */
typedef struct _sul_st {

    byte type [1];
    byte _pad [3];
    uint size [1];
    mptr_t ptr;
#if MPTR_SIZE == 4
    byte _pad4[4];
#endif

} SaltLong;

/* Stores the id, length and the pointer to the constant string. Same as the
 * the other Salt units, this is exactly 16 bytes long for easy casting.
 */
typedef struct _scs_st {

    uint id;
    uint len;
    mptr_t ptr;
#if MPTR_SIZE == 4
    byte _pad4[4];
#endif

} SaltCString;

// Virtual Machine global variables
 
/* The main array of Salt executable units. Each element represents a single
 * callable f_exec function. 
 */
extern const SaltExec execs[];
extern const uint execs_s;

/* This 16-byte header should be located on top of each .scc file. The 
 * SCC_HEADER contains the "magic bytes" indicating that this is indeed a scc
 * (salt compiled code) file. 
 */
extern const char SCC_HEADER[];
extern const ushort SCC_VERSION;

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

/* This is the array for all constant strings loaded before executing the code.
 * Each constant string has 3 pieces of information. The string id, located on
 * the first 4 bytes; the string length, located on the next 4 bytes and the 
 * other 8 bytes are the pointer to the location in bytecode where the string
 * starts. The amount of const strings is set in load_bytecode.
 */
extern SaltCString* svm_cstr;
extern uint svm_cstr_s;

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

/* This goes through each line in the bytecode (until it hits @$__INIT__) and
 * saves every constant string that can be loaded. The pointers to the memory 
 * locations are then saved in a salt unit in the svm_cstr array.  
 */
extern void load_strings(char* _code[]);

/* Because newlines are a part of the bytecode structure, they need to be 
 * replaced with a different character in the constant strings. The chosen
 * character is 0x11, which is a device control ASCII char.
 */
extern void replace_newlines(char* _str);

/* Load a single line from the file until it finds a newline. This has been
 * moved here instead of using fgets, because it needs to load bytes after
 * a null terminator. Note, the max _alloc size is by default set to 
 * SVM_INSTRUCTION_WIDTH - 1.
 */
extern int load_line(FILE* _fp, char* _alloc);

/* This is called on program exit to free all the allocated memory and close
 * all file handles. Basically, it renders the virtual machine unusable and
 * barely alive.
 */
extern void clean();

#endif // CORE_H_

/**
 * The core is responsible for the main user interaction, so parsing
 * command line options and returning errors. All the main preprocessor
 * directives are found here because this header is included in every other
 * header & source file in here, which also means all the global variables
 * are found here.
 *
 * @author bellrise
 */
#ifndef CORE_H_
#define CORE_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "object.h"

/* Architecture the program is compiled on. On Windows this is usually set to
 32-bit by default, so to properly align structures you need to add padding
 depending if the pointer size is 8 bytes or 4 bytes. Also, sucks to be a MacOs
 user because SVM doesn't support it. too bad! */
#ifdef _WIN32
  #define ARCH 32
#elif defined(_WIN64) || defined(__linux__)
  #define ARCH 64
#endif

/* These header defines are used for validating the SCC header. These contain
 important information such as the magic number & the SCC format version. */
#define SCC_HEADER_MAGIC   "\x7fSCC\xff\xee\0\0"
#define SCC_HEADER_VERSION "\x01\x00\x00\x00"

/* Core error macro, print the error message and exit. */
#define CORE_ERROR(...)         \
    {                           \
        printf("svm: ");        \
        printf(__VA_ARGS__);    \
        exit(1);                \
    }

// TYPES

typedef unsigned char byte;
typedef unsigned int  uint;

// FLAGS

/* help page flag */
extern char FLAG_HELP;

/* max memory flag */
extern uint svm_max_mem;

/* initial xregister size */
extern uint svm_xregister_size;

// GLOBALS

/* Amount of bytes the SVM has allocated (outside of its own code). This is 
 tracked by vmalloc. */
extern unsigned long long svm_allocated;

/* The amount of instructions core_load_bytecode needs to allocate space for.
 This helps to speed up the compiler to not overallocate nor underallocate,
 so it's generally faster and more memory efficient. */
extern uint svm_instructions;

/* The amount of constant strings the vm needs to allocate space for. Again, 
 this is for optimazation. */
extern uint svm_const_strings;

/* Maximum width of a single instruction provided by the compiler. */
extern uint svm_max_width;

/* Array of constant strings */
extern SaltObject *salt_const_strings;

/* Global register */
extern struct SaltArray xregister;
extern SaltObject       xnullptr;

/* Parse the command line arguments and set special flags defined here so they
 * can be accessed anywhere.
 * 
 * @argc amount of arguments
 * @argv array of (strings) arguments 
 *
 * returns: filename of the script
 */
char *core_parse_args(int argc, char **argv);

/* Show the help page and exit the program. */
void core_show_help();

/* Initialize some global variables and registers. Be sure to call this before
 * calling exec or preload. 
 */
void core_init();

/* Read & load the header file contents to the global variables. While reading,
 * validate information in the header and optionally exit the program with a
 * fatal error if something is incorrect.
 *
 * @_fp: file pointer to the scc file 
 */
void core_load_header(FILE *_fp);

/* Read & load all constant strings from the top of the file and puts the into 
 * the SaltObject constant string array (salt_cstrings).
 *
 * @_fp: file pointer to the scc file
 */
void core_load_strings(FILE *_fp);

/* Read n amount of bytes from the file and place them in the string array.
 * This takes the string length for granted.
 *
 * @_fp:  file pointer to read from
 * @_str: memory allocated string
 * @_n:   amount of bytes
 */
void core_read_bytes(FILE *_fp, char *_str, uint _n);

/* Similar to core_read_bytes, it reads the file pushing the characters into 
 * the C string until it finds the _c char. 
 * 
 * @_fp:  file pointer to read from
 * @_str: memory allocated string
 * @_c:   the char to stop at
 */
void core_read_until(FILE *_fp, char *_str, char _c);

/* Read & load the bytecode from the scc file. This must be executed after
 * core_load_header, because of the global variables it sets and also moves the
 * file cursor 64 bytes forward.
 *
 * @_fp: file pointer to the scc file
 *
 * returns: pointer to allocated area for the bytecode to sit in
 */
char **core_load_bytecode(FILE *_fp);

/* Deallocate all memory and close any open file pointers. It is very important
 * to call this before exiting the program.
 *
 * @bytecode  bytecode to deallocate
 */
void core_clean(char **bytecode);

/* Add a object to the register. The blacklisted IDs range from 0000 0000 (0)
 * to 0080 0000 (128).
 *
 * @_obj  object to add
 */
void xregister_add(SaltObject _obj);

/* Remove object from register by ID.
 *
 * @_id   id of the object
 */
void xregister_remove(uint _id);

/* Returns the SaltObject with the given register.
 *
 * @_id  id of the object to look up
 *
 * returns: pointer to object in register, NULL if not found
 */
SaltObject *xregister_find(uint _id);

#endif // CORE_H_

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
#define CORE_ERROR(...)             \
    {                               \
        printf("[svm] Error: ");    \
        printf(__VA_ARGS__);        \
        exit(1);                    \
    }

// TYPES

typedef unsigned char byte;

// FLAGS

extern char FLAG_HELP;

// GLOBALS

/* Global ID of salt objects. The initial value of this is 100, leaving space
 for system variables ranging from 0 to 100. */
extern uint salt_id_counter;

/* The amount of instructions core_load_bytecode needs to allocate space for.
 This helps to speed up the compiler to not overallocate nor underallocate,
 so it's generally faster and more memory efficient. */
extern uint svm_instructions;

/* The amount of constant strings the vm needs to allocate space for. Again, 
 this is for optimazation. */
extern uint svm_const_strings;

/* Maximum width of a single instruction provided by the compiler. */
extern uint svm_max_width;

/* Get a unique ID for every newly created object.
 *
 * returns: unique ID
 */
uint salt_id();

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

/* Read & load the header file contents to the global variables. While reading,
 * validate information in the header and optionally exit the program with a
 * fatal error if something is incorrect.
 *
 * @_fp: file pointer to the scc file 
 */
void core_load_header(FILE *_fp);

/* Read & load the bytecode from the scc file. This must be executed after
 * core_load_header, because of the global variables it sets and also moves the
 * file cursor 64 bytes forward.
 *
 * @_fp: file pointer to the scc file
 *
 * returns: pointer to allocated area for the bytecode to sit in
 */
char **core_load_bytecode(FILE *_fp);

#endif // CORE_H_
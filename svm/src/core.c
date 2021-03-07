/**
 * core.h implementation.
 *
 * @author bellrise
 */
#include "../include/core.h"
#include <stdio.h>

/* This is a useful macro for comparing the argument with the passed string. */
#define CORE_ARGS_CMP(STR, LONG, SHORT)  \
       (strcmp(STR, LONG) == 0)          \
    || (strcmp(STR, SHORT) == 0)


// FLAGS

char FLAG_HELP = 0;

// GLOBALS

/* The amount of instructions core_load_bytecode needs to allocate space for.
 This helps to speed up the compiler to not overallocate nor underallocate,
 so it's generally faster and more memory efficient. */
uint svm_instructions = 0;

/* The amount of constant strings the vm needs to allocate space for. Again, 
 this is for optimazation. */
uint svm_const_strings = 0;

/* Maximum width of a single instruction provided by the compiler. */
uint svm_max_width = 0;

/* Array of constant strings */
SaltObject *salt_const_strings;

/* Parse the command line arguments and set special flags defined here so they
 * can be accessed anywhere.
 * 
 * @argc amount of arguments
 * @argv array of (strings) arguments 
 *
 * returns: filename of the script
 */
char *core_parse_args(int argc, char **argv)
{
    if (argc < 2)
        CORE_ERROR("not enough arguments, try --help\n");

    for (short i = 0; i < argc; i++) {

        if (CORE_ARGS_CMP(argv[i], "--help", "-h"))
            FLAG_HELP = 1;
    
    }

    // TOOD: This should not be position locked.
    return argv[1];
}

/* Show the help page and exit the program. */
void core_show_help()
{
    printf("%s\n%s\n\n%s\n%s\n", "Usage: svm FILE [OPTION]...",
    "Execute compiled Salt code.",
    "  FILE        compiled Salt code (scc) file",
    "  -h, --help  show this page and exit");
    exit(1);
}

/* Read & load the header file contents to the global variables. While reading,
 * validate information in the header and optionally exit the program with a
 * fatal error if something is incorrect.
 *
 * @_fp: file pointer to the scc file 
 */
void core_load_header(FILE *_fp)
{
    char header[64];

    fseek(_fp, 0, SEEK_SET);
    for (short i = 0; i < 64; i++) {
        header[i] = fgetc(_fp);
    }

    if (strncmp(header, SCC_HEADER_MAGIC, 8) != 0)
        CORE_ERROR("This is not an SCC executable\n");

    if (strncmp(header + 8, SCC_HEADER_VERSION, 8) != 0)
        CORE_ERROR("Invalid SCC version\n");

    svm_instructions      = * (uint *) (header + 16);
    svm_const_strings     = * (uint *) (header + 24);
    svm_max_width         = * (uint *) (header + 32);
}

/* Read & load all constant strings from the top of the file and puts the into 
 * the SaltObject constant string array (salt_cstrings).
 *
 * @_fp: file pointer to the scc file
 */
void core_load_strings(FILE *_fp)
{
    salt_const_strings = calloc(sizeof(SaltObject), svm_const_strings);
    
    uint strl;
    for (uint i = 0; i < svm_const_strings; i++) {

        core_read_bytes(_fp, (char *) &strl, 4);

        // String length 
        memcpy(salt_const_strings[i].typeinfo, (char *) &strl, 4);
        salt_const_strings[i].constant = 1;
        
        salt_const_strings[i].data = calloc(sizeof(char), strl);
        core_read_bytes(_fp, salt_const_strings[i].data, strl);
    
        // Replace 0x11 with newlines
        for (uint j = 0; j < strl; j++) {
            if (((char *) salt_const_strings[i].data)[j] == 0x11) {
                ((char *) salt_const_strings[i].data)[j] = 0x0a;
            }
        }

        // Move one byte forward over the 0x0a
        fgetc(_fp);
    }
}

/* Read n amount of bytes from the file and place them in the string array.
 * This takes the string length for granted.
 *
 * @_fp:  file pointer to read from
 * @_str: memory allocated string
 * @_n:   amount of bytes
 */
void core_read_bytes(FILE *_fp, char *_str, uint _n)
{
    for (uint i = 0; i < _n; i++)
        _str[i] = fgetc(_fp);
}

/* Similar to core_read_bytes, it reads the file pushing the characters into 
 * the C string until it finds the _c char. 
 * 
 * @_fp:  file pointer to read from
 * @_str: memory allocated string
 * @_c:   the char to stop at
 */
void core_read_until(FILE *_fp, char *_str, char _c)
{
    for (uint i = 0 ;; i++) {
        _str[i] = fgetc(_fp);
        if (_str[i] == _c) {
            _str[i] = 0;
            break;
        }
    }
}

/* Read & load the bytecode from the scc file. This must be executed after
 * core_load_header, because of the global variables it sets and also moves the
 * file cursor 64 bytes forward.
 *
 * @_fp: file pointer to the scc file
 *
 * returns: pointer to allocated area for the bytecode to sit in
 */
char **core_load_bytecode(FILE *_fp)
{
    char **code = calloc(sizeof(char *), svm_instructions);
    for (uint i = 0; i < svm_instructions; i++) {
        code[i] = calloc(sizeof(char), svm_max_width);

        core_read_until(_fp, code[i], '\n');
    }

    return code;
}

/**
 * core.h implementation.
 *
 * @author bellrise
 */
#include "../include/core.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

/* This is a useful macro for comparing the argument with the passed string. */
#define CORE_ARGS_CMP(STR, LONG, SHORT)           \
       (strncmp(STR, LONG, strlen(LONG)) == 0)    \
    || (strncmp(STR, SHORT, strlen(SHORT)) == 0)


// FLAGS

char FLAG_HELP = 0;

uint svm_xregister_size = 16;

// GLOBALS

uint svm_instructions = 0;
uint svm_const_strings = 0;
uint svm_max_width = 0;

SaltObject *salt_const_strings;

struct SaltArray xregister;

/* Parse the command line arguments and set special flags defined here so they
 * can be accessed anywhere. */
char *core_parse_args(int argc, char **argv)
{
    if (argc < 2)
        CORE_ERROR("not enough arguments, try --help\n");

    char *filename;

    for (short i = 0; i < argc; i++) {

        if (CORE_ARGS_CMP(argv[i], "--help", "-h"))
            FLAG_HELP = 1;

        else if (CORE_ARGS_CMP(argv[i], "--xregsize", "-x")) {
            i++;
            if (i >= argc)
                CORE_ERROR("xregsize requires an argument\n");

            uint size = str_to_uint(argv[i]);
            if (!size) {
                CORE_ERROR("xregsize requires an uint\n");
            }
            svm_xregister_size = size;
        }

        else 
            filename = argv[i];

    }

    // TOOD: This should not be position locked.
    return filename;
}

/* Show the help page and exit the program. */
void core_show_help()
{
    printf("Usage: svm FILE [OPTION]...\n"
    "Execute compiled Salt code.\n\n"
    "  FILE           compiled Salt code (scc) file\n"
    "  -h, --help     show this page and exit\n"
    "  -x, --xregsize initial xregister size. default 16\n");
    exit(1);
}

/* Initialize some global variables and registers. Be sure to call this before
 * calling exec or preload. */
void core_init()
{
    xregister = salt_array_create(svm_xregister_size + 128, 0);
}

/* Read & load the header file contents to the global variables. While reading,
 * validate information in the header and optionally exit the program with a
 * fatal error if something is incorrect. */
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
 * the SaltObject constant string array (salt_cstrings). */
void core_load_strings(FILE *_fp)
{
    salt_const_strings = alloc(sizeof(SaltObject), svm_const_strings);
    
    uint strl;
    for (uint i = 0; i < svm_const_strings; i++) {

        core_read_bytes(_fp, (char *) &strl, 4);

        // String length 
        memcpy(salt_const_strings[i].typeinfo, (char *) &strl, 4);
        salt_const_strings[i].constant = 1;
        
        salt_const_strings[i].data = alloc(sizeof(char), strl);
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
 * This takes the string length for granted. */
void core_read_bytes(FILE *_fp, char *_str, uint _n)
{
    for (uint i = 0; i < _n; i++)
        _str[i] = fgetc(_fp);
}

/* Similar to core_read_bytes, it reads the file pushing the characters into 
 * the C string until it finds the _c char. */
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
 * file cursor 64 bytes forward. */
char **core_load_bytecode(FILE *_fp)
{
    char **code = alloc(sizeof(char *), svm_instructions);
    for (uint i = 0; i < svm_instructions; i++) {
        code[i] = alloc(sizeof(char), svm_max_width);

        core_read_until(_fp, code[i], '\n');
    }

    return code;
}

/* Initialize some global variables and registers. Be sure to call this before
 * calling exec or preload. */
void core_clean(char **bytecode)
{
    for (uint i = 0; i < xregister.space; i++) {
        free(xregister.array[i].data);
    }
    free(xregister.array);


    for (uint i = 0; i < svm_instructions; i++) {
        free(bytecode[i]);
    }
    free(bytecode);

    free(salt_const_strings);

}

/* Returns the SaltObject with the given register. */
SaltObject *xregister_find(uint id)
{
    for (uint i = 0; i < xregister.size; i++) {
        if (id == xregister.array[i].id)
            return &xregister.array[i];
    }
    return NULL;
}

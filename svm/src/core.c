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

uint svm_max_mem = 0;

uint svm_xregister_size = 16;

// GLOBALS

unsigned long long svm_allocated = 0;

uint svm_instructions = 0;
uint svm_const_strings = 0;
uint svm_max_width = 0;

SaltObject *salt_const_strings;

struct SaltArray xregister;
SaltObject       xnullptr;

/* Parse the command line arguments and set special flags defined here so they
 can be accessed anywhere. */
char *core_parse_args(int argc, char **argv)
{
    if (argc < 2)
        CORE_ERROR("not enough arguments, try --help\n");

    char *filename;

    for (int i = 0; i < argc; i++) {

        if (CORE_ARGS_CMP(argv[i], "--help", "-h"))
            FLAG_HELP = 1;

        else if (CORE_ARGS_CMP(argv[i], "--xregsize", "-x"))
            svm_xregister_size = util_arg_uint(argc, argv, &i, "xregsize");

        else if (CORE_ARGS_CMP(argv[i], "--max-mem", "-m"))
            svm_max_mem = util_arg_uint(argc, argv, &i, "max-mem");

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
    "  -x, --xregsize initial xregister size. default 16\n"
    "  -m, --max-mem  maximum bytes the SVM can use\n");
    exit(1);
}

/* Initialize some global variables and registers. Be sure to call this before
 calling exec or preload. */
void core_init()
{
    xregister = salt_array_create(svm_xregister_size + 128, 0);
    xnullptr = salt_object_create(0, SALT_NULL, 0, 1, NULL, NULL, 0, 0);

    dprintf("Loaded xregister with size %d\n", svm_xregister_size);
    dprintf("Defined max memory: %d\n", svm_max_mem);
}

/* Read & load the header file contents to the global variables. While reading,
 validate information in the header and optionally exit the program with a
 fatal error if something is incorrect. */
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
 the SaltObject constant string array (salt_cstrings). */
void core_load_strings(FILE *_fp)
{
    salt_const_strings = vmalloc(sizeof(SaltObject), svm_const_strings);
    
    uint strl;
    for (uint i = 0; i < svm_const_strings; i++) {

        core_read_bytes(_fp, (char *) &strl, 4);

        // String length 
        memcpy(salt_const_strings[i].typeinfo, (char *) &strl, 4);
        salt_const_strings[i].constant = 1;
        
        salt_const_strings[i].data = vmalloc(sizeof(char), strl);
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
 This takes the string length for granted. */
void core_read_bytes(FILE *_fp, char *_str, uint _n)
{
    for (uint i = 0; i < _n; i++)
        _str[i] = fgetc(_fp);
}

/* Similar to core_read_bytes, it reads the file pushing the characters into 
 the C string until it finds the _c char. */
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
 core_load_header, because of the global variables it sets and also moves the
 file cursor 64 bytes forward. */
char **core_load_bytecode(FILE *_fp)
{
    char **code = vmalloc(sizeof(char *), svm_instructions);
    for (uint i = 0; i < svm_instructions; i++) {
        code[i] = vmalloc(sizeof(char), svm_max_width);

        core_read_until(_fp, code[i], '\n');
    }

    return code;
}

/* Initialize some global variables and registers. Be sure to call this before
 calling exec or preload. */
void core_clean(char **bytecode)
{
    dprintf("Cleaning core\n");
    for (uint i = 0; i < xregister.space; i++) {
        vmfree(xregister.array[i].data, util_get_size(&xregister.array[i]));
    }
    vmfree(xregister.array, xregister.space * sizeof(SaltObject));

    for (uint i = 0; i < svm_instructions; i++) {
        vmfree(bytecode[i], svm_max_width);
    }
    vmfree(bytecode, svm_instructions * sizeof(char *));

    unsigned long cstrings = 0;
    for (uint i = 0; i < svm_const_strings; i++) {
        vmfree(salt_const_strings[i].data, strlen(salt_const_strings[i].data));
    }
    vmfree(salt_const_strings, svm_const_strings * sizeof(SaltObject));

    if (svm_allocated > 0)
        dprintf("Memory leak of %lld bytes\n", svm_allocated);

}

/* Add a object to the register. The blacklisted IDs range from 0000 0000 (0)
 to 0080 0000 (128). */
void xregister_add(SaltObject _obj)
{
    if (_obj.id <= 128)
        CORE_ERROR("failed to add object to xreg\n");

    if (xregister.space <= xregister.size) {
        xregister.array = realloc(&xregister, sizeof(SaltObject) 
                       * (xregister.space + svm_xregister_size));

        xregister.space += xregister.size + svm_xregister_size;
    }   

    xregister.array[xregister.size] = _obj;
    xregister.size++;
}

/* Remove object from register by ID. */
void xregister_remove(uint _id)
{
    SaltObject *obj = xregister_find(_id);
    vmfree(obj->data, util_get_size(obj));
    *obj = xnullptr;
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

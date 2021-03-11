/**
 * core.h implementation.
 *
 * @author bellrise
 */
#include "../include/core.h"
#include "../include/utils.h"
#include "../include/except.h"
#include "../include/os.h"

#include <stdio.h>
#include <string.h>

/* This is a useful macro for comparing the argument with the passed string. */
#define CORE_ARGS_CMP(STR, LONG, SHORT)           \
       (strncmp(STR, LONG, strlen(LONG)) == 0)    \
    || (strncmp(STR, SHORT, strlen(SHORT)) == 0)


// FLAGS

char FLAG_HELP    = 0;
char FLAG_UNSAFE  = 0;

uint svm_max_mem = 0;

uint svm_xregister_size = 16;

// GLOBALS

unsigned long long svm_allocated = 0;

uint svm_instructions = 0;
uint svm_const_strings = 0;
uint svm_max_width = 0;

SaltObject *salt_const_strings;

struct SaltArray xregister;
SaltObject xnullptr;

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

        else if (CORE_ARGS_CMP(argv[i], "--unsafe", "-u"))
            FLAG_UNSAFE = 1;

        else if (CORE_ARGS_CMP(argv[i], "--version", "-v")) {
            printf("%s\n", SVM_VERSION);
            exit(1);
        }

        else 
            filename = argv[i];

    }

    return filename;
}

void core_show_help()
{
    printf("Usage: svm FILE [OPTION]...\n"
    "Execute compiled Salt code.\n\n"
    "  FILE           compiled Salt code (scc) file\n"
    "  -v, --version  show the version\n"
    "  -h, --help     show this page and exit\n"
    "  -x, --xregsize initial xregister size. default 16\n"
    "  -m, --max-mem  maximum bytes the SVM can use\n"
    "  -u, --unsafe   activate unsafe mode; this allows the compiler to "
    "modify the\n"
    "                 internal SVM registers; it is not recommended\n");
    exit(1);
}

void core_init()
{
    xregister = salt_array_create(svm_xregister_size + 128, 0);
    xnullptr  = salt_object_create(0, SALT_NULL, 0, 1, NULL, NULL, 0, 0);

    // SVM registers
    SaltObject err_msg  = salt_string_create(9, PERM_USER, 17, 
                          "RuntimeException");
    SaltObject err_text = salt_string_create(10, PERM_USER, 26,
                          "Program failed to execute");

    xregister_add(err_msg,  PERM_SVM);
    xregister_add(err_text, PERM_SVM);

    dprintf("Loaded xregister with size %d\n", svm_xregister_size);
    dprintf("Defined max memory: %d\n", svm_max_mem);
}

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

void core_read_bytes(FILE *_fp, char *_str, uint _n)
{
    for (uint i = 0; i < _n; i++)
        _str[i] = fgetc(_fp);
}

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

char **core_load_bytecode(FILE *_fp)
{
    char **code = vmalloc(sizeof(char *), svm_instructions);
    for (uint i = 0; i < svm_instructions; i++) {
        code[i] = vmalloc(sizeof(char), svm_max_width);

        core_read_until(_fp, code[i], '\n');
    }

    return code;
}

void core_clean(char **bytecode)
{
    dprintf("Cleaning core\n");
    
    // xregister
    for (uint i = 0; i < xregister.space; i++) {
        vmfree(xregister.array[i].data, util_get_size(&xregister.array[i]));
    }
    vmfree(xregister.array, xregister.space * sizeof(SaltObject));

    // Bytecode
    for (uint i = 0; i < svm_instructions; i++) {
        vmfree(bytecode[i], svm_max_width);
    }
    vmfree(bytecode, svm_instructions * sizeof(char *));

    for (uint i = 0; i < svm_const_strings; i++) {
        vmfree(salt_const_strings[i].data, 
               salt_object_strlen(&salt_const_strings[i]));
    }
    vmfree(salt_const_strings, svm_const_strings * sizeof(SaltObject));


    if (svm_allocated > 0)
        dprintf("Memory leak of %lld bytes\n", svm_allocated);

}

void xregister_add(SaltObject _obj, int _perm)
{
    dprintf("Adding Object %d\n", _obj.id);
    if ((_obj.id <= 128) && (!FLAG_UNSAFE) && (_perm == PERM_USER)) {
        except_set("PermissionException", 
                   "cannot add object to xregister: protected ID");
        except_throw();
    }
    if (xregister.space <= xregister.size) {
        xregister.array = vmrealloc(&xregister, sizeof(SaltObject) 
                          * xregister.space, sizeof(SaltObject) 
                          * (xregister.space + svm_xregister_size));

        xregister.space += xregister.size + svm_xregister_size;
    }   

    xregister.array[xregister.size] = _obj;
    xregister.size++;
}

void xregister_remove(uint _id)
{
    SaltObject *obj = xregister_find(_id);
    vmfree(obj->data, util_get_size(obj));
    *obj = xnullptr;
}

SaltObject *xregister_find(uint id)
{
    for (uint i = 0; i < xregister.size; i++) {
        if (id == xregister.array[i].id)
            return &xregister.array[i];
    }
    return NULL;
}

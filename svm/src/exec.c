/**
 * exec.h implementation
 *
 * @author bellrise
 */
#include "../include/exec.h"
#include "../include/os.h"
#include "../include/utils.h"

#include <time.h>

/* Constant list of function pointers. */
const struct SaltExec salt_execs[] = {
    {"RXNEW", exec_rxnew},
    {"RXSET", exec_rxset},
    {"RXDEL", exec_rxdel},
    {"DUMPV", exec_dumpv},
    {"PRINT", exec_print},
    {"SLEEP", exec_sleep},
    {"DUMPI", exec_dumpi},
    {"KILLX", exec_killx}
};

static short salt_execs_l = 8;

/* Locations of the start and finish labels */
uint exec_label_init = 0;
uint exec_label_end  = 0;

/* The main execution loop. Executes the instructions passed in the bytecode
 by reading the 5 first characters as the instruction and finding the correct
 function in salt_execs. */
int exec(char **code)
{
    for (uint i = exec_label_init + 1; i != exec_label_end; i++) {

        /* the signal exec will recieve from any SaltExec function */
        byte signal;

        for (short j = 0; j < salt_execs_l; j++) {
            
            if (strncmp(salt_execs[j].instruction, code[i], 5) == 0) {
                signal = salt_execs[j].f_exec((byte *) code[i] + 5);
                break;
            }

        }

        if (signal == EXEC_SIGPASS)
            printf("svm: exec_ failed\n");

        if (signal == EXEC_SIGKILL)
            return 1;

    }

    return 0;
}

/* Preload the bytecode meaning: find the $__INIT__ and $__END__ labels, as well
 as all else labels for faster access. */
void preload(char **code)
{
    for (uint i = 0; i < svm_instructions; i++) {

        if (strncmp(code[i], "@$__INIT__", 11) == 0) {
            exec_label_init = i;
        }

        if (strncmp(code[i], "@$__END__", 10) == 0) {
            exec_label_end = i;
        }

    }
}

/* Print the ID of the object (without a newline). */
byte exec_dumpi(byte *data)
{
    uint id = * (uint *) data;
    uint oid = xregister.array[id].id;

    printf("%d", oid);

    return EXEC_SIGPASS;
}

/* Print the value of the object to standard out (without a newline). Note that
 this is based on a switch case statement reading the type of the object to 
 call the correct function, so it's a lot slower than just calling PRINT. */
byte exec_dumpv(byte *data)
{
    uint id = * (uint *) data;

    util_print_object(&xregister.array[id]);

    return EXEC_SIGPASS;
}

/* Kill all threads and immediately exit the program calling core_clean(). */
byte exec_killx(byte *data)
{
    return EXEC_SIGKILL;   
}

/* Print the constant string. */
byte exec_print(byte *data)
{
    uint pos = * (uint *) data - 1;
    fputs((char *) salt_const_strings[pos].data, stdout);

    return EXEC_SIGPASS;
}

/* Sleep the given amount of miliseconds. */
byte exec_sleep(byte *data)
{
    os_sleep(* (uint *) data); 

    return EXEC_SIGPASS;
}

/* Create a new object with the specified destination id and copy the object
 from the source id to it. */
byte exec_rxcpy(byte *data)
{
    SaltObject new_ = *xregister_find(* (uint *) data);
    new_.id = * (uint *) data;

    xregister_add(new_);

    return EXEC_SIGPASS;
}

/* Delete the object by freeing the allocated memory for the value and removing
 the ID from the register. */
byte exec_rxdel(byte *data)
{
    xregister_remove(* (uint *) data);

    return EXEC_SIGPASS;
}

/* Create a new object in the register. */
byte exec_rxnew(byte *data)
{
    uint id     = * (uint *) data;
    byte const_ = *          data + 8;
    byte type   = *          data + 9;

    SaltObject obj = salt_object_create(id, type, PERM_USER, 0, NULL, 
                     NULL, 0, 0);

    void *val;
    if (type == SALT_STRING) {
        val = util_generate_data(type, data + 14); 
        memcpy(obj.typeinfo, data + 10, 4);
    }
    else {
        val = util_generate_data(type, data + 10);
    }

    obj.data = val;

    xregister_add(obj);

    return EXEC_SIGPASS;
}

/* Assign a new value to the object. */
byte exec_rxset(byte *data)
{
    uint id   = * (uint *) data;
    byte type = *          data + 8; 

    // TODO: other types
    int *val = alloc(sizeof(int), 1);

    SaltObject *obj = xregister_find(id);
    if (obj->type != type)
        return EXEC_SIGERR;

    free(obj->data);
    obj->data = val;

    return EXEC_SIGPASS;
}

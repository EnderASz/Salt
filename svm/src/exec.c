/**
 * exec.h implementation
 *
 * @author bellrise
 */
#include "../include/exec.h"
#include "../include/os.h"
#include "../include/utils.h"

#include <time.h>

/* Constant list of function pointers */
const struct SaltExec salt_execs[] = {
    {"DUMPV", exec_dumpv},
    {"PRINT", exec_print},
    {"SLEEP", exec_sleep},
    {"DUMPI", exec_dumpi},
    {"KILLX", exec_killx}
};

static short salt_execs_l = 5;

/* Locations of the start and finish labels */
uint exec_label_init = 0;
uint exec_label_end  = 0;

/* The main execution loop. Executes the instructions passed in the bytecode
 * by reading the 5 first characters as the instruction and finding the correct
 * function in salt_execs.
 *
 * @code: loaded bytecode
 *
 * returns: return code
 */
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

        if (signal == EXEC_SIGKILL) {
            return 1;
        }

    }

    return 0;
}

/* Preload the bytecode meaning: find the $__INIT__ and $__END__ labels, as well
 * as all else labels for faster access.
 *
 * @code: loaded bytecode
 */
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

/* Print the ID of the object (without a newline).
 *
 * @data id
 */
byte exec_dumpi(byte *data)
{
    uint id = * (uint *) data;
    uint oid = xregister.array[id].id;

    printf("%d", oid);

    return EXEC_SIGPASS;
}

/* Print the value of the object to standard out (without a newline). Note that
 * this is based on a switch case statement reading the type of the object to 
 * call the correct function, so it's a lot slower than just calling PRINT.
 *
 * @data id
 */
byte exec_dumpv(byte *data)
{
    uint id = * (uint *) data;

    util_print_object(&xregister.array[id]);

    return EXEC_SIGPASS;
}

/* Kill all threads and immediately exit the program calling core_clean().
 *
 * @data  none
 */
byte exec_killx(byte *data)
{
    return EXEC_SIGKILL;   
}

/* Print the constant string. 
 *
 * @data: the first 4 bytes indicate the constant string ID
 */
byte exec_print(byte *data)
{
    uint pos = * (uint *) data - 1;
    fputs((char *) salt_const_strings[pos].data, stdout);

    return EXEC_SIGPASS;
}

/* Sleep the given amount of miliseconds. 
 *
 * @data: the first 4 bytes are the time you want to sleep
 */
byte exec_sleep(byte *data)
{
    os_sleep(* (uint *) data); 

    return EXEC_SIGPASS;
}

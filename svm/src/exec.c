/**
 * exec.h implementation
 *
 * @author bellrise
 */
#include "../include/exec.h"

/* Constant list of function pointers */
const struct SaltExec salt_execs[] = {
    {"PRNTR", exec_prntr}
};

static short salt_execs_l = 1;

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

        for (short j = 0; j < salt_execs_l; j++) {
            
            if (strncmp(salt_execs[j].instruction, code[i], 4) == 0) {
                salt_execs[j].f_exec((byte *) (code[i] + 5));
                break;
            }

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

/* Print the constant string. 
 *
 * @data: the first 4 bytes indicate the constant string ID
 *
 */
void exec_prntr(byte *data)
{
    uint pos = * (uint *) data - 1;
    printf("%s", (char *) salt_const_strings[pos].data);
}

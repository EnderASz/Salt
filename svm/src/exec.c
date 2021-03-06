/**
 * exec.h implementation 
 *
 *
 */
#define SVM_DEBUG

#include "../include/core.h"
#include "../include/utils.h"
#include <stdio.h>
#include <string.h>

/* Main execution loop, this is called when the whole file is read. This
 * function is created with extreme detail to achieve a high execution speed
 * for the Virtual Machine.
 */
int exec(char* code[])
{
    // Move code pointer to init label.
    svm_cp = svm_init;

    // Run only if the instruction under the code pointer isn't the end label.
    while (svm_cp != svm_end) {
        DEBUG(printf("[exec] # %d\n", svm_cp));
        
        // Find exec func
        for (uint i = 0; i < execs_s; i++) {
            if (strncmp(execs[i].id, code[svm_cp], 5) == 0) {
                execs[i].f_exec((byte*) code[svm_cp] + 5);
                break;
            }
        }
        
        svm_cp++;
    }

    return 0;
}

/* Before execution each piece of bytecode is preloaded to find all the 
 * important labels, like $__INIT__, $__END__ and all the ELSE. This is done
 * to achieve better lookup speeds for the perticular label types as it can
 * be moved to a binary search algorithm. Fast lookup speeds mean quick jumps
 * to different code parts, increasing the speed.
 */
void preload(char* _code[])
{
    svm_else = smalloc(sizeof(uint), SVM_INITIAL_ELSE_S);
    svm_else_s = SVM_INITIAL_ELSE_S;

    uint i_else = 0;
    for (uint i = 0; i < svm_mcp; i++) {
        
        // @ELSE
        if (strncmp(_code[i], "@ELSE", 6) == 0) {
            svm_else[i_else] = i;
            i_else++;
        }

        // @$__INIT__
        if (strncmp(_code[i], "@$__INIT__", 11) == 0)
            svm_init = i;

        // @$__END__
        if (strncmp(_code[i], "@$__END__", 10) == 0)
            svm_end = i;
    
    }

    svm_else_s = i_else;

    DEBUG(
        printf("[preload] @$__INIT__ at: %d\n", svm_init);
        printf("[preload] @$__END__  at: %d\n", svm_end);
        for (uint i = 0; i < svm_else_s; i++) {
            printf("[preload] @ELSE at: %d\n", svm_else[i]);
        }
    );

}

// SALT INSTRUCTIONS

/* This instruction is located at the end of every function, even if the user
 * did not define so. This sends a CSPOP signal to the virtual machine, popping
 * one element off the callstack and returning to the previous scope.
 */
void si_retrn(byte* _bytes)
{
    DEBUG(printf("[RETRN] 000\n"));
}

/* This prints the constant string to standard out. The usual CALLX io print
 * is replaced with this as oftenly as possible by the compiler to reach better 
 * speeds.
 */
void si_prntr(byte* _bytes)
{
    DEBUG(printf("[PRNTR] str: %d\n", *(int*) _bytes));
}

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
int exec(char* _code[])
{
    // Move code pointer to init label.
    svm_cp = svm_init;

    // Run only if the instruction under the code pointer isn't the end label.
    while (svm_cp != svm_end) {
        DEBUG(printf("[exec] # %d\n", svm_cp));
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

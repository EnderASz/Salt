/**
 * Core file for the Salt Virual Machine. This side is written in C, to achieve
 * maximum speed in interacting with the software.
 *
 * Note that one instance of the virutal machine is one instance of executing 
 * code.
 */
#define SVM_DEBUG

#include "include/core.h"
#include "include/exec.h"
#include "include/utils.h"
#include <time.h>

/* Main entrypoint for the virtual machine. Calls the main functions which then
 * call their respective subsystems. Finally returns 0 if nothing goes wrong.
 */
int main(int argc, char** argv)
{
    // Timer
    struct timespec start, end;
    DEBUG(clock_gettime(CLOCK_REALTIME, &start));
    
    const char* filename = parse_args(argc, argv);
    char** bytecode = load_bytecode(filename);

    preload(bytecode);
    load_strings(bytecode);

    exec(bytecode);

    // Timer
    DEBUG(
    clock_gettime(CLOCK_REALTIME, &end);
    printf("\n\nExecution took: %ld us\n", ((end.tv_nsec - start.tv_nsec) 
           + (end.tv_sec - start.tv_sec) * 1000000000) / 1000);
    );
    return 0;
}

/**
 * Core file for the Salt Virual Machine. This side is written in C, 
 * to achieve maximum speed in interacting with the software.
 *
 * Note that one instance of the virutal machine is one instance of
 * executing code.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/core.h"

/* Main entrypoint for the virtual machine. Takes care of executing
 * all the parts.
 */
int main(int argc, char** argv)
{
    if (argc < 2) {
        printf("Not enough arguments. See --help for more.\n");
        exit(1);
    }

    // Check for -h, --help
    for (uint i = 0; i < argc; i++) {
        if ((strncmp(argv[i], "-h", 20) == 0) || 
            (strncmp(argv[i], "--help", 20) == 0))
            core_help();
    }

}

/**
 * Core file for the Salt Virual Machine. This side is written in C, to achieve
 * maximum speed in interacting with the software.
 *
 * Note that one instance of the virutal machine is one instance of executing 
 * code.
 */
#include "include/core.h"
#include "include/exec.h"

/* Main entrypoint for the virtual machine. Calls the main functions which then
 * call their respective subsystems. Finally returns 0 if nothing goes wrong.
 */
int main(int argc, char** argv)
{
    const char* filename = parse_args(argc, argv);
    char** bytecode = load_bytecode(filename);

    exec(bytecode);

    return 0;
}

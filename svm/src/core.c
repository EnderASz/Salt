/* core.h implementation
 */
#include "../include/core.h"
#include "../include/utils.h"
#include "../include/exec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Virtual Machine global variables

/* The main array of Salt executable units. Each element represents a single
 * callable f_exec function. 
 */
const SaltExec execs[] = {
    {"PRNTR", si_prntr},
    {"RETRN", si_retrn},
    {"\0\0n", NULL}
};
const uint execs_s = 2;

/* This 16-byte header should be located on top of each .scc file. The 
 * SCC_HEADER contains the "magic bytes" indicating that this is indeed a scc
 * (salt compiled code) file. The second 8 bytes, SCC_VERSION indicate the 
 * format of the compiled code. If the format does not match the virtual 
 * machine version, it will not execute.
 */
const char SCC_HEADER  [] = {0x7f, 0x53, 0x43, 0x43, 0x00, 0x00, 0x00, 0x00};
const char SCC_VERSION [] = {0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f};

/* uint max code pointer: the maximum amount cp can have before exiting
 * execution. This is set by load_bytecode when reading bytecode from the 
 * input .scc file.
 */
uint svm_mcp = 0;

/* uint code pointer: points to the current location on the code array.
 * This is chosen rather than a simple for loop in order to have more control
 * over the execution flow, like making jumps.
 */
uint svm_cp = 0;

/* These are the locations of the $__INIT__ and $__END__ labels defined in the
 * preload function. These are used for as the entrypoint and exit of the 
 * program. If exec matches svm_cp to svm_end, it exits the program execution. 
 */
uint svm_init = 0;
uint svm_end  = 0;

/* Both of those variables store information about the ELSE labels found in
 * the bytecode. This is done to quickly and efficiently jump to the next 
 * section when the `if` statement returns false.
 */
uint* svm_else;
uint  svm_else_s = 0;


/* Shows the help page and exits the whole program.
 */
void core_help()
{
    printf("%s\n\n%s\n%s\n", "Usage: svm [OPTIONS]... FILE",
    "\tFILE        compiled Salt code (scc) file",
    "\t-h, --help  show this page and exit");
    exit(1);
}

/* Parse the passed arguments and set special flags defined depending on the 
 * arguments used when executing the program. This also returns the filename
 * that the user chose to execute.
 */
char* parse_args(int argc, char** argv) 
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

    // Currently set to the first parameter, this will be smarter later on
    return argv[1];
}

/* Returns every line of the bytecode in a string array. The length of the
 * array is automatically set to the global uint mbp. The first list is omited
 * from the final string array because it contains the bytecode header which
 * contains some crucial information about the bytecode itself.
 */
char** load_bytecode(const char* _filen)
{
    FILE* fp = fopen(_filen, "rb");
    if (!fp) {
        printf("Error: cannot read file\n");
        exit(1);
    }
    
    char buf[SVM_INSTRUCTION_WIDTH];
    
    // Check for .scc header before memory allocation
    fgets(buf, SVM_INSTRUCTION_WIDTH - 1, fp);
    if (strncmp(buf, SCC_HEADER, 8) != 0) {
        printf("svm: this is not an scc executable. Exiting\n");
        exit(1);
    }

    // Check for version
    if (strncmp(buf + 8, SCC_VERSION, 8) != 0) {
        printf("svm: incompatible scc file format. Exiting\n");
        exit(1);
    }

    // Size of code
    uint code_s = SVM_INITIAL_CODE_S;
    char** code = smalloc(sizeof(char*), code_s);
    for (uint i = 0; i < SVM_INITIAL_CODE_S; i++)
        code[i] = smalloc(sizeof(char), SVM_INSTRUCTION_WIDTH);

    // Reading the bytecode file needs to moved to a while loop in order to 
    // read any amount of bytes from the line, because using a simple fgets
    // stops at the first null-byte, which are also a part of the bytecode.
    // If the file is no longer readable, break.
    uint status = 0;
    
    for (uint line;;line++) {
        // Read line and break if EOF
        if (load_line(fp, code[line]) == EOF)
            break;

        svm_mcp = line + 1;
    }

    fclose(fp);
    return code;
}

/* Load a single line from the file until it finds a newline. This has been
 * moved here instead of using fgets, because it needs to load bytes after
 * a null terminator. Note, the max _alloc size is by default set to 
 * SVM_INSTRUCTION_WIDTH - 1.
 */
int load_line(FILE* _fp, char* _alloc)
{
    char c = 0;
    for (uint i = 0; i < SVM_INSTRUCTION_WIDTH; i++) {
        _alloc[i] = fgetc(_fp);
        if (_alloc[i] == EOF) {
            return -1;
        }

        if (_alloc[i] == '\n') {
            _alloc[i] = 0;
            break;
        }
    }
    return 0;
}

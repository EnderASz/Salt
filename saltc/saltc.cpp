/**
 * Core Salt compiler file. Takes care of the arguments and runs the core
 * parts.
 *
 * @author bellrise
 */
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

#include "include/utils.h"

using std::string;

/* Print the help page and exit the program. This will probably be moved to
 * a help.txt file later on in development.
 */
void core_print_help()
{
    printf("%s\n\n%s\n%s\n", "Usage: saltc [OPTIONS]... FILE", 
    "\tFILE         name of the file to be compiled",
    "\t-h, --help   show this page");
    exit(0);
}

/* This is the main function for the salt compiler. It takes care of the 
 * arguments and controls the core parts of this program, like the parser,
 * tokenizer, validator and synthesizer.
 */
int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please specify the filename. For more information use --help.\n");
        exit(1);
    }

    // Load args
    std::vector<string> args;
    for (uint i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Find --help or -h
    for (string& arg : args) {
        if ((arg == "--help") || (arg == "-h"))
            core_print_help();
    }

    return 0;
}
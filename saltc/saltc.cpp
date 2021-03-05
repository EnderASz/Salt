/**
 * Core Salt compiler file. 
 *
 *
 */
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <string>

void core_print_help()
{
    printf("%s\n\n%s\n%s\n", "Usage: saltc [OPTIONS]... FILE", 
    "\tFILE         name of the file to be compiled",
    "\t-h, --help   show this page");
    exit(0);
}

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please specify the filename. For more information use --help.\n");
        exit(1);
    }

    // Load args
    std::vector<std::string> args;
    for (uint i = 0; i < argc; i++) {
        args.push_back(argv[i]);
    }

    // Find --help or -h
    for (std::string& arg : args) {
        if ((arg == "--help") || (arg == "-h"))
            core_print_help();
    }

    return 0;
}
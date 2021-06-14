/**
 * arg_parser/params.h implementation
 *
 * @author EnderASz
 */
#include "../include/params.h"

#include "../include/utils.h"
#include "../include/logging.h"
#include "../include/error.h"
#include <queue>
#include <cstring>
#include <string>

using std::string;

namespace salt
{

/**
 * The initObject method is responsible for parse arguments and
 * initiate member variables of Params class object.
 */
void Params::initObject(std::queue<string> args) {
    dprint("Setting up compiler executable path");
    executable_path = pop<string>(args);
    dprint(
        "Compiler executable path setted up at: %s",
        executable_path.c_str());
    while(!args.empty()) {
        string arg = pop<string>(args);
        dprint("Parsing parameter: %s", arg.c_str());
        if (Params::arg_comp(arg, "--help", "-h")) {
            dprint("Displaying help page");
            print_help_page();
            dprint("Help page displayed");
            exit(0);
        }
        else if (Params::arg_comp(arg, "--no-builtins", "")) {
            dprint("Switching include builtins off");
            builtins = false;
            dprint("Include builtins switched off");
        }
        else if (Params::arg_comp(arg, "--output", "-o")) {
            dprint("Setting up output file path");
            if(args.empty()) {
                eprint(MissingOptionArgumentError, arg);
            }
            output_path = pop<string>(args);
            dprint(
                "Output file path setted up at: %s",
                output_path.c_str());
        }
        else if (arg[0] == '-') {
            eprint(UnrecognizedOptionError, arg);
        } else { // Nameless arguments
            if (input_path.empty()) {
                dprint("Setting up input file path");
                input_path = arg;
                dprint(
                    "Output file path setted up at: %s",
                    input_path.c_str());
            }
            else {
                wprint(
                    "Argument '%s' can not be parsed now. "
                    "This will be implemented in a future",
                    arg.c_str());
            }
        }
    }

    if (input_path.empty()) {
        eprint(UnspecifiedMainError);
    }
}

/**
 * Params class constructor
 * 
 * @argc - amount of arguments
 * @argv - array of strings (char*) arguments
 */ 
Params::Params(uint argc, char* argv[]) {
    std::queue<string> args;
    for (uint i = 0; i < argc; i++) {
        args.push(argv[i]);
    }
    this->initObject(args);
}

/**
 * Params class constructor
 * 
 * @argc - amount of arguments
 * @argv - array of strings arguments
 */ 
Params::Params(uint argc, string argv[]) {
    std::queue<string> args;
    for (uint i = 0; i < argc; i++) {
        args.push(argv[i]);
    }
    this->initObject(args);
}

/**
 * Params class constructor
 * 
 * @args - Queue (std::queue) of string arguments
 */ 
Params::Params(std::queue<string> args) {this->initObject(args);}

bool Params::arg_comp(
    const string str,
    const string long_arg,
    const char short_arg[3]) {
        return !(str.compare(short_arg) && str.compare(long_arg));
    }

/* Gets executable path argument value */
string Params::getExecutablePath() {return this->executable_path;}

/* Gets input path argument value */
string Params::getInputPath() {return this->input_path;}

/* Gets output path argument value */
string Params::getOutputPath() {return this->output_path;}

/* Gets builtins include switch value */
bool Params::getBuiltinsSwitch() {return this->builtins;}

void Params::print_help_page() {
    printf(
        "Usage: saltc [OPTIONS]... FILE\n\n"
        "\tFILE                 "
            "name of the file to be compiled\n"
        "\t-h, --help           "
            "show this page\n"
        "\t-o, --output <path>  "
            "path of the compilation output file\n"
        "\t--no-builtins        "
            "don't link builtin functionality when compiling\n"
        "\n");
}

};
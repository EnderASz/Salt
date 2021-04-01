/**
 * arg_parser/params.h implementation
 *
 * @author EnderASz
 */
#include "../../include/arg_parser/params.h"

#include "../../include/arg_parser/prints.h"
#include "../../include/utils.h"
#include "../../include/logging.h"
#include <queue>
#include <cstring>
#include <string>

using std::string;

namespace salt::arg_parser
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
            prints::help();
            dprint("Help page displayed");
        }
        else if (Params::arg_comp(arg, "--no-builtins", "")) {
            dprint("Switching include builtins off");
            builtins = false;
            dprint("Include builtins switched off");
        }
        else if (Params::arg_comp(arg, "--output", "-o")) {
            dprint("Setting up output file path");
            output_path = pop<string>(args);
            dprint(
                "Output file path setted up at: %s",
                output_path.c_str());
        }
        else if (arg[0] == '-')
            prints::unrecognized_option_error(arg);
        else { // Nameless arguments
            if (input_path.empty()) {
                dprint("Setting up input file path");
                input_path = arg;
                dprint(
                    "Output file path setted up at: %s",
                    input_path.c_str());
            }
            else {
                prints::too_many_nameless_error();
            }
        }
    }

    if (input_path.empty()) {
        prints::specify_filename_error();
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

};
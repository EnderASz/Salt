/**
 * arg_parser/params.h implementation
 *
 * @author EnderASz
 */
#include "../../include/arg_parser/params.h"

#include "../../include/arg_parser/prints.h"
#include "../../include/utils.h"
#include <cstdio>
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
    this->executable_path = pop<string>(args);

    while(!args.empty()) {
        string arg = pop<string>(args);
        if (Params::arg_comp(arg, "--help", "-h"))
            prints::help();
        else if (Params::arg_comp(arg, "--output", "-o"))
            this->output_path = pop<string>(args);
        else if (arg[0] == '-')
            prints::unrecognized_option_error(arg);
        else // Nameless arguments
            if (this->input_path.empty())
                this->input_path = arg;
            else {
                prints::too_many_nameless_error();
            }
    }

    if (this->input_path.empty()) {
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
    string str,
    string long_arg,
    const char short_arg[3]) {
        return (!std::strcmp(str.c_str(), short_arg)) ||
            (!std::strcmp(str.c_str(), long_arg.c_str()));
    }

/* Gets executable path argument value */
string Params::getExecutablePath() {return this->executable_path;}

/* Gets input path argument value */
string Params::getInputPath() {return this->input_path;}

/* Gets output path argument value */
string Params::getOutputPath() {return this->output_path;}

};
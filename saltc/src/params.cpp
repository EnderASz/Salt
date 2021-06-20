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
 * Params class constructor
 * 
 * @args - Queue (std::queue) of string arguments
 */ 
Params::Params(std::queue<string> args): arg_list(args) {initObject();}

Params::Params(const std::vector<string>& args)
    : arg_list(std::deque<string>(args.begin(), args.end())) 
    {
        initObject();
    }

bool Params::arg_comp(
    const string str,
    const string long_arg,
    const char short_arg[3]) {
        return !(str.compare(short_arg) && str.compare(long_arg));
    }

/* Gets executable path argument value */
const string& Params::getExecutablePath() {return this->executable_path;}

/* Gets input path argument value */
const std::vector<string>& Params::getInputPaths() {return this->input_paths;}

/* Gets output path argument value */
const string& Params::getOutputPath() {return this->output_path;}

/* Gets builtins include switch value */
bool Params::getBuiltinsSwitch() {return this->builtins;}

/**
 * The initObject method is responsible for parse arguments and
 * initiate member variables of Params class object.
 */
void Params::initObject() {
    dprint("Setting up compiler executable path");
    executable_path = popArg();
    dprint(
        "Compiler executable path setted up at: %s",
        executable_path.c_str());
    string arg;
    while(!(arg = popArg()).empty()) {
        dprint("Parsing parameter: %s", arg.c_str());
        if (Params::arg_comp(arg, "--help", "-h")) helpAction();
        if (Params::arg_comp(arg, "--no-builtins", "")) {
            noBuiltinsAction();
            continue;
        }
        if (Params::arg_comp(arg, "--output", "-o")) {
            setOutputAction();
            continue;
        }
        if (arg[0] != '-') {
            addInputAction(arg);
            continue;
        }
        eprint(UnrecognizedOptionError, arg);
    }
    if (input_paths.empty())
        eprint(UnspecifiedMainError);
}

string Params::popArg() {
    string arg = !arg_list.empty() ? arg_list.front() : "";
    if(!arg.empty()) arg_list.pop();
    return arg;
}

void Params::helpAction() {
    dprint("Displaying help page");
    printf(
        "Usage: saltc [OPTIONS]... FILES...\n\n"
        "\tFILES...             "
            "paths of files to be compiled\n"
        "\n"
        "\t-h, --help           "
            "show this page\n"
        "\t-o, --output <path>  "
            "path of the compilation output file\n"
        "\t--no-builtins        "
            "don't link builtin functionality when compiling\n"
        "\n");
    dprint("Help page displayed");
    exit(0);
}

void Params::noBuiltinsAction() {
    dprint("Switching include builtins off");
    builtins = false;
    dprint("Include builtins switched off");
}

void Params::setOutputAction() {
    dprint("Setting up output file path");
    if((output_path = popArg()).empty())
        eprint(MissingOptionArgumentError, "--output / -o");
    dprint("Output file path setted up at: %s", output_path.c_str());
}

void Params::addInputAction(string filepath) {
    dprint("Adding input file path...");
    input_paths.push_back(filepath);
    dprint("Added input file path: %s", filepath.c_str());
}

};
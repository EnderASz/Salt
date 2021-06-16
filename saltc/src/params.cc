/**
 * arg_parser/params.h implementation
 *
 * @author EnderASz
 */
#include <saltc/logging.h>
#include <saltc/params.h>
#include <saltc/error.h>

#include <cstring>

namespace salt
{

void Params::initObject(std::queue<string> args) {
    dprint("Setting up compiler executable path");
    executable_path = pop<string>(args);

    dprint(
        "Compiler executable path setted up at: %s",
        executable_path.c_str()
    );

    while (!args.empty()) {
        string arg = pop<string>(args);
        dprint("Parsing parameter: %s", arg.c_str());

        if (Params::arg_comp(arg, "--help", "-h")) {

            // Displaying help page

            dprint("Displaying help page");
            print_help_page();
            exit(0);

        } else if (Params::arg_comp(arg, "--no-builtins", "")) {

            // Switching include builtins off

            dprint("Switching include builtins off");
            builtins = false;

        } else if (Params::arg_comp(arg, "--output", "-o")) {

            // Setting up output file path

            dprint("Setting up output file path");
            if(args.empty()) {
                eprint(MissingOptionArgumentError, arg);
            }
            output_path = pop<string>(args);
            dprint(
                "Output file path set up at: %s",
                output_path.c_str());

        } else if (arg[0] == '-') {

            // Unrecognized optioon

            eprint(UnrecognizedOptionError, arg);

        } else {

            // Unrecognized optioon

            if (input_path.empty()) {
                dprint("Setting up input file path");
                input_path = arg;
                dprint(
                    "Input file path set up at: %s",
                    input_path.c_str()
                );
            } else {
                wprint(
                    "Argument '%s' cannot be parsed now. "
                    "This will be implemented in a future",
                    arg.c_str()
                );
            }
        }
    }

    if (input_path.empty())
        eprint(UnspecifiedMainError);
}

Params::Params(int argc, char* argv[]) {
    std::queue<string> args;
    for (int i = 0; i < argc; i++) {
        args.push(argv[i]);
    }
    this->initObject(args);
}

Params::Params(int argc, string argv[]) {
    std::queue<string> args;
    for (int i = 0; i < argc; i++) {
        args.push(argv[i]);
    }
    this->initObject(args);
}

Params::Params(std::queue<string> args) {this->initObject(args);}

bool Params::arg_comp(const string str, const string long_arg,
    const char short_arg[3]) {
    return !(str.compare(short_arg) && str.compare(long_arg));
}

string Params::getExecutablePath() {
    return this->executable_path;
}

string Params::getInputPath() {
    return this->input_path;
}

string Params::getOutputPath() {
    return this->output_path;
}

bool Params::getBuiltinsSwitch() {
    return this->builtins;
}

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
        "\n"
    );
}

};
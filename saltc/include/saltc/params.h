/**
 * Params class is the parser of arguments. It parses given arguments and
 * options and contains their values in private member variables
 * accesible by "get" methods.
 *
 * @author EnderASz, throwbly, bellrise
 */
#ifndef CORE_PARAMS_H_
#define CORE_PARAMS_H_

#include "utils.h"
#include <string>
#include <queue>

using std::string;

namespace salt
{

/**
 * The Params class represents list of all command line parameters.
 * Its constructors parse arguments and saves parsed data in object members.
 */
class Params
{
private:
    string executable_path;
    string input_path;
    string output_path = "a.scc";
    bool builtins = true;

    /**
     * The initObject method is responsible for parse arguments and
     * initiate member variables of Params class object.
     */
    void initObject(std::queue<string>args);

public:

    /**
     * Params class constructor
     *
     * @argc - amount of arguments
     * @argv - array of strings (char*) arguments
     */
    Params(std::queue<string> args);
    Params(int argc, char* argv[]);
    Params(int argc, string args[]);

    /**
     * Compares the given string with the given short and long argument names.
     *
     * @str - source string
     * @long_arg - short version
     * @short_arg - long version
     */
    static bool arg_comp(
        const string str,
        const string long_arg,
        const char short_arg[3]);

    /**
     * Returns the path to the executable.
     */
    string getExecutablePath();

    /**
     * Returns the source file path.
     */
    string getInputPath();

    /**
     * Returns the output file path.
     */
    string getOutputPath();

    /**
     * Returns false if --no-builtins is defined, otherwise true.
     */
    bool getBuiltinsSwitch();

    /**
     * Prints the help page.
     */
    static void print_help_page();

};

}; // namespace salt

#endif // CORE_PARAMS_H_
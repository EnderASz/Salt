/**
 * Params class is the parser of arguments. It parses given arguments and
 * options and contains their values in private member variables
 * accesible by "get" methods.
 * 
 * @author EnderASz
 */ 
#ifndef CORE_PARAMS_H_
#define CORE_PARAMS_H_

#include "../utils.h"
#include <queue>
#include <string>

using std::string;

namespace salt::arg_parser
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

    /**
     * The initObject method is responsible for parse arguments and
     * initiate member variables of Params class object.
     */
    void initObject(std::queue<string>args);

public:
    
    Params(std::queue<string> args);
    Params(uint argc, char* argv[]);
    Params(uint argc, string args[]);

    /* Compares given string with given short and long argument names. */
    static bool arg_comp(
        string str,
        string long_arg,
        const char short_arg[3]);

    /* Gets executable path argument value */
    string getExecutablePath();

    /* Gets input path argument value */
    string getInputPath();

    /* Gets output path argument value */
    string getOutputPath();
}; // salt::core::Params

}; // salt::core

#endif // CORE_PARAMS_H_
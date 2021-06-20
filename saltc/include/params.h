/**
 * Params class is the parser of arguments. It parses given arguments and
 * options and contains their values in private member variables
 * accesible by "get" methods.
 * 
 * @author EnderASz
 */ 
#ifndef CORE_PARAMS_H_
#define CORE_PARAMS_H_

#include "utils.h"
#include <queue>
#include <string>

using std::string;

namespace salt
{

/**
 * The Params class represents list of all command line parameters.
 * Its constructors parse arguments and saves parsed data in object members.
 */
class Params
{

public:
    
    Params(std::queue<string> args);
    Params(const std::vector<string>& args);

    /* Compares given string with given short and long argument names. */
    static bool arg_comp(
        const string str,
        const string long_arg,
        const char short_arg[3]);

    /* Gets executable path argument value */
    const string& getExecutablePath();

    /* Gets input paths argument value */
    const std::vector<string>& getInputPaths();

    /* Gets output path argument value */
    const string& getOutputPath();

    /* Gets import init switch value */
    bool getBuiltinsSwitch();

private:
    string executable_path;
    std::vector<string> input_paths;
    string output_path = "a.scc";
    bool builtins = true;

    /**
     * Argument list passed via constructor and used by initObject method to
     * interpret command line 
     */
    std::queue<string> arg_list;

    /**
     * The initObject method is responsible for parse arguments and
     * initiate member variables of Params class object.
     */
    void initObject();

    /**
     * Removes first argument from 'arg_list' and returns it or returns empty
     * string if 'arg_list' is empty.
     */
    string popArg();

    void helpAction();

    void noBuiltinsAction();

    void setOutputAction();

    void addInputAction(string filepath);

}; // salt::core::Params

}; // salt::core

#endif // CORE_PARAMS_H_
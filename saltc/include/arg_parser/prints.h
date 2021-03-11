/**
 * 'arg_parser::prints' is a namespace cointains functions responsible for print common
 * messages in command line during arguments parsing.
 * 
 * @author EnderASz
 */
#ifndef CORE_PRINTS_H_
#define CORE_PRINTS_H_

#include <string>
using std::string;

namespace salt::arg_parser::prints
{

    void help();
    void unrecognized_option_error(string arg_name);
    void too_many_nameless_error();
    void specify_filename_error();

}; // salt::core::prints

#endif
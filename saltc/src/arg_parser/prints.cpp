/**
 * arg_parser/prints.h implementation
 * 
 * @author EnderASz
 */
#include "../../include/arg_parser/prints.h"

#include "../../include/arg_parser/consts.h"
#include <cstdio>

namespace consts = salt::arg_parser::consts;

namespace salt::arg_parser::prints
{

/** 
 * Print the help page and exit the program. This will probably be moved to
 * a help.txt file later on in development.
 */
void help() {
    printf(consts::HELP_PAGE);
    exit(0);
}

/**
 * Print the error about use unrecognized option in command line and
 * exit the program.
 */
void unrecognized_option_error(string arg) {
    printf(consts::UNRECOGNIZED_OPTION_FORMAT, arg);
    exit(1);
}

/**
 * Print the error about use too many nameless arguments in command line and
 * exit the program.
 */
void too_many_nameless_error() {
    printf(consts::TOO_MANY_NAMELESS_ARGS_MESSAGE);
    exit(1);
}

/**
 * Print the error about unspecified filepath to compile in command line and
 * exit the program.
 */
void specify_filename_error() {
    printf(consts::SPECIFY_FILENAME_MESSAGE);
    exit(1);
}

};
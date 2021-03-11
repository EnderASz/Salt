/**
 * The namespace contains constances messages and string formats for the Salt Compiler Arguments Parser.
 * 
 * @author EnderAsz
 */
#ifndef CONSTS_H_
#define CONSTS_H_

namespace salt::arg_parser::consts
{

    const char HELP_PAGE[158] = (
        "Usage: saltc [OPTIONS]... FILE\n\n"
        "\tFILE         name of the file to be compiled\n"
        "\t-h, --help   show this page\n"
        "\t-o, --output path of the compilation output file\n");

    const char UNRECOGNIZED_OPTION_FORMAT[80] = (
        "Error: unrecognized command line option '%s'. "
        "For more information use --help.\n");

    const char TOO_MANY_NAMELESS_ARGS_MESSAGE[70] = (
        "Error: Too many nameless arguments. For more information use --help.\n");

    const char SPECIFY_FILENAME_MESSAGE[81] = (
        "Error: Please specify the filename to compile. "
        "For more information use --help.\n");

} // salt::consts

#endif
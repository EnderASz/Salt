/**
 * The namespace contains constances messages and string formats for the Salt Compiler Arguments Parser.
 * 
 * @author EnderAsz
 */
#ifndef CONSTS_H_
#define CONSTS_H_

namespace salt::arg_parser::consts
{

    const char HELP_PAGE[] = (
        "Usage: saltc [OPTIONS]... FILE\n\n"
        "\tFILE                 name of the file to be compiled\n"
        "\t-h, --help           show this page\n"
        "\t-o, --output <path>  path of the compilation output file\n"
        "\t--no-builtins        don't include builtin functionality when compiling\n");

    const char UNRECOGNIZED_OPTION_FORMAT[] = (
        "Error: unrecognized command line option '%s'. "
        "For more information use --help.\n");

    const char TOO_MANY_NAMELESS_ARGS_MESSAGE[] = (
        "Error: Too many nameless arguments. For more information use --help.\n");

    const char SPECIFY_FILENAME_MESSAGE[] = (
        "Error: Please specify the filename to compile. "
        "For more information use --help.\n");

} // salt::arg_parser::consts

#endif
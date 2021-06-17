/**
 * Core Salt compiler file. Takes care of the arguments and runs the core
 * parts.
 *
 * @author bellrise
 * @author EnderASz
 */

#include "include/params.h"
#include "include/utils.h"
#include "include/source_file.h"
#include "include/logging.h"
#include "include/tokenizer.h"

using namespace salt;

/* This is the main function for the salt compiler. It takes care of the 
 * arguments and controls the core parts of this program, like the
 * precompilation, parser, tokenizer, validator and synthesizer.
 */
int main(int argc, char* argv[]) {
    setup_windows_logging();
    dprint("Parsing parameters");
    Params parameters({argv, argv + argc});
    dprint("Parameters parsed");
    reset_print_padding();

    string main_path = parameters.getInputPaths().at(0);
    iprint(
        "Initializing main source file from: %s",
        main_path.c_str());
    SourceFile main_source(main_path);
    if(parameters.getBuiltinsSwitch())
        main_source.includeBuiltins();
    Tokenizer main_tokenizer(main_source);
    

    return 0;
}
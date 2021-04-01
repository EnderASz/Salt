/**
 * Core Salt compiler file. Takes care of the arguments and runs the core
 * parts.
 *
 * @author bellrise
 * @author EnderASz
 */

#include "include/arg_parser/params.h"
#include "include/utils.h"
#include "include/source_file.h"
#include "include/logging.h"

using namespace salt;

/* This is the main function for the salt compiler. It takes care of the 
 * arguments and controls the core parts of this program, like the
 * precompilation, parser, tokenizer, validator and synthesizer.
 */
int main(int argc, char* argv[]) {
    dprint("Parsing parameters");
    arg_parser::Params parameters((uint) argc, argv);
    dprint("Parameters parsed");

    dprint(
        "Initializing main source file from: %s",
        parameters.getInputPath().c_str());
    SourceFile main_source(parameters.getInputPath());
    if(parameters.getBuiltinsSwitch())
        main_source.includeBuiltins();

    return 0;
}
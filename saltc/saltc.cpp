/**
 * Core Salt compiler file. Takes care of the arguments and runs the core
 * parts.
 *
 * @author bellrise
 * @author EnderASz
 */

#include "include/arg_parser/params.h"
#include "include/utils.h"
#include "include/precompiler.h"

using namespace salt;

/* This is the main function for the salt compiler. It takes care of the 
 * arguments and controls the core parts of this program, like the
 * precompilation, parser, tokenizer, validator and synthesizer.
 */
int main(int argc, char* argv[]) {
    arg_parser::Params parameters((uint) argc, argv);
    string salt_code = load_file(parameters.getInputPath());
    precompiler::init_paste(salt_code);
    precompiler::clean_comments(salt_code);

    return 0;
}
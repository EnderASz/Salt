/**
 * Core Salt compiler file. Takes care of the arguments and runs the core
 * parts.
 *
 * @author bellrise
 * @author EnderASz
 */

#include <saltc/source_file.h>
#include <saltc/tokenizer.h>
#include <saltc/logging.h>
#include <saltc/params.h>
#include <saltc/utils.h>

using namespace salt;

int main(int argc, char* argv[]) {
    setup_windows_logging();
    dprint("Parsing parameters");
    Params parameters(argc, argv);
    reset_print_padding();

    iprint(
        "Initializing main source file from: %s",
        parameters.getInputPath().c_str()
    );

    SourceFile main_source(parameters.getInputPath());

    if (parameters.getBuiltinsSwitch())
        main_source.includeBuiltins();

    Tokenizer main_tokenizer(main_source);

    return 0;
}
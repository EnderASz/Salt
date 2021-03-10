/**
 * Salt Virtual Machine
 *
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 *
 * Code style
 *
 * Note for every developer wanting to contribute to the svm: the core goal is
 * execution speed, memory usage comes isn't as important in today's times.
 * The code must be clean and easy to read, without too many line comments but
 * with rich documentation in the form of block comments. Functions & variables
 * both have lower_case_names. Constants are always UPPER_CASE and structures
 * are PascalCase. Pointers are near the value, not the type.
 * 
 * Comments should be located on top of every file, and should follow the two
 * star style. Other comments above functions should have one star and should
 * start from the first line. The function commenting style is simmilar to the
 * Linux Kernel style, although it's a bit modified.
 *
 * Coding rules
 *
 * Functions should be short and do only one thing very well. If your function
 * is longer than 50 lines, you should think about splitting it - but it's not
 * required. Code should be clear and easy to read, use whitespace god damnit!
 * Function names are constructed using the topic as the first word, and then
 * the class & verb. Use `core_unit_clear` instead of `core_clear_unit`. The
 * max line width is 80 characters, and there are no excuses. If you write even
 * one character over the 80 char limit, you're certified braindead.
 *
 * Keep your code english: write proper english comments in a simple style so
 * others can understand you. 
 *
 * Versions 
 *
 * Every new set of functionality or bugfixes should get a new `minor` version,
 * and every major release should get a new `major` version. Simple as that.
 *
 *
 * @author   bellrise
 * @version  0.1
 */
#include "include/core.h"
#include "include/object.h"
#include "include/exec.h"
#include "include/utils.h"
#include "include/os.h"

/* This string will show up in the compiled version of SVM which you can then
 grep to, checking the format. */
const char *svm_version_string = "SVM: format 1";


int main(int argc, char **argv)
{
    dprintf("[!] USING DEBUG SVM BUILD, DO NOT USE IN PRODUCTION\n");
    char *filename = core_parse_args(argc, argv);

    if (FLAG_HELP)
        core_show_help();

    FILE *fp = fopen(filename, "rb"); 
    if (!fp)
        CORE_ERROR("Cannot read file\n");

    // Load code
    core_load_header(fp);
    core_load_strings(fp);
    char **code = core_load_bytecode(fp);

    fclose(fp);

    // Initialize variables
    core_init();
    
    // Execute
    preload(code);
    int ret = exec(code);

    for (uint i = 0; i < xregister.size; i++) {
        printf("variable %d of type %02hhx\n", xregister.array[i].id, xregister.array[i].type);
    }

    // Deallocate memory
    core_clean(code);

    return ret;
}

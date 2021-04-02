/**
 * args.h implementation
 *
 * @author  bellrise, 2021
 */
#include "../include/args.h"
#include "../include/core.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static u8 arg(char *str, const char *_long, const char *_short)
{
    if (strcmp(str, _long) == 0 || strcmp(str, _short) == 0)
        return 1;
    return 0;
}

static void help_page(SVMRuntime *_rt)
{
    printf("Usage: svm [OPTION]... FILE\n"
           "svm - Salt Virtual Machine for executing compiled salt code\n\n"
           "  FILE          the compiled salt executable\n"
           "  -h, --help    show this and exit\n"
           "  -v, --version show the version and exit\n"
           "  -m, --mem-used show the amount of memory used at the end\n");
    core_exit(_rt);
}

static void version_page(SVMRuntime *_rt)
{
    printf("%s\n", SVM_VERSION);
    core_exit(_rt);
}

char *args_parse(SVMRuntime *_rt, int argc, char **argv) Nullable
{
    char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        dprintf("Checking [%d] \"%s\"\n", i, argv[i]);

        if (arg(argv[i], "--help", "-h"))
            help_page(_rt);
        else if (arg(argv[i], "--version", "-v"))
            version_page(_rt);
        else if (arg(argv[i], "--mem-used", "-m"))
            _rt->arg_mem_used = 0x01;
        else
            filename = argv[i];
    }
    return filename;
}

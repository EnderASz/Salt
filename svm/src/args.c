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


static byte arg(char *str, const char *_long, const char *_short)
{
    if (strcmp(str, _long) == 0 || strcmp(str, _short) == 0)
        return 1;
    return 0;
}

static void help_page()
{
    printf("Usage: svm [OPTION]... FILE\n"
           "svm - Salt Virtual Machine for executing compiled salt code\n\n"
           "  FILE          the compiled salt executable\n"
           "  -h, --help    show this and exit\n"
           "  -v, --version show the version and exit\n");
    core_exit();
}

static void version_page()
{
    printf("%s\n", SVM_VERSION);
    core_exit();
}

Nullable char *args_parse(int argc, char **argv)
{
    char *filename = NULL;
    for (int i = 1; i < argc; i++) {
        dprintf("Checking [%d] \"%s\"\n", i, argv[i]);

        if (arg(argv[i], "--help", "-h"))
            help_page();
        else if (arg(argv[i], "--version", "-v"))
            version_page();
        else
            filename = argv[i];
    }
    return filename;
}

/**
 * Salt Virtual Machine
 * 
 * Copyright (C) 2021  The Salt Programming Language Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * END OF COPYRIGHT NOTICE
 *
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 * 
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

char *args_parse(SVMRuntime *_rt, i32 argc, char **argv) Nullable
{
    char *filename = NULL;
    for (i32 i = 1; i < argc; i++) {
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

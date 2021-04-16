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
 * args.h implementation
 *
 * @author  bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/args.h"
#include "../include/exception.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/* Argument parser errors */
#define ARG_INVALID_VALUE   -1
#define ARG_NO_ARGUMENT     -2

/* This is defined in svm.c */
extern const char *svm_grep_string;


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
           "  FILE                      the compiled salt executable\n"
           "  -h, --help                show this and exit\n"
           "  -v, --version             show the version and exit\n"
           "  -m, --mem-used            show the amount of memory used at the end\n"
           "  -d, --allow-debug         allow debug output from MLMAP & TRACE\n"
           "  -l, --limit-mem [KB]      limit the memory usage to a certain amount of kilobytes\n"
           "  -D, --decompile           decompile the passed file\n"
    );
    core_exit(_rt);
}

static void version_page(SVMRuntime *_rt)
{
    printf("%s\n", svm_grep_string);
    core_exit(_rt);
}

static void arg_error(const char *error)
{
    printf("svm: %s\n", error);
    exit(1);
}

static i32 consume_i32(i32 argc, char **argv, i32 pos)
{
    if (pos + 1 >= argc)
        return ARG_NO_ARGUMENT;

    i32 value = atoi(argv[pos+1]);
    if (value <= 0)
        return ARG_INVALID_VALUE;

    return value;
}

char *args_parse(SVMRuntime *_rt, i32 argc, char **argv) Nullable
{
    char *filename = NULL;

    for (i32 i = 1; i < argc; i++) {

        dprintf("Checking [%d] \"%s\"", i, argv[i]);

        if (arg(argv[i], "--help", "-h")) {
            help_page(_rt);
        }

        else if (arg(argv[i], "--version", "-v")) {
            version_page(_rt);
        }

        else if (arg(argv[i], "--decompile", "-D")) {
            _rt->arg_decompile = 1;
        }

        else if (arg(argv[i], "--mem-used", "-m")) {
            _rt->arg_mem_used = 1;
        }

        else if (arg(argv[i], "--allow-debug", "-d")) {
            _rt->arg_allow_debug = 1;
        }

        else if (arg(argv[i], "--limit-mem", "-l")) {
            i32 value = consume_i32(argc, argv, i);
            if (value == ARG_NO_ARGUMENT)
                arg_error("limit-mem requires a int value");
            if (value == ARG_INVALID_VALUE)
                arg_error("invalid argument to limit-mem");

            if (value > 2097151)
                arg_error("value of limit-mem is too high");

            _rt->arg_limit_mem = value * 1024;
            i++;
        }

        else {
            filename = argv[i];
        }
    
    }
    return filename;
}


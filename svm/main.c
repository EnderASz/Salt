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
 * start from the first line. The function commenting style is similar to the
 * Linux Kernel style, although it's a bit modified.
 *
 * Coding rules
 *
 * Functions should be short and do only one thing very well. If your function
 * is longer than 50 lines, you should think about splitting it - but it's not
 * required. Code should be clear and easy to read, use whitespace god damn it!
 * Function names are constructed using the topic as the first word, and then
 * the class & verb. Use `core_unit_clear` instead of `core_clear_unit`. The
 * max line width is 80 characters, and there are no excuses.
 *
 * Keep your code english: write proper english comments in a simple style so
 * others can understand you. 
 *
 * Versions 
 *
 * Every new set of functionality or bugfixes should get a new `minor` version,
 * and every major release should get a new `major` version. Simple as that.
 *
 * @version  Salt Virtual Machine; format 3 version 0.12  
 */
#include <svm/svm.h>
#include <svm/args.h>
#include <svm/decompiler.h>
#include <svm/exec.h>
#include <svm/loader.h>
#include <svm/module.h>

#include <stdio.h>
#include <stdlib.h>

/* If we're in linux, add support for signalling. */
#ifdef __linux__
#include <signal.h>
#include <unistd.h>
#endif

#ifdef DEBUG
#define SVM_GREP_STRING_DEBUG_FLAG " [DEBUG]"
#else
#define SVM_GREP_STRING_DEBUG_FLAG ""
#endif


/* This string will show up in the compiled version of SVM which you can then
   grep to, checking the format. */

const char *svm_grep_string = "SVM: f3 "SVM_VERSION" on "__TIMESTAMP__
    " ("SVM_STRINGIFY(SVM_TARGET_ARCH)" bit for "SVM_TARGET_SYSTEM")"
    SVM_GREP_STRING_DEBUG_FLAG;

static void size_check();
static void interrupt_handler(i32 _sig);


i32 main(i32 argc, char **argv)
{
    dprintf("Starting %s", SVM_VERSION);

    SVMRuntime runtime = {
        
        .registers = NULL,
        .register_size = 0,

        .flag_comparison = 0,

        .arg_allow_debug = 0,
        .arg_limit_mem = 0,
        .arg_decompile = 0,
        .arg_mem_used = 0,

        .m_used = 0,
        .m_max_used = 0,
        .m_allocations = 0,
        .m_frees = 0,

        .module_size = 0,
        .modules = NULL,

        .callstack_size = 0,
        .callstack = NULL

    };

    size_check();
    char *filename = args_parse(&runtime, argc, argv);

#ifdef __linux__
    signal(SIGINT, interrupt_handler);
#endif

    if (filename == NULL) {
        printf("Please provide a filename. See \"--help\" for more\n");
        goto end;
    }

    /* If the user chooses to decompile the program instead of running it, 
       don't load the main module and run decompile(). */

    if (runtime.arg_decompile) {
        decompile(&runtime, filename);
        core_exit(&runtime);
    }

    /* End of the preparation stage, load the main file and start executing it. 
       The runtime variables have to be loaded before this, in order to pass it
       to the program. */

    struct SaltModule *main = load(&runtime, filename);
    strcpy(main->name, "__main__");

    exec(&runtime, main, "%__load");
    exec(&runtime, main, "main");

    if (runtime.arg_mem_used)
        printf("Memory used: %lu\n", runtime.m_max_used);

    core_exit(&runtime);
end:
    return 0;
}

static void size_check()
{
    dprintf("sizeof(SVMRuntime) = %ld", sizeof(SVMRuntime));
    dprintf("sizeof(SaltObject) = %ld", sizeof(SaltObject));
    dprintf("sizeof(SaltModule) = %ld", sizeof(struct SaltModule));
    dprintf("sizeof(SaltObjectNode) = %ld", sizeof(struct SaltObjectNode));
    dprintf("sizeof(StackFrame) = %ld", sizeof(struct StackFrame));
    dprintf("sizeof(String) = %ld", sizeof(String));
    dprintf("sizeof(SCC3_Header) = %ld", sizeof(struct SCC3_Header)); 
}

static void interrupt_handler(i32 _sig)
{
#ifdef __linux__

    if (_sig != SIGINT)
        return;
    printf("Recieved interrupt\n");
    exit(1);

#endif
}

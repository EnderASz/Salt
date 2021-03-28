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
 *
 *
 *
 *
 * @version  Salt Virtual Machine; format 3 version 0.9  
 */
#include "include/args.h"
#include "include/core.h"
#include "include/module.h"
#include "include/callstack.h"
#include "include/object.h"
#include "include/loader.h"
#include "include/exec.h"

#include <stdio.h>
#include <string.h>

/* This string will show up in the compiled version of SVM which you can then
 grep to, checking the format. */
const char *svm_grep_string = "SVM: f3 "SVM_VERSION" on "__TIMESTAMP__" ("
            STRINGIFY(TARGET_ARCH)" bit for "TARGET_SYSTEM")";

static void size_check();

int main(int argc, char **argv)
{
    dprintf("Starting %s\n", SVM_VERSION);
    char *filename = args_parse(argc, argv);

    size_check();

    if (filename == NULL) {
        printf("Please provide a filename. See \"--help\" for more\n");
        goto end;
    }

    struct SaltModule *main = load(filename);
    strcpy(main->name, "__main__");

    exec(main);

    core_exit();

end:
    return 0;
}

static void size_check()
{
    dprintf("sizeof(SaltObject) = %ld\n", sizeof(SaltObject));
    dprintf("sizeof(SaltModule) = %ld\n", sizeof(struct SaltModule));
    dprintf("sizeof(SaltInstruction) = %ld\n", sizeof(struct SaltInstruction));
    dprintf("sizeof(SaltObjectNode) = %ld\n", sizeof(struct SaltObjectNode));
    dprintf("sizeof(StackFrame) = %ld\n", sizeof(struct StackFrame));
}

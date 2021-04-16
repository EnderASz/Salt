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
 * exception.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/exception.h"
#include "../include/callstack.h"
#include "../include/module.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


void exception_throw(SVMRuntime *_rt, const char *exception, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    dprintf("\033[91mThrowing exception: %s\033[0m", exception);
    fprintf(stderr, "An exception occured during execution: %s\n", exception);

    dprintf("Deconstructing %ld elements from the stack", _rt->callstack_size);
    u64 size = _rt->callstack_size;

    for (u64 i = 0; i < size; i++) {
        struct StackFrame *frame = callstack_peek(_rt);
        fprintf(stderr, "  at %s.%s\n", frame->module->name,
                frame->function);
        callstack_pop(_rt);
    }

    fprintf(stderr, "\n ~ ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    core_exit(_rt);
}


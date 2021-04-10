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
 * callstack.h implementation
 *
 * @author  bellrise, 2021
 */
#include "../include/callstack.h"
#include "../include/exception.h"
#include "../include/core.h"
#include "../include/module.h"

#include <string.h>
#include <stdio.h>

void callstack_push(SVMRuntime *_rt, u32 line, struct SaltModule *module,
                    char *__restrict function)
{
    dprintf("Pushing stack frame [%ld](%d, %s, %s)\n", _rt->callstack_size, 
            line, module->name, function);

#ifdef DEBUG_CALLSTACK
    printf("Confirm callstack push ");
    fgetc(stdin);
#endif

    _rt->callstack = vmrealloc(
        _rt->callstack, 
        sizeof(struct StackFrame) * _rt->callstack_size,
        sizeof(struct StackFrame) * (_rt->callstack_size + 1)
    );
    
    _rt->callstack[_rt->callstack_size].module = module;
    strncpy(_rt->callstack[_rt->callstack_size].function, function, 64);
    _rt->callstack[_rt->callstack_size].line = line;

    _rt->callstack_size++;

    if (_rt->callstack_size > 8192) {
        printf("fatal: Callstack is way too big, try using loops!\n");
        core_exit(_rt);
    }
}

struct StackFrame *callstack_peek(SVMRuntime *_rt) Nullable
{
    dprintf("Peeking at [%ld]\n", _rt->callstack_size - 1);
    if (_rt->callstack_size == 0)
        return NULL;
    return &_rt->callstack[_rt->callstack_size - 1];
}

void callstack_pop(SVMRuntime *_rt)
{
    dprintf("Popping %ld -> %ld\n", _rt->callstack_size, 
            _rt->callstack_size - 1);

    if (_rt->callstack_size == 1) {
        vmfree(_rt->callstack, sizeof(struct StackFrame));
        _rt->callstack = NULL;
        _rt->callstack_size = 0;
        return;
    }

    _rt->callstack = vmrealloc(
        _rt->callstack, 
        sizeof(struct StackFrame) * _rt->callstack_size,
        sizeof(struct StackFrame) * (_rt->callstack_size - 1)
    );
    _rt->callstack_size--;
}

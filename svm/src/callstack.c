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
 * callstack.h implementation
 *
 * @author  bellrise, 2021
 */
#include <svm/svm.h>
#include <svm/callstack.h>

#include <string.h>
#include <stdio.h>


void callstack_push(SVMRuntime *_rt, u32 line, struct SaltModule *module,
                    char *__restrict function)
{
    dprintf("Pushing stack frame [%ld](%d, %s, %s)", _rt->callstack_size,
            line, module->name, function);

#ifdef DEBUG_CALLSTACK
    /* We want to give an option of "stepping" through the function calls if 
       some recursion bug has to be fixed. */
    
    printf("Confirm callstack push ");
    fgetc(stdin);
#endif

    _rt->callstack = vmrealloc(
        _rt->callstack, 
        sizeof(struct StackFrame) * _rt->callstack_size,
        sizeof(struct StackFrame) * (_rt->callstack_size + 1)
    );
    
    _rt->callstack[_rt->callstack_size].module_ = module;
    strncpy(_rt->callstack[_rt->callstack_size].function, function, 63);
    _rt->callstack[_rt->callstack_size].line = line;

    _rt->callstack_size++;

    if (_rt->callstack_size > 8192) {
        printf("fatal: Callstack is way too big, try using loops!\n");
        core_exit(_rt);
    }
}

struct StackFrame *callstack_peek(SVMRuntime *_rt) Nullable
{
    dprintf("Peeking at [%ld]", _rt->callstack_size - 1);
    if (_rt->callstack_size == 0)
        return NULL;
    return &_rt->callstack[_rt->callstack_size - 1];
}

void callstack_pop(SVMRuntime *_rt)
{
    dprintf("Popping %ld -> %ld", _rt->callstack_size,
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


/**
 * callstack.h implementation
 *
 * @author  bellrise, 2021
 */
#include "../include/callstack.h"
#include "../include/exception.h"
#include "../include/core.h"

#include <string.h>

void callstack_push(SVMRuntime *_rt, u32 line, char *__restrict module, 
                    char *__restrict function)
{
    dprintf("Pushing stack frame [%ld](%d, %s, %s)\n", _rt->callstack_size, 
            line, module, function);

    if (strlen(module) > 61 || strlen(function) > 61) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Stack frame name too long");
    }

    _rt->callstack = vmrealloc(
        _rt->callstack, 
        sizeof(struct StackFrame) * _rt->callstack_size,
        sizeof(struct StackFrame) * (_rt->callstack_size + 1)
    );

    strcpy(_rt->callstack[_rt->callstack_size].module, module);
    strcpy(_rt->callstack[_rt->callstack_size].function, function);
    _rt->callstack[_rt->callstack_size].line = line;

    _rt->callstack_size++;
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

/**
 * exception.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
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

    dprintf("\033[91mThrowing exception: %s\033[0m\n", exception);
    fprintf(stderr, "An exception occured during execution: %s\n", exception);

    dprintf("Deconstructing %ld elements from the stack\n", _rt->callstack_size);
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

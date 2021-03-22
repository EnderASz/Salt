//
// exception.h implementation
//
#include "../include/exception.h"
#include "../include/core.h"
#include "../include/callstack.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void exception_throw(const char *exception, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    dprintf("\033[91mThrowing exception: %s\033[0m\n", exception);
    fprintf(stderr, "An exception occured during execution: %s\n", exception);

    dprintf("Deconstructing %d elements from the stack\n", callstack_size());
    int size = callstack_size();
    for (uint i = 0; i < size; i++) {
        struct StackFrame *frame = callstack_peek();
        fprintf(stderr, "  at %s.%s\n", frame->module,
                frame->function);
        callstack_pop();
    }

    fprintf(stderr, "\n ~ ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");

    va_end(args);
    core_exit();
}
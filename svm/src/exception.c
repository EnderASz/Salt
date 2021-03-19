//
// exception.h implementation
//
#include "../include/exception.h"
#include "../include/core.h"
#include "../include/callstack.h"
#include <stdio.h>
#include <stdlib.h>

void exception_throw(const char *exception, const char *msg)
{
    fprintf(stderr, "An exception occured during execution: %s\n", exception);

    dprintf("Deconstructing %d elements from the stack\n", callstack_size());
    int size = callstack_size();
    for (uint i = 0; i < size; i++) {
        struct StackFrame *frame = callstack_peek();
        fprintf(stderr, "  at: %s.%s\n", frame->module,
                frame->function);
        callstack_pop();
    }

    fprintf(stderr, "\n~ %s\n", msg);

    core_exit();
}
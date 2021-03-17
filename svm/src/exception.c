//
// exception.h implementation
//
#include "../include/exception.h"
#include "../include/core.h"
#include <stdio.h>
#include <stdlib.h>

void exception_throw(const char *exception, const char *msg)
{
    fprintf(stderr, "An exception occured during execution: %s\n", exception);
    fprintf(stderr, "  at: __main__\n"); // callstack
    fprintf(stderr, " ~ %s\n", msg);
}
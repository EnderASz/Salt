//
// Created by bellrise on 19.03.2021.
//
#include "../include/callstack.h"
#include "../include/exception.h"
#include <string.h>

static struct StackFrame *g_callstack = NULL;
static int g_callstack_size = 0;

void callstack_push(uint line, char *module, char *function)
{
    dprintf("Pushing stack frame [%d](%d, %s, %s)\n", g_callstack_size, line,
            module, function);

    if (strlen(module) > 61 || strlen(function) > 61) {
        exception_throw(EXCEPTION_RUNTIME, "Stack frame name too long");
    }

    g_callstack = vmrealloc(g_callstack, sizeof(struct StackFrame) * g_callstack_size,
                  sizeof(struct StackFrame) * (g_callstack_size + 1));

    strcpy(g_callstack[g_callstack_size].module, module);
    strcpy(g_callstack[g_callstack_size].function, function);
    g_callstack[g_callstack_size].line = line;

    g_callstack_size++;
}

int callstack_size()
{
    return g_callstack_size;
}

struct StackFrame __NULLABLE *callstack_peek()
{
    dprintf("Peeking at [%d]\n", g_callstack_size - 1);
    if (g_callstack_size - 1 < 0)
        return NULL;
    return &g_callstack[g_callstack_size - 1];
}

void callstack_pop()
{
    dprintf("Popping [%d]\n", g_callstack_size - 1);
    // I'm sorry that I need to sin here, I know this is copying over a lot of
    // memory but realloc keeps crashing on me!
    if (g_callstack_size == 1) {
        vmfree(g_callstack, sizeof(struct StackFrame));
        g_callstack_size = 0;
        return;
    }

    struct StackFrame *new_stack = vmalloc(sizeof(struct StackFrame) * (g_callstack_size - 1));
    memcpy(new_stack, g_callstack, sizeof(struct StackFrame) * (g_callstack_size - 1));
    vmfree(g_callstack, sizeof(struct StackFrame) * g_callstack_size);
    g_callstack = new_stack;
    g_callstack_size--;
}


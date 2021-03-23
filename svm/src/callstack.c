/**
 * callstack.h implementation
 *
 * @author  bellrise, 2021
 */
#include "../include/callstack.h"
#include "../include/exception.h"
#include "../include/core.h"

#include <string.h>
#include <stdint.h>


static struct StackFrame *g_callstack = NULL;
static uint64_t g_callstack_size = 0;

void callstack_push(uint32_t line, char *__restrict module, 
                    char *__restrict function)
{
    dprintf("Pushing stack frame [%ld](%d, %s, %s)\n", g_callstack_size, 
            line, module, function);

    if (strlen(module) > 61 || strlen(function) > 61) {
        exception_throw(EXCEPTION_RUNTIME, "Stack frame name too long");
    }

    g_callstack = vmrealloc(
        g_callstack, 
        sizeof(struct StackFrame) * g_callstack_size,
        sizeof(struct StackFrame) * (g_callstack_size + 1)
    );

    strcpy(g_callstack[g_callstack_size].module, module);
    strcpy(g_callstack[g_callstack_size].function, function);
    g_callstack[g_callstack_size].line = line;

    g_callstack_size++;
}

uint64_t callstack_size()
{
    return g_callstack_size;
}

Nullable struct StackFrame *callstack_peek()
{
    dprintf("Peeking at [%ld]\n", g_callstack_size - 1);
    if (g_callstack_size == 0)
        return NULL;
    return &g_callstack[g_callstack_size - 1];
}

void callstack_pop()
{
    dprintf("Popping [%ld]\n", g_callstack_size - 1);
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

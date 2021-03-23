/**
 * core.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/module.h"
#include "../include/object.h"
#include "../include/exception.h"
#include "../include/exec.h"

#include <stdlib.h>

#ifndef DEBUG_ALLOCATIONS
#define DEBUG_ALLOCATIONS 1
#endif

static uint64_t g_memory_used = 0;
static uint64_t g_max_used    = 0;
static uint64_t g_allocations = 0;
static uint64_t g_frees       = 0;

void core_exit()
{
    register_clear();
    module_delete_all();
    vibe_check();
    exit(0);
}

void *vmalloc(uint size)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mAllocating \033[32m%d\033[90m bytes\033[0m\n", size);
#endif
    g_allocations++;

    g_memory_used += size;
    if (g_memory_used > g_max_used)
        g_max_used = g_memory_used;

    void *ptr = malloc(size);
    if (!ptr)
        exception_throw(EXCEPTION_RUNTIME, "Failed to allocate memory");
    return malloc(size);
}

void vmfree(void *ptr, uint size)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mFreeing \033[33m%d\033[90m bytes\033[0m\n", size);
#endif
    g_frees++;

    g_memory_used -= size;
    free(ptr);
}

void *vmrealloc(void *ptr, uint before, uint after)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mReallocating \033[33m%d\033[90m to \033[32m%d\033[90m"
            " bytes\033[0m\n", before, after);
#endif
    g_allocations++;

    g_memory_used -= before;
    g_memory_used += after;
    if (g_memory_used > g_max_used)
        g_max_used = g_memory_used;

    void *pointer = realloc(ptr, (unsigned long) after);
    if (!pointer)
        exception_throw(EXCEPTION_RUNTIME, "Failed to allocate memory");
    return pointer;
}

uint64_t vmused()
{
    return g_memory_used;
}

void vibe_check()
{
    if (g_memory_used == 0) {
        dprintf("heap memory clean (%ld, %ld, %ld)\n", g_max_used,
                g_allocations, g_frees);
        return;
    }
    dprintf("\033[91m( %ld ) bytes are not free'd yet!\033[0m\n", g_memory_used);
}
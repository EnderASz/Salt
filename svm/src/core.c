/**
 * core.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/module.h"
#include "../include/exception.h"
#include "../include/exec.h"
#include "../include/callstack.h"

#include <stdlib.h>

#ifndef DEBUG_ALLOCATIONS
#define DEBUG_ALLOCATIONS 1
#endif

void core_exit(SVMRuntime *_rt)
{
    // Clear callstack
    for (uint i = 0; i < _rt->callstack_size; i++)
        callstack_pop(_rt);

    register_clear();
    module_delete_all(_rt);
    vibe_check(_rt);
    exit(0);
}

void *vmalloc(SVMRuntime *_rt, uint size)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mAllocating \033[32m%d\033[90m bytes\033[0m\n", size);
#endif
    _rt->m_allocations++;

    _rt->m_used += size;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_max_used;

    void *ptr = malloc(size);
    if (!ptr)
        exception_throw(_rt, EXCEPTION_RUNTIME, "Failed to allocate memory");
    return malloc(size);
}

void vmfree(SVMRuntime *_rt, void *ptr, uint size)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mFreeing \033[33m%d\033[90m bytes\033[0m\n", size);
#endif
    _rt->m_frees++;

    _rt->m_used -= size;
    free(ptr);
}

void *vmrealloc(SVMRuntime *_rt, void *ptr, uint before, uint after)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mReallocating \033[33m%d\033[90m to \033[32m%d\033[90m"
            " bytes\033[0m\n", before, after);
#endif
    _rt->m_allocations++;

    _rt->m_used -= before;
    _rt->m_used += after;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_used;

    void *pointer = realloc(ptr, (unsigned long) after);
    if (!pointer)
        exception_throw(_rt, EXCEPTION_RUNTIME, "Failed to allocate memory");
    return pointer;
}

void vibe_check(SVMRuntime *_rt)
{
    if (_rt->m_used == 0) {
        dprintf("heap memory clean (%ld, %ld, %ld)\n", _rt->m_max_used,
                _rt->m_allocations, _rt->m_frees);
        return;
    }
    dprintf("\033[91m( %ld ) bytes are not free'd yet!\033[0m\n", _rt->m_used);
}


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
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 *
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

inline u8 bit_at(u8 byte, u8 bit)
{
    return byte & (1 << (8 - bit)) ? 1 : 0;
}

inline void bit_set(u8 *byte, u8 bit)
{
    *byte |= 1 << (8 - bit);
}

inline void bit_unset(u8 *byte, u8 bit)
{
    *byte &= ~(1 << (8 - bit));
}

void core_exit(SVMRuntime *_rt)
{
    dprintf("Calling cleanup\n");
    
    // Clear callstack
    for (u64 i = 0; i < _rt->callstack_size; i++)
        callstack_pop(_rt);

    /* For some reason this *reallY* likes to break, so just check if the
     whole callstack has been cleared. */
    if (_rt->callstack_size == 1)
        callstack_pop(_rt);

    register_clear(_rt);
    module_delete_all(_rt);
    vibe_check(_rt);
    exit(0);
}

void *_vmalloc(SVMRuntime *_rt, u32 size, const char *func)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mAllocating \033[32m%d\033[90m bytes in %s\033[0m\n", 
            size, func);
#endif
    _rt->m_allocations++;

    _rt->m_used += size;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_max_used;

    void *ptr = malloc(size);
    if (!ptr) {
        /* Unforunately I can't call the cleanup functions from here because
         this has to be SVM runtime independent, i'm putting my memory in the
         hands of the OS. */
        printf("Failed to allocate memory\n");
        exit(1);
    }    
    return malloc(size);
}

void _vmfree(SVMRuntime *_rt, void *ptr, u32 size, const char *func)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mFreeing \033[33m%d\033[90m bytes in %s\033[0m\n", 
            size, func);
#endif
    _rt->m_frees++;

    _rt->m_used -= size;
    free(ptr);
}

void *_vmrealloc(SVMRuntime *_rt, void *ptr, u32 before, u32 after, 
                 const char *func)
{
#ifdef DEBUG_ALLOCATIONS
    dprintf("\033[90mReallocating \033[33m%d\033[90m to \033[32m%d\033[90m"
            " bytes in %s\033[0m\n", before, after, func);
#endif
    _rt->m_allocations++;

    _rt->m_used -= before;
    _rt->m_used += after;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_used;

    void *pointer = realloc(ptr, (unsigned long) after);
    if (!pointer) {
        printf("Failed to reallocate memory\n");
        exit(1);
    }
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


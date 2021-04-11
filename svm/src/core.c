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
 * svm.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/module.h"
#include "../include/exception.h"
#include "../include/exec.h"
#include "../include/callstack.h"

#include <stdlib.h>
#include <stdarg.h>

#if defined(_WIN32) && defined(DEBUG)
/* If we're compiling for _WIN32 (debug mode only), include the Windows header
   in order to call SetConsoleTextAttribute to make terminal colours. */
#include <windows.h>
#endif

#ifndef DEBUG_ALLOCATIONS
#define DEBUG_ALLOCATIONS 1
#endif

void core_exit(SVMRuntime *_rt)
{
    dprintf("Calling cleanup");
    
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

void _linux_dprintf(char *file, const char *func, const char *fmt, ...)
{
#ifdef DEBUG
    /* We only want this function to compile if DEBUG is defined. Otherwise,
       just keep this empty. The ununsed parameter checks are disabled by
       default because of the exec_ functions. */

    va_list args;
    va_start(args, fmt);

    u32 pos = 0;
    for (u32 i = 0; i < strlen(file); i++) {
        if (file[i] == '/')
            pos = i + 1;
    }

    i32 file_size = (i32) strlen(file);
    i32 func_size = (i32) strlen(func);

    char *path[file_size + func_size];
    memset((char *) path, 0, file_size + func_size);

    strncpy((char *) path, file + pos, file_size - 2 - pos);

    printf("\033[96m%*s\033[0m \033[90m: \033[92m%s%*c\033[90m|\033[0m ",
            12, (char *) path,
            (char *) func, 20 - func_size, ' '
    );

    vprintf(fmt, args);
    printf("\n");

    va_end(args);
#endif
}

void _win_dprintf(char *file, const char *func, const char *fmt, ...)
{
#if defined(DEBUG) && defined(_WIN32)
    /* Again, we want to compile this only if we're building a debug
       version. Also, thank god I don't have to deal with this shit from
       Windows. */

    va_list args;
    va_start(args, fmt);

    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (3 & 0x0F));
    printf("%s", file);
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (1 & 0x0F));
    printf("::");
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (2 & 0x0F));
    printf("%s: ", func);
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (15 & 0x0F));
    vprintf(fmt, args);
    printf("\n");

    va_end(args);
#endif
}

static inline void check_memory_limit(SVMRuntime *_rt)
{
    if (_rt->arg_limit_mem && (u64) _rt->arg_limit_mem <= _rt->m_max_used) {
        printf("Out of memory\n");
        core_exit(_rt);
    }
}

void *_vmalloc(SVMRuntime *_rt, u32 size, const char *func)
{
#if DEBUG_ALLOCATIONS == 1
    dprintf("\033[90mAllocating \033[92m%d\033[90m bytes in %s\033[0m",
            size, func);
#endif
    _rt->m_allocations++;

    _rt->m_used += size;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_max_used;

    check_memory_limit(_rt);

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
#if DEBUG_ALLOCATIONS == 1
    dprintf("\033[90mFreeing \033[93m%d\033[90m bytes in %s\033[0m",
            size, func);
#endif
    _rt->m_frees++;

    _rt->m_used -= size;
    free(ptr);
}

void *_vmrealloc(SVMRuntime *_rt, void *ptr, u32 before, u32 after,
                 const char *func)
{
#if DEBUG_ALLOCATIONS == 1
    dprintf("\033[90mReallocating \033[93m%d\033[90m to \033[92m%d\033[90m"
            " bytes in %s\033[0m", before, after, func);
#endif
    _rt->m_allocations++;

    _rt->m_used -= before;
    _rt->m_used += after;
    if (_rt->m_used > _rt->m_max_used)
        _rt->m_max_used = _rt->m_used;

    /* If we're dropping memory, do not check for the limit again. */
    if (before <= after)
        check_memory_limit(_rt);

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
        dprintf("heap memory clean (%ld, %ld, %ld)", _rt->m_max_used,
                _rt->m_allocations, _rt->m_frees);
        return;
    }
    dprintf("\033[91m( %ld ) bytes are not free'd yet!\033[0m", _rt->m_used);
}


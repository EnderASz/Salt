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
 * The core module provides a couple of type definitions and core functions
 * for the whole virtual machine. This defines a couple of important symbols
 * like Nullable or just the interface SVM_VERSION, which is defined by the
 * compiler anyway.
 *
 * @author bellrise, 2021
 */

/* The Nullable symbol is placed before function declarations and implemen-
 -tations to signify that it may return NULL. */
#ifndef Nullable
#define Nullable /* may return null */
#endif

/* The SVM_VERSION defined here is just to not upset the auto complete and 
 static analasys. The actual value of SVM_VERSION is passed on the command
 line when compiling it. */
#ifndef SVM_VERSION
#define SVM_VERSION "(unspecified by the compiler)"
#endif

/* Header guard */
#ifndef SVM_CORE_H
#define SVM_CORE_H

#include "object.h"

#include <stdio.h>
#include <stdint.h>
#include <string.h>

/* Compilation settings. _ARCH is defined to the bit settings. */
#if __INTPTR_MAX__ == __INT32_MAX__
#define ARCHITECTURE 32
#elif __INTPTR_MAX__ == __INT64_MAX__
#define ARCHITECTURE 64
#else
#define ARCHITECTURE 0
#endif

/* Wrap the given value in quotes. This is needed for svm_grep_string where the
 const string is created from concatinating several values. */
#define ISTRINGIFY(X)  #X
#define STRINGIFY(NUM) ISTRINGIFY(NUM)

/* dprintf implementations */
#ifdef DEBUG

#ifdef _WIN32
#include <windows.h>

/* Bruh, programming on windows sucks! This looks actually terrible but I was 
 told this had to be done to support terminal colouring on the original 32bit
 windows cmd. - bellrise */
#define dprintf(...)                                                \
{                                                                   \
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);                  \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (3 & 0x0F));  \
    printf("%s", __FILE__);                                         \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (1 & 0x0F));  \
    printf("::");                                                   \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (2 & 0x0F));  \
    printf("%s: ", __func__);                                       \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (15 & 0x0F)); \
    printf(__VA_ARGS__);                                            \
}

#elif defined(__linux__)

/* The simple Linux version which just uses ANSI escape codes. */
#define dprintf(...)                                  \
{                                                     \
    printf("\033[96m%s\033[34m::\033[92m%s: \033[0m", \
    __FILE__, __func__);                              \
    printf(__VA_ARGS__);                              \
}

#endif // __linux__
#else // DEBUG

/* If DEBUG is not defined, define dprintf as an empty function. */
#define dprintf(...)

#endif // DEBUG

typedef __UINT8_TYPE__  byte;
typedef __UINT32_TYPE__ uint;

/**
 * This stores a single Salt instruction that can be executed. 
 *
 * @a name     name of the instruction, the 5 chars are compared to get the
 *             correct exec.
 * @a size     size of the string array (null included)
 * @a content  the pointer to a heap char array storing the instruction
 */
struct SaltInstruction {

    char  name[6];
    uint  size;
    char *content;

};

/**
 * Always call this instead of the normal exit, because this function
 * additionally cleans up the heap allocated memory before calling exit
 * from stdlib.
 */
void core_exit();

/**
 * Allocate n bytes in the heap, registering the memory usage in the global
 * MEMORY_USED variable.
 *
 * @param   size  amount of bytes to allocate
 */
void *vmalloc(uint size);

/**
 * Free n bytes from the heap at the given pointer. Removes [size] bytes from
 * the MEMORY_USED variable.
 *
 * @param   ptr  pointer to memory
 * @param   size amount of bytes to deallocate
 */
void vmfree(void *ptr, uint size);

/**
 * Reallocate the given piece of memory to another location.
 *
 * @param   ptr     pointer to initial memory
 * @param   before  amount of memory used before
 * @param   after   amount of memory used after
 * @return  pointer to memory
 */
void *vmrealloc(void *ptr, uint before, uint after);

/**
 * Check the current used heap memory.
 *
 * @return g_memory_used
 */
uint64_t vmused();

/**
 * Check the current status of the memory.
 */
void vibe_check();

#endif // SVM_CORE_H

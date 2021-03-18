/**
 * The core module provides a couple of type definitions and core functions
 * for the whole virtual machine.
 *
 */
#ifndef SVM_CORE_H
#define SVM_CORE_H

#include "core.h"
#include "object.h"
#include "module.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef _WIN32
#define _ARCH 32
#elif defined(_WIN64) || defined(__linux__)
#define _ARCH 64
#endif

/* dprintf implementations */
#ifdef DEBUG

#if defined(_WIN32)

#include <windows.h>
#define dprintf(...)                                                \
{                                                                   \
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);                  \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (3 & 0x0F));  \
    printf("%s", __FILE__);                                         \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (1 & 0x0F));  \
    printf("::");                                                   \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (2 & 0x0F));  \
    printf("%s: ", __FUNCTION__);                                   \
    SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (15 & 0x0F)); \
    printf(__VA_ARGS__);                                            \
}
#elif defined(__linux__)

#define dprintf(...)                                  \
{                                                     \
    printf("\033[96m%s\033[34m::\033[92m%s: \033[0m", \
    __FILE__, __FUNCTION__);                          \
    printf(__VA_ARGS__);                              \
}

#endif

#else
#define dprintf(...)
#endif
/* end of dprintf implementations */

// Types

typedef unsigned int  uint;
typedef unsigned char byte;

/* This stores a single salt instruction that can be executed */
struct SaltInstruction {

    char name[6];
    uint size;
    char *content;

};

// Globals

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
 * @param size  amount of bytes to allocate
 */
void *vmalloc(uint size);

/**
 * Free n bytes from the heap at the given pointer. Removes [size] bytes from
 * the MEMORY_USED variable.
 *
 * @param ptr  pointer to memory
 * @param size amount of bytes to deallocate
 */
void vmfree(void *ptr, uint size);

/**
 * Reallocate the given piece of memory to another location.
 *
 * @param   ptr     pointer to initial memory
 * @param   before  amount of memory used before
 * @param   after   amount of memory used after
 * @return pointer to memory
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

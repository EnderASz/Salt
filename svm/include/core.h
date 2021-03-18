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

#ifdef _WIN32
#define _ARCH 32
#elif defined(_WIN64) || defined(__linux__)
#define _ARCH 64
#endif

#ifdef DEBUG
#define dprintf(...)                                \
{                                                   \
        printf("\033[96m%s: \033[0m", __FILE__);    \
        printf(__VA_ARGS__);                        \
}
#else
#define dprintf(...)
#endif

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

#endif // SVM_CORE_H

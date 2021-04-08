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

/* System settings, includes the core os headers for functionality. */
#ifndef TARGET_SYSTEM
#ifdef _WIN32
#define TARGET_SYSTEM "Windows"
#elif defined(__linux__)
#define TARGET_SYSTEM "Linux"
#else
/* Add your system here if you're porting to something else & have fun
 fixing all the system dependencies! There shouldn't be a lot of them,
 because I'm just using the standard C lib.  */

#error "There is no support for the system you're using"
#endif
#endif

/* Header guard */
#ifndef SVM_CORE_H
#define SVM_CORE_H

#include <stdio.h>
#include <string.h>

/* Compilation settings. TARGET_ARCH is defined to the target bits. */
#if __INTPTR_MAX__ == __INT32_MAX__
#define TARGET_ARCH 32
#elif __INTPTR_MAX__ == __INT64_MAX__
#define TARGET_ARCH 64
#else
#define TARGET_ARCH 0
#error "Other than 32/64 bit are not supported"
#endif


/* Wrap the given value in quotes. This is needed for svm_grep_string where the
 const string is created from concatinating several values. */
#define ISTRINGIFY(X)  #X
#define STRINGIFY(NUM) ISTRINGIFY(NUM)

/* dprintf implementations */
#ifdef DEBUG

#ifdef _WIN32

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

/* The notice me macro will print detailed information about the current
 location where it was called and make you notice the point where it 
 happened. */
#ifndef NOTICE_ME
#define NOTICE_ME(STR)                                                      \
{                                                                           \
    printf("\033[91mNOTICE ME!\n");                                         \
    printf("In %s in '%s' on line %d\n", __FILE__, __func__, __LINE__);     \
    printf("\n>>> %s\n\n\033[0m", STR);                                     \
}
#endif

#define READONLY_FALSE (0x00)
#define READONLY_TRUE  (0x01)

// Simple types

#define OBJECT_TYPE_NULL   (0x00)
#define OBJECT_TYPE_INT    (0x01)
#define OBJECT_TYPE_FLOAT  (0x02)
#define OBJECT_TYPE_BOOL   (0x03)
#define OBJECT_TYPE_STRING (0x04)

typedef __INT8_TYPE__       i8;
typedef __INT16_TYPE__      i16;
typedef __INT32_TYPE__      i32;
typedef __INT64_TYPE__      i64;

typedef __UINT8_TYPE__      u8;
typedef __UINT16_TYPE__     u16;
typedef __UINT32_TYPE__     u32;
typedef __UINT64_TYPE__     u64;

/**
 * This represents a single object, which can hold different types of data
 * depending on the [type] byte. When building a new object, default values are
 * assigned an the constructor & destructor pointers are passed, which take 
 * care of allocating the needed memory on the heap. Be sure to never build a 
 * raw object yourself, instead use salt_object_create(...). 
 *
 * @a id            ID of the object
 * @a readonly      0x01 if the object should be const 
 * @a type          type of the object, see OBJECT_TYPE_xxx
 * @a mutex_acquired if this is not 1, it means the object 
 * @a value         pointer to the value
 * @a size          amount of bytes allocated
 * @a vhandler      value handler method
 * @a destructor    object destructor
 */
typedef struct _salt_object_st {

     u32 id;
      u8 readonly;
      u8 type;
      u8 mutex_acquired;

      u8 _pad1[1];
 
     u32 size;
    void *value;

    /* the runtime pointers have to be set to a void * because the actual
     type is defined later on, being struct _svm_runtime_st. */
    void (* ctor) (void *_rt, struct _salt_object_st *self);
    void (* dtor) (void *_rt, struct _salt_object_st *self);
    void (*print) (void *_rt, struct _salt_object_st *self);

#if TARGET_ARCH == 32 
    u8 _pad2[12];
#endif

} SaltObject;


/**
 * The SVM Runtime container stores all the global runtime variables used
 * in the virtual machine. Only a single struct will most likely be created
 * in the main function.
 */
typedef struct _svm_runtime_st {

    /* Global register array. The amount of registers is defined in SCC 
     headers, and is handled upon loading every module. */
    SaltObject *registers;
    u8 register_size;

    /**
     * The runtime flags are single bits that are set and unset by different 
     * instructions. Here is a quick summary about what each flag does:
     *
     *      +---+---------------------------------+
     *      | 1 | System flag, restricted use     |
     *      +---+---------------------------------+
     *      | 2 | Comparison flag                 |  
     *      +---+---------------------------------+
     *      | 3 |                                 |
     *      +---+---------------------------------+
     *      | 4 |                                 |
     *      +---+---------------------------------+
     *      | 5 |                                 |
     *      +---+---------------------------------+
     *      | 6 |                                 |
     *      +---+---------------------------------+
     *      | 7 |                                 |
     *      +---+---------------------------------+
     *      | 8 |                                 |
     *      +---+---------------------------------+
     * 
     * Modifyings flags can be done using bit_set() and bit_unset(), and can
     * be checked using bit_at(). You should not do it by hand, because you may
     * access the incorrect flag. The second parameter is always the bit 
     * position, < which is counted from 1, not 0! >
     */
    u8 flags;

    /* Argument flags. */
    u8 arg_mem_used;

    /* Memory status variables. This keeps track of every allocation and the 
     amount of memory used. Used for checking for memory leaks. */
    u64 m_used;
    u64 m_max_used;
    u64 m_allocations;
    u64 m_frees;

    /* Global table of loaded modules. (module.h) */
    u32 module_size;
    struct SaltModule **modules;

    /* The callstack. (callstack.h) */
    u64 callstack_size;
    struct StackFrame *callstack;

} SVMRuntime;

/**
 * This stores a null terminated string
 *
 * @a size     size of the char array (null included)
 * @a content  pointer to a heap allocated string array
 */
typedef struct _string_st {

    u64 size;
    char *content;

} String;

// Bit operations

/**
 * Return the bit at the given value. 
 *
 * @param   byte    byte to check
 * @param   bit     location of bit to check 
 */
u8 bit_at(u8 byte, u8 bit);

/**
 * Set a bit to 1 at the given location.
 *
 * @param   byte    value
 * @param   bit     location of bit to set 
 */
void bit_set(u8 *byte, u8 bit);

/**
 * Set a bit to 1 at the given location.
 *
 * @param   byte    value
 * @param   bit     location of bit to unset 
 */
void bit_unset(u8 *byte, u8 bit);

/**
 * Always call this instead of the normal exit, because this function
 * additionally cleans up the heap allocated memory before calling exit
 * from stdlib.
 */
void core_exit(SVMRuntime *_rt);

/**
 * Allocate n bytes in the heap, registering the memory usage in the global
 * MEMORY_USED variable.
 *
 * @param   size  amount of bytes to allocate
 */
void *_vmalloc(SVMRuntime *_rt, u32 size, const char *func);

/**
 * Free n bytes from the heap at the given pointer. Removes [size] bytes from
 * the MEMORY_USED variable.
 *
 * @param   ptr  pointer to memory
 * @param   size amount of bytes to deallocate
 */
void _vmfree(SVMRuntime *_rt, void *ptr, u32 size, const char *func);

/**
 * Reallocate the given piece of memory to another location.
 *
 * @param   ptr     pointer to initial memory
 * @param   before  amount of memory used before
 * @param   after   amount of memory used after
 * @return  pointer to memory
 */
void *_vmrealloc(SVMRuntime *_rt, void *ptr, u32 before, u32 after, 
                 const char *func);

/* These macros wrap around the allocation functions passing the runtime and 
 function name automatically. */
#define vmalloc(N) _vmalloc(_rt, N, __func__)
#define vmfree(PTR, N) _vmfree(_rt, PTR, N, __func__)
#define vmrealloc(PTR, N, M) _vmrealloc(_rt, PTR, N, M, __func__)

/**
 * Check the current status of the memory.
 */
void vibe_check(SVMRuntime *_rt);

/**
 * Create a raw salt object. Assign all the needed values, settings the object
 * to a null type & value.
 *
 * @return brand new heap allocated salt object.
 */
SaltObject *salt_object_create(SVMRuntime *_rt);

/**
 * Deep copy a salt object.
 *
 * @param   dest  destination object
 * @param   src   source object
 */
void salt_object_copy(SVMRuntime *_rt, SaltObject *dest, SaltObject *src);

/**
 * Create a new salt object from the given payload. See doc/scc.html for
 * information about the payload.
 *
 * @param   payload  pointer to bytes
 * @return  brand new heap allocated SaltObject.
 */
void salt_object_define(SVMRuntime *_rt, SaltObject *obj, u8 *payload);

/**
 * Print the object without a newline or space after it.
 *
 * @param   obj  pointer to the object
 */
void salt_object_print(SVMRuntime *_rt, SaltObject *obj);

/**
 * Salt object constructor and destructor. These should be called when creating
 * or removing an object from memory.
 *
 * @param   self  the object instance
 */
void salt_object_ctor(void *_rt, SaltObject *self);
void salt_object_dtor(void *_rt, SaltObject *self);

#endif // SVM_CORE_H

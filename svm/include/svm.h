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
#ifndef SVM_CORE_H
#define SVM_CORE_H

#include <stdio.h>
#include <string.h>


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
#ifndef SVM_TARGET_SYSTEM
#if defined(_WIN32)
#define SVM_TARGET_SYSTEM "Windows"

#elif defined(__linux__)
#define SVM_TARGET_SYSTEM "Linux"

#else

/* Add your system here if you're porting to something else & have fun
   fixing all the system dependencies! There shouldn't be a lot of them,
   because I'm just using the standard C lib.  */

#error "There is no support for the system you're using"
#endif /* _WIN32 / __linux__ */
#endif /* TARGET_SYSTEM */


/* TARGET_ARCH is the amount of bits the program will be compiled on. I'm
   throwing an error if we're not using building for 32 or 64 bits, because
   I have no idea why that would happen! If you're building for 16 bits, it's
   probably and embeded system and you should know how to fix this yourself.
   Have fun! */

#if __INTPTR_MAX__ == __INT32_MAX__
#define SVM_TARGET_ARCH 32

#elif __INTPTR_MAX__ == __INT64_MAX__
#define SVM_TARGET_ARCH 64

#else
#define SVM_TARGET_ARCH 0
#error "Other than 32/64 bit are not supported"

#endif


/* Wrap the given value in quotes. This is needed for svm_grep_string where the
   const string is created by concatinating several values. */

#define SVM_STRINGIFY_INTERNAL(X)  #X
#define SVM_STRINGIFY(NUM) SVM_STRINGIFY_INTERNAL(NUM)

// ----------------------------------------------------------------------------
// DEBUG
// ----------------------------------------------------------------------------

/* This section will only be included if DEBUG is defined by the compiler. It
   contains the dprintf macro, which is basically a printf but for debug output.
   I'd probably add debug malloc & realloc wrappers if I was not using this
   amazing clang sanitizer. */

#ifdef DEBUG

#ifdef _WIN32
/* Use _win_dprintf for Windows, you can check the implementation in the source
   file, it looks kinda strage. Look, I'll be honest, programming in C kinda 
   sucks on Windows. I hope my co-worker doesn't lose his mind working on Windows 
   compatibility problems hehe. */
#define dprintf(...) _win_dprintf(__FILE__, __func__, __VA_ARGS__)

#elif defined(__linux__)
/* Use _linux_dprintf for Linux, you can check the implementation in the source
   file, it's much better than the Windows one for sure! */
#define dprintf(...) _linux_dprintf(__FILE__, __func__, __VA_ARGS__);

#endif
#else

/* If DEBUG is not defined, define dprintf as nothing, because we don't want
   to compile with useless code. */
#define dprintf(...)

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

#endif /* DEBUG */


// ----------------------------------------------------------------------------
// GLOBAL TYPES
// ----------------------------------------------------------------------------

/* Read-only markers. These u8 flags are stored in an object and they mark if
   the object is constant or not. */

#define READONLY_FALSE  (0x00)
#define READONLY_TRUE   (0x01)

/* Salt object types. */

#define SALT_TYPE_NULL      (0x00)
#define SALT_TYPE_INT       (0x01)
#define SALT_TYPE_FLOAT     (0x02)
#define SALT_TYPE_BOOL      (0x03)
#define SALT_TYPE_STRING    (0x04)
#define SALT_TYPE_MODEL     (0x05)

/* All the Rust programming language style integers are defined here, I use them
   because a) they look nice b) I can be sure about the size of the variable. */

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
 */
typedef struct _salt_object_st {

    /* ID of the object. The virutal machine does not control if the created
       object has a unique ID or not, it's up to the compiler to not break
       anything. */
    u32 id;

    /* Can be either READONLY_TRUE or READONLY_FALSE. If true, this object is
       const, and therefor the value cannot be modified. */
    u8  readonly;

    /* Salt object type. This can be one of SALT_TYPE_... */
    u8  type;

    /* Padding between the first values and the data. */
    u8 _pad1[2];

    /* The value the object stores, paired with the size for more solid memory
       control. And yes, strings cannot have a length > 4 bilion. */
    u32   size;
    void *value;

    /* The runtime pointers have to be set to a void * because the actual
       type is defined later on, being struct _svm_runtime_st. */
    void (* ctor) (void *_rt, struct _salt_object_st *self);
    void (* dtor) (void *_rt, struct _salt_object_st *self);
    void (*print) (void *_rt, struct _salt_object_st *self);

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

    /* Flags */
    u8 flag_comparison;

    /* Argument flags. These are set by arg_parse, and different code is
       run depending on these flags. */
     u8 arg_mem_used;
     u8 arg_allow_debug;
    i32 arg_limit_mem;

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

// ----------------------------------------------------------------------------
// VIRTUAL MACHINE ALLOCATION FUNCTIONS
// ----------------------------------------------------------------------------

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


// ----------------------------------------------------------------------------
// CORE SVM FUNCTIONS
// ----------------------------------------------------------------------------

/**
 * Always call this instead of the normal exit, because this function
 * additionally cleans up the heap allocated memory before calling exit
 * from stdlib.
 */
void core_exit(SVMRuntime *_rt);

/**
 * Linux-only debug printf. Prints very nice debug messages using the passed
 * format and variables. Note: This places a newline at the end.
 */
void _linux_dprintf(char *file, const char *func, const char *fmt, ...);

/**
 * Debug output for Windows.
 */
void _win_dprintf(char *file, const char *func, const char *fmt, ...);

/**
 * Check the current status of the memory.
 */
void vibe_check(SVMRuntime *_rt);


// ----------------------------------------------------------------------------
// SALT OBJECT FUNCTIONS
// ----------------------------------------------------------------------------

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

#endif /* SVM_CORE_H */


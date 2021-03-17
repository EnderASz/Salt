/**
 * The core module provides a couple of type definitions and core functions
 * for the whole virtual machine.
 *
 */
#ifndef SVM_CORE_H
#define SVM_CORE_H

#include <stdio.h>

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

#endif // SVM_CORE_H

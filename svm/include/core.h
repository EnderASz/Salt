/**
 * The core module provides a couple of type definitions and core functions
 * for the whole virtual machine.
 *
 */
#ifndef CORE_H
#define CORE_H

#include <stdio.h>

#ifdef DEBUG
#define dprintf(...) printf(__VA_ARGS__)
#else
#define dprintf(...)
#endif

// Types

typedef unsigned int  uint;
typedef unsigned char byte;


#endif // CORE_H

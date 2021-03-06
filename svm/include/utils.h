/**
 * This is a collection of different utility functions used most of the time
 * for debugging.
 *
 */
#ifndef UTILS_H_
#define UTILS_H_

#include "core.h"

/* The debug macro. Compiled the code found inside if SVM_DEBUG is defined.
 */
#ifdef SVM_DEBUG
# define DEBUG(__CODE__) __CODE__
#else
# define DEBUG(__CODE__)
#endif


/* Salt allocation function, which may be replaced later with some special 
 * functionality. This is used instead of the basic malloc or calloc.
 */
extern void* smalloc(size_t _size, uint _elem);

/* Print _n amount of bytes grouped into 2 bytes starting from the mptr_t byte
 * location. This is used for runtime memory debugging in the current given
 * point.
 */
extern void utils_hexdump(mptr_t byte, uint _n);

/* Print _n amount of bytes in ASCII form from the given memory pointer.
 * It only prints characters that are in the visual ASCII range, printing 
 * the rest as single spaces.
 */
static void utils_hexdump_c(mptr_t byte, uint _n);

#endif // UTILS_H_
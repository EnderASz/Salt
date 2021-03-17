/**
 * Utility library
 *
 * @author bellrise
 */
#ifndef SVM_UTILS_H
#define SVM_UTILS_H

#include "core.h"

/**
 * Print n amount of bytes in hex format. Used for debugging.
 *
 * @param   bytes   pointer to the bytes
 * @param   amount  amount of bytes to print
 */
void util_hexdump(byte *bytes, uint amount);

#endif // SVM_UTILS_H

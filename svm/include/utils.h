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
 * Utility library
 *
 * @author bellrise
 */
#ifndef SVM_UTILS_H
#define SVM_UTILS_H

#include "svm.h"

/**
 * Print n amount of bytes in hex format. Used for debugging.
 *
 * @param   bytes   pointer to the bytes
 * @param   amount  amount of bytes to print
 */
void util_hexdump(u8 *bytes, u32 amount);

/**
 * Print a single byte in the (hex bits) format. Used for debugging. 
 *
 * @param   byte    byte to print
 */
void util_bitdump(u8 byte);

#endif /* SVM_UTILS_H */


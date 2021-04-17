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
 * This header has all the types & constants the SCC file format provides.
 * Further versions of SCC will be supported when they become stable enough,
 * but first supported version is SCC3.
 */
#ifndef SVM_SCC_H
#define SVM_SCC_H

#include "svm.h"

/* SCC Header constants. */

#define SCC3_HEADER (* (u64 *) "\x7fSCC\xff\xee\0\0")
#define SCC3_VERSION  ((u64) 3)

/**
 * The SCC3 header consists of the magic number, the SCC format version,
 * the amount of instructions and the amount of used registers in this 
 * current module. It has a consistent 64 bytes.
 *
 * Additional padding bytes are added so we can cast a 64 byte string 
 * straight into the header structure. 
 */
struct SCC3_Header /* 64 bytes */ {
    
    /* Both the header and the version, both taking up 16 bytes must be present
       in all SCC formats. Without this consistency, reading any SCC header 
       would be a lot harder. Because we are sure the version will be located
       at the 8th byte, we can just compare the version located there. */
    u64 header;
    u64 version;

    /* Amount of instructions that will be present in this module. */
    u32 instructions;
    u8 _pad1[4];

    /* Amount of registers that will be used in this module. */
    u8 registers;
    u8 _pad2[7];

    u8 _pad3[24];

    /* Compiler signature */
    u32 signature_prefix;
    u32 signature;

};


#endif /* SVM_SCC_H */
 

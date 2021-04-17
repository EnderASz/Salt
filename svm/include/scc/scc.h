/**
 * Standalone header
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
 */
#ifndef SCC_H
#define SCC_H

/* README
 *
 * Because this a standalone header, it does not require any SVM headers and it
 * is not developed in any relation with the current SVM version. This is
 * designed so it can be included in both C++ and C code, providing different
 * ways of accessing the values, without a difference in implementations. Also,
 * this header does not have any header dependency (only a sane compiler).
 *
 * Each symbol defined in this header starts with the scc prefix (for C) or
 * is in the scc:: namespace (for C++).
 *
 * GUARANTEE: NO SYMBOL WILL BE CHANGED OR REMOVED FROM THIS HEADER, IT CAN
 * ONLY BECOME DEPRECATED.
 */

/* CONSTANT VALUES */

typedef __UINT8_TYPE__  _scc_u8_t;
typedef __UINT32_TYPE__ _scc_u32_t;
typedef __UINT64_TYPE__ _scc_u64_t;

#ifdef __cplusplus

/* Provide const values for C++ instead of macros. */

namespace scc
{
    /* SCC 3 */
    const _scc_u64_t SCC3_MAGIC = * (_scc_u64_t *) "\x7fSCC\xff\xee\0\0";
    const _scc_u64_t SCC3_VERSION = 3;
}

#else /* __cplusplus */

/* If we're using C, implement const values data using macros. */

#define SCC3_MAGIC (*(_scc_u64_t*) "\x7fSCC\xff\xee\0\0")
#define SCC3_VERSION ((_scc_u64_t) 3 )

#endif /* __cplusplus */


/* STRUCTURES */

/* Wrap the structures inside the scc:: namespace if we're compiling C++. */
#ifdef __cplusplus
namespace scc {
#endif

/* All SCC headers must have one thing in common: the first 16 bytes are the
   magic number and the SCC format version number. This allows for easy
   identification of SCC formats. Each header struct is defined as packed,
   so we are sure the compiler does not expand the header. */


struct __attribute__((__packed__)) SCC3_Header /* 64 bytes */
{
    _scc_u64_t  h_magic;            /* Magic number */
    _scc_u64_t  h_version;          /* SCC format version */
    _scc_u32_t  m_ins;              /* Instruction amount */
    _scc_u8_t   pad1[4];
    _scc_u8_t   m_reg;              /* Register amount */
    _scc_u8_t   pad2[7];
    _scc_u8_t   pad3[24];
    _scc_u8_t   s_pre;              /* Signature prefix */
    _scc_u8_t   s_comp;             /* Compiler signature */
};


#ifdef __cplusplus
} // namespace scc
#endif

#endif /* SCC_H */

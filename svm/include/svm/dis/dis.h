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
 * @author bellrise
 */
#ifndef SVM_DIS_H
#define SVM_DIS_H

/* The built-in SVM disassembler is used for providing a readable version of the
   compiled Salt code. All versions of SCC starting from SCC3 should be
   supported, as long as they are not deprecated. */

#include <svm/svm.h>


/* The current version of the disassembler. This can only be changed here. */
#define SVM_DISASSEMBLER_VERSION "0.1"

/**
 * Disassemble the given file into human readable instructions.
 *
 * @param   filename    name of the file to disassemble
 * @return  0 if the compilation did not fail
 */
i32 disassemble(SVMRuntime *_rt, char *filename);


#endif /* SVM_DIS_H */

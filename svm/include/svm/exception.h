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
#ifndef SVM_EXCEPTION_H
#define SVM_EXCEPTION_H

/* SVM Exceptions do not provide a traceback, nor catch statements. They are
   only for printing useful information to the user about failed execution of
   code. */

#include <svm/svm.h>


/* Exception strings. You should use these instead of raw strings. */
#define EXCEPTION_RUNTIME  "RuntimeException"
#define EXCEPTION_TYPE     "TypeException"
#define EXCEPTION_MEMORY   "MemoryException"
#define EXCEPTION_LABEL    "LabelException"
#define EXCEPTION_NULLPTR  "NullPointerException"
#define EXCEPTION_REGISTER "RegisterException"
#define EXCEPTION_READONLY "ReadOnlyExeception"

/**
 * Throw an exception and exit the virtual machine safely.
 *
 * @param exception title of exception (use EXCEPTION_xxx)
 * @param fmt       printf format style const string
 */
void exception_throw(SVMRuntime *_rt, const char *__restrict exception,
                     const char *__restrict fmt, ...);


#endif /* SVM_EXCEPTION_H */

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
 * The callstack structure is a stack which stores the name and location
 * of all the previous calls.
 *
 * @author  bellrise
 */
#ifndef SVM_CALLSTACK_H
#define SVM_CALLSTACK_H

#include "core.h"

#include <stdint.h>

/**
 * The callstack is built out of single StackFrames. Because objects are stored
 * in the module scope, and there are no local variables, so the stack frame 
 * only points.
 *
 * @a module   name of the module
 * @a function name of the function 
 * @a line     the line the call was made on
 */
struct StackFrame {

    char     module[62];
    char     function[62];
    uint32_t line;

};

/**
 * Push one frame onto the callstack.
 *
 * @param   line     line number where the call happened
 * @param   module   title of module, is checked if it's shorter than 62 chars
 * @param   function title of the function, is checked if it's shorter than 62
 *                   chars.
 */
void callstack_push(SVMRuntime *_rt, uint32_t line, char *__restrict module, 
                    char *__restrict function);

/**
 * Peek at the top of the callstack. After using the data inside the frame,
 * be sure to pop it to remove it from the stack.
 *
 * @return  last element of the callstack or NULL if callstack is empty
 */
struct StackFrame *callstack_peek() Nullable;

/**
 * Remove the last element of the callstack from the frame.
 */
void callstack_pop(SVMRuntime *_rt);

#endif // SVM_CALLSTACK_H

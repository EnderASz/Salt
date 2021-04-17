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
#ifndef SVM_CALLSTACK_H
#define SVM_CALLSTACK_H

/* The callstack stores the location of all the calls so the virtual machine
   can return back to the previous point when a return instruction is
   executed. */

#include <svm/svm.h>


/**
 * The callstack is built out of single StackFrames. Because objects are stored
 * in the module scope, and there are no local variables, so the stack frame
 * only points.
 */
struct StackFrame
{
    struct SaltModule *module_;
    char function[64];
    u32  line;
};

/**
 * Push one frame onto the callstack.
 *
 * @param   line     line number where the call happened
 * @param   module_  title of module, is checked if it's shorter than 62 chars
 * @param   function title of the function, is checked if it's shorter than 62
 *                   chars.
 */
void callstack_push(SVMRuntime *_rt, u32 line, struct SaltModule *module_,
                    char *__restrict function);

/**
 * Peek at the top of the callstack. After using the data inside the frame,
 * be sure to pop it to remove it from the stack.
 *
 * @return  last element of the callstack or NULL if callstack is empty
 */
struct StackFrame *callstack_peek(SVMRuntime *_rt) Nullable;

/**
 * Remove the last element of the callstack from the frame.
 */
void callstack_pop(SVMRuntime *_rt);


#endif /* SVM_CALLSTACK_H */

/**
 * The callstack structure is a stack which stores the name and location
 * of all the previous calls.
 *
 * @author  bellrise
 */
#ifndef SVM_CALLSTACK_H
#define SVM_CALLSTACK_H

#include "core.h"

struct StackFrame {

    char module[62];
    char function[62];
    int line;

};

/**
 * Push one frame onto the callstack.
 *
 * @param   line  line number where the call happened
 */
void callstack_push(uint line, char *module, char *function);

/**
 * Size of the current callstack.
 *
 * @return number of elements on the callstack
 */
int callstack_size();

/**
 * Peek at the top of the callstack. After using the data inside the frame,
 * be sure to pop it to remove it from the stack.
 *
 * @return  last element of the callstack
 */
struct StackFrame __NULLABLE *callstack_peek();

/**
 * Remove the last element of the callstack from the frame
 */
void callstack_pop();

#endif // SVM_CALLSTACK_H

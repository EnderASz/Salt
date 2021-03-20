/**
 * This is the exceptions module for throwing SVM exceptions.
 *
 * @author  bellrise
 */
#ifndef SVM_EXCEPTION_H
#define SVM_EXCEPTION_H

#define EXCEPTION_RUNTIME  "RuntimeException"
#define EXCEPTION_TYPE     "TypeException"
#define EXCEPTION_MEMORY   "MemoryException"
#define EXCEPTION_LABEL    "LabelException"

/**
 * Throw an exception and exit the virtual machine.
 *
 * @param exception title of exception
 * @param msg       message under the exception (cause of problem)
 */
void exception_throw(const char *exception, const char *msg);

#endif // SVM_EXCEPTION_H

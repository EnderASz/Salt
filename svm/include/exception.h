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
#define EXCEPTION_NULLPTR  "NullPointerException"
#define EXCEPTION_REGISTER "RegisterException"

/**
 * Throw an exception and exit the virtual machine.
 *
 * @param exception title of exception
 * @param fmt       printf format style
 */
void exception_throw(const char *exception, const char *fmt, ...);

#endif // SVM_EXCEPTION_H

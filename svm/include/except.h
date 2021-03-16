/**
 * Exception module for SVM. 
 *
 *
 * 
 * @author bellrise
 */
#ifndef EXCEPT_H_
#define EXCEPT_H_

#include "core.h"
#include "object.h"
#include "utils.h"

/* Deconstruct the callstack and throw an error. This depends on the xregister
 * for its data. See core.h::xregister for more info.
 *
 * @param   title  title of the exception
 * @param   msg    message under the exception
 */
void except_throw(char *title, char *msg);

#endif // EXCEPT_H
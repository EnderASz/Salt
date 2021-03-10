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
 */
void except_throw();

/* Set the exception message in the xregisters to the passed strings.
 *
 * @title   title of the exception   
 * @msg     message that will show under the exception
 */
void except_set(const char *title, const char *msg);

#endif // EXCEPT_H
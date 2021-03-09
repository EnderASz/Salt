/**
 * The utils module contains several functions mostly for printing data to 
 * standard out.
 *
 * @author bellrise
 */
#ifndef UTILS_H_
#define UTILS_H_

#include "object.h"

/* Wrapper for the calloc function, checks if the memory has been allocated. 
 *
 * @_size       size of each item
 * @_elements   amount of items
 */
void *alloc(uint _size, uint _elements);

/* Raise the number to the given exponential
 *
 * @_num    number to raise
 * @_exp    exponential
 *
 * returns: power
 */
uint util_pow(uint _num, short _exp);

/* Print the contents of the object. This is basically a switch statement that
 * calls different static functions for each type of object.
 *
 * @_obj object to print
 */
void util_print_object(SaltObject *_obj);

/* Subroutine for printing an int object
 *
 * @_obj the object of type int
 */
static void _util_print_int(SaltObject *_obj);

/* Cast the passed string to a uint.
 *
 * @_str string with only numbers
 *
 * returns: uint or 0 if fails
 */
uint str_to_uint(char *_str);

#endif // UTILS_H_
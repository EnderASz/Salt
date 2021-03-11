/**
 * The utils module contains several functions mostly for printing data to 
 * standard out.
 *
 * @author bellrise
 */
#ifndef UTILS_H_
#define UTILS_H_

#include "object.h"
#include "core.h"

/* Printf that only works when DEBUG is defined */
#ifdef DEBUG
  #define dprintf(...) printf(__VA_ARGS__)
#else
  #define dprintf(...)
#endif

/* Wrapper for the calloc function, checks if the memory has been allocated. 
 *
 * @_size       size of each item
 * @_elements   amount of items
 */
void *vmalloc(uint _size, uint _elements);

/* Wrapper for the realloc function, counts memory.
 *
 * @_ptr      pointer to memory
 * @_initial  memory size (before)
 * @_new      new size
 *
 * returns: pointer to the new memory
 */
void *vmrealloc(void *_ptr, uint _initial, uint _new);

/* Free the given pointer and remove the amount of bytes from the total stack
 *
 * @_ptr    pointer to memory
 * @_size   size of memory
 */
void vmfree(void *_ptr, uint _size);

/* Raise the number to the given exponential
 *
 * @_num    number to raise
 * @_exp    exponential
 *
 * returns: power
 */
uint util_pow(uint _num, short _exp);

/* Pull the next argument from the pos location.
 *
 * @_argc   amount of total arguments
 * @_argv   array of arguments
 * @_pos    current position
 * @_field  string of field
 *
 * returns: read uint
 */
uint util_arg_uint(int _argc, char **_argv, int *_pos, char *_field);

/* Print the contents of the object. This is basically a switch statement that
 * calls different static functions for each type of object.
 *
 * @_obj object to print
 */
void util_print_object(SaltObject *_obj);

/* Subroutines for util_print_object */
void util_dumpv_null   (SaltObject *_obj);
void util_dumpv_int    (SaltObject *_obj);
void util_dumpv_float  (SaltObject *_obj);
void util_dumpv_string (SaltObject *_obj);
void util_dumpv_bool   (SaltObject *_obj);
void util_dumpv_array  (SaltObject *_obj);

/* Print n amount of chars in hex form.
 *
 * @_bytes  pointer to first byte
 * @_amount amount of bytes
 */
void util_hexdump(byte *_bytes, uint _amount);

/* Generate data from the given type and pointer, returning a pointer to the 
 * allocated data. 
 *
 * @_type   type of the object
 * @_data   pointer to the bytes
 *
 * returns: void pointer to allocated data
 */
void *util_generate_data(byte _type, void *_data);

/* Return size of allocated payload
 * 
 * @_obj    pointer to object
 * 
 * returns: size of object
 */
uint util_get_size(SaltObject *_obj);

/* Cast the passed string to a uint.
 *
 * @_str string with only numbers
 *
 * returns: uint or 0 if fails
 */
uint str_to_uint(char *_str);

#endif // UTILS_H_
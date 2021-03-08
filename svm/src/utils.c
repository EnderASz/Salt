/**
 * utils.h implementation
 * 
 * @author bellirse
 */
#include "../include/utils.h"
#include "../include/core.h"

#include <stdio.h>
#include <stdlib.h>

/* Wrapper for the calloc function, checks if the memory has been allocated. 
 *
 * @_size:       size of each item
 * @_elements:   amount of items
 */
void *alloc(uint _size, uint _elements)
{
    void *ptr = calloc(_size, _elements);
    if (ptr == NULL) {
        CORE_ERROR("Failed to allocate memory.\n");
        exit(1);
    }
    return ptr;
}

/* Print the contents of the object. This is basically a switch statement that
 * calls different static functions for each type of object.
 *
 * @_obj: object to print
 */
void util_print_object(SaltObject *_obj)
{
    switch (_obj->type) {
        
        case SALT_INT:
            _util_print_int(_obj);
            break;

        default:
            printf("Cannot print object type %02hhx\n", _obj->type);
    }
}

/* Subroutine for printing an int object
 *
 * @_obj: the object of type int
 */
inline void _util_print_int(SaltObject *_obj)
{
    printf("%d", * (int *) _obj->data);
}

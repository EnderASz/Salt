/**
 * utils.h implementation
 * 
 * @author bellirse
 */
#include "../include/utils.h"
#include "../include/core.h"

#include <stdio.h>
#include <stdlib.h>

/* Wrapper for the calloc function, checks if the memory has been allocated. */
void *alloc(uint _size, uint _elements)
{
    void *ptr = calloc(_size, _elements);
    if (ptr == NULL) {
        CORE_ERROR("Failed to allocate memory.\n");
        exit(1);
    }
    return ptr;
}

/* Raise the number to the given exponential */
uint util_pow(uint _num, short _exp)
{
    uint base = _num;
    for (short i = 1; i < _exp; i++) {
        _num = base * _num;
    }
    return _num;
}

/* Print the contents of the object. This is basically a switch statement that
 * calls different static functions for each type of object. */
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

/* Subroutine for printing an int object */
inline void _util_print_int(SaltObject *_obj)
{
    printf("%d", * (int *) _obj->data);
}

/* Cast the passed string to a uint. */
uint str_to_uint(char *_str)
{
    uint total = 0;
    char c;

    for (short i = strlen(_str) - 1; i >= 1; i--) {
        c = _str[strlen(_str) - i - 1];
        if ((c < 0x30) || (c > 0x39)) {
            return 0;
        }

        total += (c - 0x30) * util_pow(10, i);
    }

    total += _str[strlen(_str) - 1] - 0x30;
    
    return total;
}

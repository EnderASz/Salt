/**
 * utils.h implementation
 * 
 * @author bellirse
 */
#include "../include/utils.h"
#include "../include/core.h"

#include <stdio.h>
#include <stdlib.h>

void *vmalloc(uint _size, uint _elements)
{
    svm_allocated += _size * _elements;
    dprintf("svm_allocated is %lld\n", svm_allocated);

    if (svm_max_mem)
        if (svm_allocated > svm_max_mem)
            CORE_ERROR("MemoryException: out of memory\n");

    void *ptr = calloc(_size, _elements);
    if (ptr == NULL) {
        CORE_ERROR("Failed to allocate memory\n");
    }
    return ptr;
}

void vmfree(void *_ptr, uint _size)
{
    svm_allocated -= _size;
    free(_ptr);
}

uint util_pow(uint _num, short _exp)
{
    uint base = _num;
    for (short i = 1; i < _exp; i++) {
        _num = base * _num;
    }
    return _num;
}

uint util_arg_uint(int _argc, char **_argv, int *_pos, char *_field)
{
    (*_pos)++;
    if (*_pos >= _argc) {
        printf("svm: %s requires an argument\n", _field);
        exit(1);
    }

    uint size = str_to_uint(_argv[*_pos]);
    if (!size) {
        printf("svm: %s requires and uint\n", _field);
        exit(1);
    }
    return size;
}

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

void *util_generate_data(byte _type, void *_data)
{
    void *ptr;
    switch (_type) {
    
    case SALT_INT:
        ptr = vmalloc(sizeof(int), 1);
        * (int *) ptr = * (int *) _data;
        break;

    case SALT_FLOAT:
        ptr = vmalloc(sizeof(float), 1);
        * (float *) ptr = * (float *) _data;
        break;
    
    case SALT_STRING:
        ptr = vmalloc(sizeof(char), * (uint *) _data);
        strncpy(ptr, _data + 4, * (uint *) _data);
        break;

    case SALT_NULL:
        ptr = NULL;
        break;

    case SALT_BOOL:
        ptr = vmalloc(sizeof(byte), 1);
        * (byte *) ptr = * (byte *) _data;
        break;

    default:
        CORE_ERROR("unknown data type when creating object\n");

    }
    return ptr;
}

uint util_get_size(SaltObject *_obj)
{
    switch (_obj->type) {

    case SALT_INT:
        return 4;
    case SALT_FLOAT:
        return 4;
    case SALT_NULL:
        return 0;
    case SALT_BOOL:
        return 1;
    case SALT_STRING:
        return * (uint *) _obj->typeinfo;
    }
    return -1;
}

inline void _util_print_int(SaltObject *_obj)
{
    printf("%d", * (int *) _obj->data);
}

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

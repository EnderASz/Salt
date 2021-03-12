/**
 * utils.h implementation
 * 
 * @author bellirse
 */
#include "../include/utils.h"
#include "../include/core.h"
#include "../include/except.h"

#include <stdio.h>
#include <stdlib.h>

static void (*_dumpv_calls[6]) (SaltObject *) = {
    util_dumpv_null,
    util_dumpv_int,
    util_dumpv_float,
    util_dumpv_string,
    util_dumpv_bool,
    util_dumpv_array
};


void *vmalloc(uint _size, uint _elements)
{
    svm_allocated += _size * _elements;
    dprintf("vmalloc(%d)\n", _size * _elements);

    if (svm_max_mem && (svm_allocated > svm_max_mem)) {
        except_set("MemoryException", "out of memory");
        except_throw();
    }

    void *ptr = calloc(_size, _elements);
    if (ptr == NULL) {
        except_set("MemoryException", "cannot allocate memory");
        except_throw();        
    }
    return ptr;
}

void *vmrealloc(void *_ptr, uint _initial, uint _new)
{
    svm_allocated = svm_allocated - _initial + _new;
    dprintf("vmrealloc(%d)\n", _new);

    if (svm_max_mem && (svm_allocated > svm_max_mem)) {
        except_set("MemoryException", "out of memory");
        except_throw();
    }

    void *ptr = realloc(_ptr, _new);
    if (ptr == NULL) {
        except_set("MemoryExeception", "cannot reallocate memory");
        except_throw();
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
    _dumpv_calls[_obj->type](_obj);
}

void util_hexdump(byte *_bytes, uint _amount)
{   
    for (uint i = 0; i < _amount; i++) {
        printf("%02hhx", _bytes[i]);
        if (i % 2 != 0)
            printf(" ");
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
        except_set("TypeException", "invalid type for object creation");
        except_throw();

    }
    return ptr;
}

uint util_get_size(SaltObject *_obj)
{
    switch (_obj->type) {

    case SALT_INT:
        return sizeof(int);
    case SALT_FLOAT:
        return sizeof(float);
    case SALT_NULL:
        return 0;
    case SALT_BOOL:
        return sizeof(byte);
    case SALT_STRING:
        return * (uint *) _obj->typeinfo;
    }
    return -1;
}

inline void util_dumpv_int(SaltObject *_obj)
{ printf("%d", * (int *) _obj->data); }

inline void util_dumpv_float(SaltObject *_obj)
{ printf("%f", * (float *) _obj->data); }

inline void util_dumpv_array(SaltObject *_obj)
{ printf("Array printing not implemented yet (%d)\n", _obj->id); }

inline void util_dumpv_null(SaltObject *_obj)
{ printf("<null : %d>", _obj->id); }

inline void util_dumpv_string(SaltObject *_obj)
{
    for (uint i = 0; i < * (uint *) _obj->typeinfo; i++) {
        fputc(((char *) _obj->data)[i], stdout);
    }
}

inline void util_dumpv_bool(SaltObject *_obj)
{
    if (* (byte *) _obj->data)
        printf("true");
    else
        printf("false");
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

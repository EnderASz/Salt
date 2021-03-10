/**
 * object.h implementation
 *
 * @author bellrise
 */
#include "../include/object.h"
#include "../include/utils.h"
#include <string.h>
#include <stdlib.h>

uint salt_id_counter = 100;

uint salt_id()
{
    salt_id_counter++;
    return salt_id_counter - 1;
}

SaltObject salt_object_create(uint id, byte type, byte permission, 
           byte constant, byte *typeinfo, void *data, uint mutex_id,
           uint scope_id)
{
    SaltObject obj;
    obj.id         = id;
    obj.type       = type;
    obj.permission = permission;
    obj.constant   = constant;
    obj.mutex_id   = mutex_id;
    obj.scope_id   = scope_id;
    obj.data       = data;
    
    if (typeinfo == NULL)
        memset(obj.typeinfo, 0, 8);
    else
        memcpy(obj.typeinfo, typeinfo, 8);

    return obj;
}

SaltObject salt_object_mkconst(byte type, byte *typeinfo, void *data)
{
    return salt_object_create(salt_id(), type, 0, 1, typeinfo, data, 0, 0);
}

uint salt_object_strlen(SaltObject *obj)
{
    return * (uint *) obj->typeinfo;
}

struct SaltArray salt_array_create(byte size, byte constant)
{
    struct SaltArray arr;
    arr.array = vmalloc(sizeof(SaltObject), size);
    arr.space = size;
    arr.size  = 0;

    return arr;
}

void salt_array_append(struct SaltArray *array, SaltObject data)
{
    if (array->space <= array->size) {
        array->array = realloc(array->array, sizeof(SaltObject) 
                       * (array->space + 16));

        array->space += 1;
    }   

    array->array[array->size] = data;
    array->size++;
}

uint salt_array_length(SaltObject *obj)
{
    return (* (struct SaltArray *) obj->data).size;
}

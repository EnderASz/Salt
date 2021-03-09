/**
 * object.h implementation
 *
 * @author bellrise
 */
#include "../include/object.h"
#include "../include/utils.h"
#include <string.h>
#include <stdlib.h>

/* Global ID of salt objects. The initial value of this is 100, leaving space
 for system variables ranging from 0 to 100. */
uint salt_id_counter = 100;

/* Get a unique ID for every newly created object.
 *
 * returns: unique ID
 */
uint salt_id()
{
    salt_id_counter++;
    return salt_id_counter - 1;
}


/* The full method for creating a brand new Salt Object. Defines all the fields
 * which it can assign to. This method is private because it should be called
 * using other wrapper functions that are much easier to use.
 *
 * @id          unique object id
 * @type        type of the object
 * @permission  permission level, basically who created the object 
 * @constant    if the object is constant or not
 * @typeinfo    additional information about the type. the data is zeroed if 
 *              NULL is passed here
 * @data        pointer to the actual data, needs to be allocated on the heap
 * @mutex_id    id of the mutex, used for locking threads
 * @scope_id    id of the scope the variable was declared in
 * 
 * returns: brand new SaltObject
 */
SaltObject _salt_object_create(uint id, byte type, byte permission, 
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

/* Create a new constant variable from the passed information.
 *
 * @type     type of the object
 * @typeinfo additional type information
 * @data     pointer to the allocated data
 *
 * returns: brand new constant SaltObject
 */
SaltObject salt_object_mkconst(byte type, byte *typeinfo, void *data)
{
    return _salt_object_create(salt_id(), type, 0, 1, typeinfo, data, 0, 0);
}

/* Cast the typeinfo of the SaltObject into a uint.
 *
 * @_obj: salt object of string type
 * 
 * returns: length of the string
 */
uint salt_object_strlen(SaltObject *obj)
{
    return * (uint *) obj->typeinfo;
}

/* Create a new Salt array with the specified parameters.
 *
 * @size      initial size of the array
 * @constant  additional information about the type
 *
 * returns: new salt object of type SALT_ARRAY
 */
struct SaltArray salt_array_create(byte size, byte constant)
{
    struct SaltArray arr;
    arr.array = alloc(sizeof(SaltObject), size);
    arr.space = size;
    arr.size  = 0;

    return arr;
}

/* Append a single object to the array. This copies the object data so you
 * can safely use local variables to append.
 *
 * @array   pointer to array
 * @data    data to add
 */
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

/* Return the length of the Salt array.
 *
 * @obj  pointer to salt object
 */
uint salt_array_length(SaltObject *obj)
{
    return (* (struct SaltArray *) obj->data).size;
}

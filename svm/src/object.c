/**
 * object.h implementation
 *
 * @author bellrise
 */
#include "../include/object.h"
#include <string.h>

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
    return _salt_object_create(salt_id(), type, 1, 1, typeinfo, data, 0, 0);
}

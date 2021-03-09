/**
 * The object module provides functionality around the SaltObject, which is
 * a universal data container that stores any type of data coming from Salt.
 *
 *
 * @author bellrise
 */
#ifndef OBJECT_H_
#define OBJECT_H_

typedef unsigned int  uint;
typedef unsigned char byte; 

/* This is a list of built-in Salt object types. */
#define SALT_NULL   0x00
#define SALT_INT    0x01
#define SALT_FLOAT  0x02 
#define SALT_STRING 0x03
#define SALT_BOOL   0x04

#define SALT_ARRAY  0x80

/* Permission levels */
#define PERM_USER   0
#define PERM_SVM    1

/* The SaltObject structure type is a universal data container used everywhere
 in Salt to hold any type of data. This is achieved by leaving space for a void
 pointer, which you can then dereference to get the data. */
typedef struct _salt_object_st { // (32)

    /* unique ID of the object, assigned by salt_id() */
    uint          id;
    
    /* type information */
    byte        type;
    byte  permission;
    byte    constant;
    byte       _pad1;

    byte typeinfo[8];

    /* pointer to the data itself. of course, this needs to allocated when 
     creating a new object. */
    void       *data;

#if ARCH == 32
    /* this 4-byte padding is defined only when the compilation destination of
     the SVM is for a 32-bit platform, where pointers are only 4 bytes instead
     of 8 bytes */
    byte    _pad2[4];
#endif

    /* for threading */
    uint    mutex_id;

    /* for scope access */
    uint    scope_id;

} SaltObject;

/* This stores an dynamic array of SaltObjects. Use the functions that come 
 with this because it supports dynamic allocation. */
struct SaltArray { // (16)

    /* current size of array */
    uint         size;

    /* allocated space */
    uint        space;

    SaltObject *array;

};

/* Global ID of salt objects. The initial value of this is 100, leaving space
 for system variables ranging from 0 to 100. */
extern uint salt_id_counter;

/* Get a unique ID for every newly created object.
 *
 * returns: unique ID
 */
uint salt_object_id();

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
SaltObject salt_object_create(uint id, byte type, byte permission, 
           byte constant, byte *typeinfo, void *data, uint mutex_id,
           uint scope_id);

/* Create a new constant variable from the passed information.
 *
 * @type     type of the object
 * @typeinfo additional type information
 * @data     pointer to the allocated data
 *
 * returns: brand new constant SaltObject
 */
SaltObject salt_object_mkconst(byte type, byte *typeinfo, void *data);

/* Cast the typeinfo of the SaltObject into a uint.
 *
 * @_obj: salt object of string type
 * 
 * returns: length of the string
 */
uint salt_object_strlen(SaltObject *obj);

/* Create a new Salt array with the specified parameters.
 *
 * @size      initial size of the array
 * @constant  additional information about the type
 *
 * returns: new salt object of type SALT_ARRAY
 */
struct SaltArray salt_array_create(byte size, byte constant);

/* Append a single object to the array. This copies the object data so you
 * can safely use local variables to append.
 *
 * @array   pointer to array
 * @data    data to add
 */
void salt_array_append(struct SaltArray *array, SaltObject data);

/* Return the length of the Salt array.
 *
 * @obj  pointer to salt object
 */
uint salt_array_length(SaltObject *obj);

#endif // OBJECT_H_

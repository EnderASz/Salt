/**
 * This module provides the structure & functionality for the SaltObject
 *
 * @author  bellrise
 */
#ifndef SVM_OBJECT_H
#define SVM_OBJECT_H

#include "core.h"

#define ACCESS_PUBLIC  0
#define ACCESS_PRIVATE 1

// Simple types

#define OBJECT_TYPE_NULL   (0x00)
#define OBJECT_TYPE_INT    (0x01)
#define OBJECT_TYPE_FLOAT  (0x02)
#define OBJECT_TYPE_BOOL   (0x03)
#define OBJECT_TYPE_STRING (0x04)

/* This represents a single object, which can hold different types of data
 depending on the [type] byte. When building a new object, default values are
 assigned an the constructor & destructor pointers are passed, which take care
 of allocating the needed memory on the heap. Be sure to never build a raw
 object yourself, instead use salt_object_create(...). */
struct SaltObject {

    /* object information */
    uint id;
    byte readonly;
    byte access;
    byte type;

    uint size;
    void *value;

    /* methods */
    void (*constructor) (struct SaltObject *obj, char *bytes);
    void (*destructor ) (struct SaltObject *obj);

    /* threading support */
    uint threaded;
    byte mutex_aquired;

};

/* Because the salt object will be used very often, I add a typedef here.
 This does not obstruct the information that a SaltObject is a structure,
 because this whole project revolves around such a thing. */
typedef struct SaltObject SaltObject;

/**
 * Create a raw salt object. Assign all the needed values, settings the object
 * to a null type & value.
 *
 * @return brand new heap allocated salt object.
 */
SaltObject *salt_object_create();

/**
 * This should be assigned to each created salt object in order to free
 * the proper amount of memory.
 *
 * @param obj pointer to the salt object
 */
void salt_object_destructor(SaltObject* obj);

#endif // SVM_OBJECT_H

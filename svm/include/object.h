/**
 * This module provides the structure & functionality for the SaltObject
 *
 * @author  bellrise
 */
#ifndef SVM_OBJECT_H
#define SVM_OBJECT_H

#include "core.h"

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

    /* value data */
    uint allocated;
    void *value_ptr;

    /* methods */
    void (* constructor) (struct SaltObject *obj, char *bytes);
    void (* destructor ) (struct SaltObject *obj);

    /* threading support */
    uint threaded;
    byte mutex_aquired;

};

/* Because the salt object will be used very often, I add a typedef here.
 This does not obstruct the information that a SaltObject is a structure,
 because this whole project revolves around such a thing. */
typedef struct SaltObject SaltObject;

#endif // SVM_OBJECT_H

/**
 * Salt Virtual Machine
 * 
 * Copyright (C) 2021  The Salt Programming Language Developers
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * END OF COPYRIGHT NOTICE
 *
 * This module provides the structure & functionality for the SaltObject
 *
 * @author bellrise, 2021
 */
#ifndef SVM_OBJECT_H
#define SVM_OBJECT_H

#define READONLY_FALSE (0x00)
#define READONLY_TRUE  (0x01)

// Simple types

#define OBJECT_TYPE_NULL   (0x00)
#define OBJECT_TYPE_INT    (0x01)
#define OBJECT_TYPE_FLOAT  (0x02)
#define OBJECT_TYPE_BOOL   (0x03)
#define OBJECT_TYPE_STRING (0x04)

typedef __UINT8_TYPE__  byte;
typedef __UINT32_TYPE__ uint;

/**
 * This represents a single object, which can hold different types of data
 * depending on the [type] byte. When building a new object, default values are
 * assigned an the constructor & destructor pointers are passed, which take 
 * care of allocating the needed memory on the heap. Be sure to never build a 
 * raw object yourself, instead use salt_object_create(...). 
 *
 * @a id            ID of the object
 * @a readonly      0x01 if the object should be const 
 * @a type          type of the object, see OBJECT_TYPE_xxx
 * @a mutex_acquired if this is not 1, it means the object 
 * @a value         pointer to the value
 * @a size          amount of bytes allocated
 * @a vhandler      value handler method
 * @a destructor    object destructor
 */
struct SaltObject {

    uint  id;
    byte  readonly;
    byte  type;
    byte  mutex_acquired;

    byte _pad1[1];

    uint  size;
    void *value;

    void ( * vhandler  ) 
    (struct SaltObject *obj, byte *bytes);
    
    void ( * destructor ) 
    (struct SaltObject *obj);

#if TARGET_ARCH == 32
    byte _pad2[12];
#endif

};

/* Because the salt object will be used very often, I add a typedef here.
 This does not obstruct the information that a SaltObject is a structure,
 because this whole project revolves around such a thing. */
typedef struct SaltObject SaltObject;

/**
 * Create a raw salt object. Assign all the needed values, settings the object
 * to a null type & value. This calls salt_object_init under the hood.
 *
 * @return brand new heap allocated salt object.
 */
SaltObject *salt_object_create();

/**
 * Initializes the passed object with all the default values.
 *
 * @param   obj  pointer to salt object
 */
void salt_object_init(SaltObject *obj);

/**
 * Deep copy a salt object.
 *
 * @param   dest  destination object
 * @param   src   source object
 */
void salt_object_copy(SaltObject *dest, SaltObject *src);

/**
 * Create a new salt object from the given payload. See doc/scc.html for
 * information about the payload.
 *
 * @param   payload  pointer to bytes
 * @return  brand new heap allocated SaltObject.
 */
void salt_object_define(SaltObject *obj, byte *payload);

/**
 * Print the object without a newline or space after it.
 *
 * @param   obj  pointer to the object
 */
void salt_object_print(SaltObject *obj);

/**
 * This should be assigned to each created salt object in order to free
 * the proper amount of memory.
 *
 * @param   obj  pointer to the salt object
 */
void salt_object_destructor(SaltObject* obj);

#endif // SVM_OBJECT_H

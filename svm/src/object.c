//
// Created by bellrise on 17.03.2021.
//
#include "../include/object.h"
#include "../include/core.h"

SaltObject *salt_object_create()
{
    SaltObject *ptr = vmalloc(sizeof(SaltObject));

    ptr->locked = 0;

    ptr->id = 0;
    ptr->readonly = 0;
    ptr->access = ACCESS_PUBLIC;
    ptr->type = OBJECT_TYPE_NULL;

    ptr->size = 0;
    ptr->value = NULL;

    ptr->constructor = NULL;
    ptr->destructor = salt_object_destructor;

    ptr->threaded = 0;
    ptr->mutex_aquired = 0;
    return ptr;
}

static void render_value(SaltObject *obj, byte *payload)
{

}

void salt_object_define(SaltObject *obj, byte *payload)
{

}

void salt_object_destructor(SaltObject *obj)
{
    vmfree(obj->value, obj->size);
}

//
// Created by bellrise on 17.03.2021.
//
#include "../include/object.h"
#include "../include/core.h"

SaltObject *salt_object_create()
{
    SaltObject *ptr = vmalloc(sizeof(SaltObject));
    salt_object_init(ptr);
    return ptr;
}

void salt_object_init(SaltObject *obj)
{
    obj->id = 0;
    obj->readonly = 0;
    obj->access = ACCESS_PUBLIC;
    obj->type = OBJECT_TYPE_NULL;
    obj->mutex_aquired = 0;

    obj->value = NULL;
    obj->size = 0;

    obj->vhandler = NULL;
    obj->destructor = salt_object_destructor;
}

void salt_object_print(SaltObject *obj)
{
    if (obj == NULL) {
        puts("null");
        return;
    }

    switch (obj->type) {

        case OBJECT_TYPE_INT:
            printf("%d", * (int *) obj->value);
            break;

        case OBJECT_TYPE_FLOAT:
            printf("%f", * (float *) obj->value);
            break;

        case OBJECT_TYPE_STRING:
            printf("%s", (char *) obj->value);
            break;

        case OBJECT_TYPE_BOOL:
            if (* (byte *) obj->value)
                printf("true");
            else
                printf("false");
            break;

        default:
            printf("null");
    }
}

static void render_value(SaltObject *obj, byte *payload)
{

}

void salt_object_define(SaltObject *obj, byte *payload)
{
    obj->id       = * (uint *) payload;
    obj->readonly = * (byte *) payload + 4;
    obj->access   = * (byte *) payload + 5;
    obj->type     = * (byte *) payload + 6;
    obj->mutex_aquired = * (byte *) payload + 7;
    render_value(obj, payload + 8);
}

void salt_object_destructor(SaltObject *obj)
{
    vmfree(obj->value, obj->size);
}

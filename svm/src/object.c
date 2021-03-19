//
// Created by bellrise on 17.03.2021.
//
#include "../include/object.h"
#include "../include/core.h"
#include "../include/utils.h"
#include "../include/exception.h"

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

    dprintf("Printing %d of type 0x%02hhx\n", obj->id, obj->type);

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
    dprintf("Rendering type 0x%02hhx\n", obj->type);
    switch (obj->type) {

        case OBJECT_TYPE_INT:
            obj->value = vmalloc(sizeof(int));
            obj->size = sizeof(int);
            memcpy(obj->value, payload, 4);
            return;

        case OBJECT_TYPE_FLOAT:
            obj->value = vmalloc(sizeof(float));
            obj->size = sizeof(float);
            memcpy(obj->value, payload, 4);
            return;

        case OBJECT_TYPE_STRING:
            obj->size = * (uint *) payload;
            obj->value = vmalloc(sizeof(char) * obj->size);
            memcpy(obj->value, (payload + sizeof(uint)), obj->size);

            for (uint i = 0; i < obj->size; i++) {
                if (((char *) obj->value)[i] == '\x11')
                    ((char *) obj->value)[i] = '\n';
            }

            return;

        case OBJECT_TYPE_BOOL:
            obj->value = vmalloc(1);
            obj->size = 1;
            memcpy(obj->value, payload, 1);
            return;

        case OBJECT_TYPE_NULL:
            return;

        default:
            exception_throw(EXCEPTION_TYPE, "Cannot print object");
    }
}

void salt_object_define(SaltObject *obj, byte *payload)
{
    obj->id       = * (uint *) payload;
    obj->readonly = * (byte *) (payload + 4);
    obj->access   = * (byte *) (payload + 5);
    obj->type     = * (byte *) (payload + 6);
    obj->mutex_aquired = * (byte *) (payload + 7);
    render_value(obj, payload + 8);
}

void salt_object_destructor(SaltObject *obj)
{
    vmfree(obj->value, obj->size);
}

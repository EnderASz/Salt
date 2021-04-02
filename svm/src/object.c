/**
 * core.h implementation from the salt object size
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/utils.h"
#include "../include/exception.h"

SaltObject *salt_object_create(SVMRuntime *_rt)
{
    SaltObject *ptr = vmalloc(sizeof(SaltObject));
    
    ptr->ctor = salt_object_ctor;
    ptr->dtor = salt_object_dtor;

    return ptr;
}

void salt_object_copy(SVMRuntime *_rt, SaltObject *dest, SaltObject *src)
{
    dprintf("Copying object {%d} to {%d}\n", src->id, dest->id);
    dest->id = src->id;
    dest->type = src->type;
    dest->readonly = src->readonly;
    dest->mutex_acquired = src->mutex_acquired;

    dest->ctor = src->ctor;
    dest->dtor = src->dtor;
    dest->print = src->print;

    dest->size = src->size;
    if (dest->size != 0) {
        dest->value = vmalloc(dest->size);
        memcpy(dest->value, src->value, dest->size);
    }
    else {
        dest->value = NULL;
    }
}

void salt_object_print(SVMRuntime *_rt, SaltObject *obj)
{
    if (obj == NULL) {
        puts("null");
        return;
    }

    if (obj->id == 0)
        exception_throw(_rt, EXCEPTION_NULLPTR, "Cannot resolve object");

    dprintf("Printing {%d} of type 0x%02hhx\n", obj->id, obj->type);

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
            if (* (u8 *) obj->value)
                printf("true");
            else
                printf("false");
            break;

        case OBJECT_TYPE_NULL:
            printf("null");
    }
}

static void render_value(SVMRuntime *_rt, SaltObject *obj, u8 *payload)
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
            obj->size = * (u32 *) payload;
            obj->value = vmalloc(sizeof(char) * obj->size);
            memcpy(obj->value, (payload + sizeof(u32)), obj->size);

            for (u32 i = 0; i < obj->size; i++) {
                if (((char *) obj->value)[i] == '\x11')
                    ((char *) obj->value)[i] = '\n';
            }

            // Add null terminator in case of some wacky file reading
            ((char *) obj->value)[obj->size - 1] = 0;
            return;

        case OBJECT_TYPE_BOOL:
            obj->value = vmalloc(1);
            obj->size = 1;
            memcpy(obj->value, payload, 1);
            return;

        case OBJECT_TYPE_NULL:
            return;

        default:
            exception_throw(_rt, EXCEPTION_TYPE, "Cannot print object");
    }
}

void salt_object_define(SVMRuntime *_rt, SaltObject *obj, u8 *payload)
{
    obj->id       = * (u32 *) payload;
    obj->readonly = * (u8  *) (payload + 4);
    obj->type     = * (u8  *) (payload + 5);
    render_value(_rt, obj, payload + 6);
    dprintf("Created object {%d} of type 0x%02hhx\n", obj->id, obj->type);
}

void salt_object_ctor(void *_rt, SaltObject *self)
{
    self->id = 0;
    self->readonly = 0;
    self->mutex_acquired = 0;

    self->value = NULL;
    self->size = 0;
}

void salt_object_dtor(void *_rt, SaltObject *self)
{
    vmfree(self->value, self->size);
}


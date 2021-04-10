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
 * The Salt Virtual Machine is the interpreter for compiled Salt code generated
 * by saltc, the Salt compiler. It is written in C to have more control over 
 * the bytes and what is happening in the background, to achieve better 
 * execution speeds. This code is mostly written and handled by me (bellrise)
 * but there may be more people in the future wanting to contribute to the
 * project. 
 *
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
    dprintf("Copying object {%d} to {%d}", src->id, dest->id);
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

    switch (obj->type) {

        case OBJECT_TYPE_INT:
            printf("%d", * (i32 *) obj->value);
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
    dprintf("Rendering type 0x%02hhx", obj->type);
    switch (obj->type) {

        case OBJECT_TYPE_INT:
            obj->value = vmalloc(sizeof(i32));
            obj->size = sizeof(i32);
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
    dprintf("Created object {%d} of type 0x%02hhx", obj->id, obj->type);
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


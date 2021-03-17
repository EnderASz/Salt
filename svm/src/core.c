//
// Created by bellrise on 17.03.2021.
//
#include "../include/core.h"
#include "../include/data.h"
#include "../include/module.h"
#include "../include/object.h"
#include <stdlib.h>

void *vmalloc(uint size)
{
    data_memory_used += size;
    return malloc(size);
}

void vmfree(void *ptr, uint size)
{
    data_memory_used -= size;
    free(ptr);
}

void *vmrealloc(void *ptr, uint before, uint after)
{
    data_memory_used -= before;
    data_memory_used += after;

    return realloc(ptr, (unsigned long) after);
}
//
// Created by bellrise on 17.03.2021.
//
#include "../include/core.h"
#include "../include/data.h"
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
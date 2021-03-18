//
// Created by bellrise on 17.03.2021.
//
#include "../include/core.h"
#include "../include/module.h"
#include "../include/object.h"
#include <stdlib.h>

static uint64_t g_memory_used = 0;

void *vmalloc(uint size)
{
    g_memory_used += size;
    return malloc(size);
}

void vmfree(void *ptr, uint size)
{
    g_memory_used -= size;
    free(ptr);
}

void *vmrealloc(void *ptr, uint before, uint after)
{
    g_memory_used -= before;
    g_memory_used += after;

    return realloc(ptr, (unsigned long) after);
}

uint64_t vmused()
{
    return g_memory_used;
}
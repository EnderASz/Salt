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

void clean_module(struct SaltModule *module)
{
    for (uint i = 0; i < module->object_amount; i++)
        module->objects[i].destructor(&module->objects[i]);

    for (uint i = 0; i < module->function_ptr_amount; i++) {
        vmfree(module->function_ptr, sizeof(*module->function_ptr) * module->function_ptr_amount);
    }

    for (uint i = 0; i < module->import_amount; i++) {
        vmfree(module->imports, sizeof(*module->imports) * module->import_amount);
    }

    for (uint i = 0; i < module->instruction_amount; i++) {
        vmfree(module->instructions[i].content, module->instructions[i].size * sizeof(char));
    }
}

void clean_all()
{
    for (uint i = 0; i < MODULE_AMOUNT; i++) {
        clean_module(&MODULES[i]);
    }
}
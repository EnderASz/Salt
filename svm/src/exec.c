/**
 * exec.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/core.h"
#include "../include/exec.h"
#include "../include/exception.h"
#include "../include/callstack.h"
#include "../include/utils.h"
#include "../include/module.h"
#include "../include/object.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

/* killx is the last instruction if no other is found. */
static const struct SVMCall g_execs[] = {
        
    {"KILLX", exec_killx},
    {"RPUSH", exec_rpush},
    {"RGPOP", exec_rgpop},
    {"EXITE", exec_exite},
    {"IVADD", exec_ivadd},
    {"IVSUB", exec_ivsub},
    {"CALLF", exec_callf},
    {"EXTLD", exec_extld},
    {"OBJMK", exec_objmk},
    {"OBJDL", exec_objdl},
    {"PRINT", exec_print},
    {"RETRN", exec_retrn},
    {"MLMAP", exec_mlmap}

};

static const uint g_exec_amount = 13;

/* registers */
static SaltObject *g_registers;
static uint8_t g_register_size = 0;


static uint exec_find_end(struct SaltModule *module)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strncmp(content, "@$__END__", 9) == 0)
            return module->labels[i];
    }
    exception_throw(EXCEPTION_RUNTIME, "Cannot find end label");
    return 0;
}

inline static SaltObject *fetch_from_tape(struct SaltModule *module, uint id)
{
    SaltObject *obj = module_object_find(module, id);
    if (!obj) {
        exception_throw(EXCEPTION_NULLPTR, "Object %d not found", id);
    }
    return obj;
}

static uint preload(struct SaltModule *main)
{
    uint i = 0;
    for (uint j = 0; j < main->label_amount; j++) {
        if (strncmp(main->instructions[main->labels[j]].content, "@main", 5) == 0) {
            dprintf("Found main function at [%d]\n", main->labels[j]);
            i = main->labels[j];
        }
    }

    if (i == 0)
        exception_throw(EXCEPTION_RUNTIME, "main function not found");

    return i;
}

/* main exec */

int exec(struct SaltModule *main)
{
    dprintf("Executing '%s'\n", main->name);

    uint i = preload(main);

    // "Call" the main instruction
    callstack_push(exec_find_end(main), main->name, "main");

    for (; i < main->instruction_amount;) {

        if (*main->instructions[i].content == '@') {
            i++;
            continue;
        }

        dprintf("[%d] < \033[95m%.5s\033[0m >\n", i, main->instructions[i].name);

        const struct SVMCall *exec = exec_get(main->instructions[i].name);
        i = exec->f_exec(main, (byte *) main->instructions[i].content + 5, i);
    }
    return 0;
}

const struct SVMCall *exec_get(char *title)
{
    for (uint i = 0; i < g_exec_amount; i++) {
        if (strncmp(g_execs[i].instruction, title, 5) == 0)
            return &g_execs[i];
    }
    return &g_execs[0];
}

void register_control(uint8_t size)
{
    if (g_register_size < size) {
        g_registers = vmrealloc(
            g_registers, 
            sizeof(SaltObject) * g_register_size, 
            sizeof(SaltObject) * size
        );
        g_register_size = size;
        for (uint8_t i = 0; i < g_register_size; i++)
            salt_object_init(&g_registers[i]);
    }
}

void register_clear()
{
    for (uint i = 0; i < g_register_size; i++) {
        vmfree(g_registers[i].value, g_registers[i].size);
    }
    vmfree(g_registers, sizeof(SaltObject) * g_register_size);
}

uint exec_callf(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    uint strl = * (uint * ) payload;
    char *name = vmalloc(sizeof(char) * strl + 1);
    strncpy(name, (char *) payload + 4, strl);
    name[strl - 1] = 0;

    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strncmp(content + 1, name, strl - 1) == 0) {
            callstack_push(pos, module->name, name);
            vmfree(name, strl + 1);
            return module->labels[i];
        }
    }

    vmfree(name, strl + 1);
    exception_throw(EXCEPTION_LABEL, "Cannot find function");
    return 0;
}

uint exec_exite(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    return exec_find_end(module);
}

uint exec_extld(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    exception_throw(EXCEPTION_RUNTIME, "EXTLD is not implemented yet");
    return ++pos;
}

uint exec_ivadd(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    SaltObject *obj = fetch_from_tape(module, * (uint *) payload);
    if (obj->type != OBJECT_TYPE_INT)
        exception_throw(EXCEPTION_TYPE, "Cannot add to non-int type");

    if (obj->readonly)
        exception_throw(EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Adding %d\n", * (int *) (payload + 4));
    * (int *) obj->value += * (int *) (payload + 4);

    return ++pos;
}

uint exec_ivsub(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    SaltObject *obj = fetch_from_tape(module, * (uint *) payload);
    if (obj->type != OBJECT_TYPE_INT)
        exception_throw(EXCEPTION_TYPE, "Cannot subtract from non-int type");
    
    if (obj->readonly)
        exception_throw(EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Subtracting %d\n", * (int *) (payload + 4));
    * (int *) obj->value -= * (int *) (payload + 4);

    return ++pos;
}

uint exec_killx(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    core_exit();
    return ++pos;
}

uint exec_mlmap(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    struct SaltObjectNode *node = module->head;
    while (node != NULL) {

        printf("SaltObjectNode[%d] %p : %p : %p\n", node->data.id, 
              (void *) node->previous, (void *) node, (void *) node->next);

        node = node->next;
    }

    return ++pos;
}


uint exec_objmk(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    SaltObject *obj = module_object_acquire(module);
    salt_object_define(obj, payload);
    return ++pos;
}

uint exec_objdl(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    module_object_delete(module, * (uint *) payload);
    return ++pos;
}

uint exec_print(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{

    SaltObject *obj = module_object_find(module, * (uint *) payload);
    if (obj == NULL) {
        exception_throw(EXCEPTION_NULLPTR, "Cannot find object %d", 
                        * (uint *) payload);
    }

    salt_object_print(obj);
    return ++pos;
}

uint exec_retrn(struct SaltModule *__restrict module, byte *__restrict payload,  
                uint pos)
{
    struct StackFrame *frame = callstack_peek();
    if (frame == NULL) {
        pos = exec_find_end(module);
    }
    else {
        pos = frame->line + 1;
        if (module->instruction_amount <= pos - 1) {
            pos--;
        }
    }
    dprintf("Jumping back to [%d]\n", pos);
    callstack_pop();
    return ++pos;
}

uint exec_rpush(struct SaltModule *__restrict module, byte *__restrict payload,
                uint pos)
{
    uint8_t r = * (uint8_t *) payload;
    uint id = * (uint *) (payload + 1);

    SaltObject *obj = module_object_find(module, id);
    if (obj == NULL) {
        exception_throw(EXCEPTION_RUNTIME, "Cannot load object %d into the "
                "register because it does not exist");
    }

    if (r >= g_register_size)
        exception_throw(EXCEPTION_REGISTER, "Register %d out of bounds");

    dprintf("Pushing to register [%d] from {%d}\n", r, id);

    g_registers[r].id = obj->id;
    g_registers[r].type = obj->type;
    g_registers[r].readonly = obj->readonly;

    g_registers[r].mutex_acquired = obj->mutex_acquired;
    g_registers[r].destructor = obj->destructor;
    g_registers[r].vhandler = obj->vhandler;
    
    g_registers[r].size = obj->size;
    g_registers[r].value = vmalloc(g_registers[r].size);

    // Copy data over
    for (uint i = 0; i < g_registers[r].size; i++)
        ((uint8_t *) g_registers[r].value)[i] = ((uint8_t *) obj->value)[i];

    module_object_delete(module, obj->id);

    return ++pos;
}

uint exec_rgpop(struct SaltModule *__restrict module, byte *__restrict payload,
                uint pos)
{
    uint8_t r = * (uint8_t *) payload;
    uint id = * (uint *) (payload + 1);

    SaltObject *obj = module_object_acquire(module);

    if (r >= g_register_size)
        exception_throw(EXCEPTION_REGISTER, "Register %d out of bounds");

    dprintf("Pulling from register [%d] to {%d}\n", r, id);

    // todo : copy object
    obj->id = id;
    obj->type = g_registers[r].type;
    obj->readonly = g_registers[r].readonly;

    obj->mutex_acquired = g_registers[r].mutex_acquired;
    obj->destructor = g_registers[r].destructor;
    obj->vhandler = g_registers[r].vhandler;

    obj->size = g_registers[r].size;
    obj->value = vmalloc(obj->size);

    // Copy data over
    for (uint i = 0; i < g_registers[r].size; i++)
        ((uint8_t *) obj->value)[i] = ((uint8_t *) g_registers[r].value)[i];

    vmfree(g_registers[r].value, g_registers[r].size);
    g_registers[r].value = NULL;
    g_registers[r].size = 0;

    return ++pos;  
}

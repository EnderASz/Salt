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

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <unistd.h>
#endif

// ----------------------------------------------------------------------------
// Global const values
// ----------------------------------------------------------------------------

/* killx is the last instruction if no other is found. */
const struct SVMCall g_execs[] = {
        
    {"KILLX", exec_killx, 0},
    {"RPUSH", exec_rpush, 5},
    {"RGPOP", exec_rgpop, 5},
    {"EXITE", exec_exite, 0},
    {"IVADD", exec_ivadd, 8},
    {"IVSUB", exec_ivsub, 8},
    {"JMPTO", exec_jmpto, 4},
    {"JMPFL", exec_jmpfl, 8},
    {"JMPNF", exec_jmpnf, 8},
    {"CALLF", exec_callf, 4},
    {"EXTLD", exec_extld, 4},
    {"OBJMK", exec_objmk, 7},
    {"OBJDL", exec_objdl, 4},
    {"PRINT", exec_print, 4},
    {"RETRN", exec_retrn, 0},
    {"MLMAP", exec_mlmap, 0},
    {"CXXEQ", exec_cxxeq, 8},
    {"CXXNE", exec_cxxne, 8},
    {"SLEEP", exec_sleep, 4}

};

const uint g_exec_amount = 19;

// ----------------------------------------------------------------------------
// Utility & loop functions
// ----------------------------------------------------------------------------

static uint find_label(SVMRuntime *_rt, struct SaltModule *module, char *label)
{
    for (uint i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strncmp(content + 1, label, strlen(label) - 1) == 0)
            return module->labels[i];
    }
    exception_throw(_rt, EXCEPTION_LABEL, "Cannot find '%s' label", label);
    return 0;
}

inline static SaltObject *fetch_from_tape(SVMRuntime *_rt, struct SaltModule 
                          *module, uint id)
{
    dprintf("Looking up object {%d} in '%s'\n", id, module->name);
    SaltObject *obj = module_object_find(module, id);
    if (!obj) {
        exception_throw(_rt, EXCEPTION_NULLPTR, "Object %d not found", id);
    }
    return obj;
}

static uint preload(SVMRuntime *_rt, struct SaltModule *main)
{
    uint i = 0;
    for (uint j = 0; j < main->label_amount; j++) {
        if (strncmp(main->instructions[main->labels[j]].content, "@main", 6) == 0) {
            dprintf("Found main function at [%d]\n", main->labels[j]);
            i = main->labels[j];
        }
    }

    if (i == 0)
        exception_throw(_rt, EXCEPTION_RUNTIME, "main function not found");

    return i;
}

/* main exec */

int exec(SVMRuntime *_rt, struct SaltModule *main)
{
    dprintf("Executing '%s'\n", main->name);

    uint i = preload(_rt, main);

    // "Call" the main instruction
    callstack_push(_rt, main->instruction_amount - 1, main->name, "main");

    for (; i < main->instruction_amount;) {

        if (main->instructions[i].content[0] == '@') {
            i++;
            continue;
        }

        dprintf("[%d] < \033[95m%.5s\033[0m >\n", i, main->instructions[i].content);

        const struct SVMCall *exec = exec_get(main->instructions[i].content);
        i = exec->f_exec(_rt, main, (byte *) main->instructions[i].content + 5, i);
    }

    dprintf("Finished exec\n");
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

void register_control(SVMRuntime *_rt, uint8_t size)
{
    dprintf("Changing %d to %d\n", _rt->register_size, size);
    if (_rt->register_size < size) {
        _rt->registers = vmrealloc(
            _rt->registers, 
            sizeof(SaltObject) * _rt->register_size, 
            sizeof(SaltObject) * size
        );
        _rt->register_size = size;
        for (uint8_t i = 0; i < _rt->register_size; i++) {
            _rt->registers[i].ctor = salt_object_ctor;
            _rt->registers[i].dtor = salt_object_dtor;

            _rt->registers[i].ctor(_rt, &_rt->registers[i]);
        }
    }
}

void register_clear(SVMRuntime *_rt)
{
    for (uint i = 0; i < _rt->register_size; i++) {
        vmfree(_rt->registers[i].value, _rt->registers[i].size);
    }
    vmfree(_rt->registers, sizeof(SaltObject) * _rt->register_size);
}

static void copy_object(SVMRuntime *_rt, SaltObject *dest, SaltObject *src)
{
    dest->id = src->id;
    dest->type = src->type;
    dest->readonly = src->readonly;

    dest->mutex_acquired = src->mutex_acquired;
    dest->ctor = src->ctor;
    dest->dtor = src->dtor;
    dest->print = src->print;

    // Free previous memory in dest
    vmfree(dest->value, dest->size);
    dest->value = NULL;
    dest->size = 0;

    dest->size = src->size;
    dest->value = vmalloc(dest->size);
    memcpy(dest->value, src->value, dest->size);
}

// ----------------------------------------------------------------------------
// Instruction implementations
// ----------------------------------------------------------------------------

uint exec_callf(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    uint line = find_label(_rt, module, (char *) (payload + 4));
    callstack_push(_rt, pos, module->name, (char *) (payload + 4));
    return line;
}

uint exec_cxxeq(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    SaltObject *o1 = module_object_find(module, * (uint *) payload);
    SaltObject *o2 = module_object_find(module, * (uint *) (payload + 4));

    dprintf("Comparing {%d} and {%d}\n", o1->id, o2->id);

    if (o1 == NULL || o2 == NULL)
        exception_throw(_rt, EXCEPTION_NULLPTR, "Cannot find object");

    if (o1->type != o2->type)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot compare two different types");

    switch (o1->type) {

        case OBJECT_TYPE_BOOL:
            if (* (byte *) o1->value == * (byte *) o2->value)
                _rt->compare_flag = 1;
            break;

        /* the float & int case may look pretty slow, but the alternative is 
         comparing 4 bytes, while the CPU can compare ints & floats in one 
         clock cycle. */
        case OBJECT_TYPE_FLOAT:
        {
            if (* (float *) o1->value == * (float *) o2->value)
                _rt->compare_flag = 1;
            break;
        }
        case OBJECT_TYPE_INT:
        {
            if (* (int *) o1->value == * (int *) o2->value)
                _rt->compare_flag = 1;
            break;
        }
        case OBJECT_TYPE_STRING:
        {
            uint size = o1->size;
            if (o2->size != size) {
                _rt->compare_flag = 0;
                break;
            }

            if(strncmp((char *) o1->value, (char *) o2->value, size - 1) == 0)
                _rt->compare_flag = 1;
            else
                _rt->compare_flag = 0;
            break;
        }
        
        default:
            /* if the type is unknown, return false by default */
            _rt->compare_flag = 0;
    
    }

    return ++pos;
}

uint exec_cxxne(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    exec_cxxeq(_rt,module, payload, pos);
    _rt->compare_flag = !_rt->compare_flag;
    return pos;
}

uint exec_exite(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    return module->instruction_amount - 1;
}

uint exec_extld(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    exception_throw(_rt, EXCEPTION_RUNTIME, "EXTLD is not implemented yet");
    return ++pos;
}

uint exec_ivadd(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    SaltObject *obj = fetch_from_tape(_rt, module, * (uint *) payload);
    if (obj->type != OBJECT_TYPE_INT)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot add to non-int type");

    if (obj->readonly)
        exception_throw(_rt, EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Adding %d\n", * (int *) (payload + 4));
    * (int *) obj->value += * (int *) (payload + 4);

    return ++pos;
}

uint exec_ivsub(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    SaltObject *obj = fetch_from_tape(_rt, module, * (uint *) payload);
    if (obj->type != OBJECT_TYPE_INT)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot subtract from non-int type");
    
    if (obj->readonly)
        exception_throw(_rt, EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Subtracting %d\n", * (int *) (payload + 4));
    * (int *) obj->value -= * (int *) (payload + 4);

    return ++pos;
}

uint exec_jmpfl(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    dprintf("Compare flag on %02hx\n", _rt->compare_flag); 
    if (_rt->compare_flag)
        return find_label(_rt, module, (char *) (payload + 4));

    return ++pos;
}

uint exec_jmpnf(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    dprintf("Compare flag on %02hx\n", _rt->compare_flag); 
    if (!_rt->compare_flag)
        return find_label(_rt, module, (char *) (payload + 4));

    return ++pos;
}

uint exec_jmpto(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    return find_label(_rt, module, (char *) (payload + 4));
}

uint exec_killx(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    core_exit(_rt);
    return ++pos;
}

uint exec_mlmap(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    struct SaltObjectNode *node = module->head;
    while (node != NULL) {

        printf("SaltObjectNode[%d] %p : %p : %p\n", node->data.id, 
              (void *) node->previous, (void *) node, (void *) node->next);

        node = node->next;
    }

    return ++pos;
}


uint exec_objmk(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    SaltObject *obj = module_object_acquire(_rt, module);
    salt_object_define(_rt, obj, payload);
    return ++pos;
}

uint exec_objdl(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    module_object_delete(_rt, module, * (uint *) payload);
    return ++pos;
}

uint exec_print(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{

    SaltObject *obj = module_object_find(module, * (uint *) payload);
    if (obj == NULL) {
        exception_throw(_rt, EXCEPTION_NULLPTR, "Cannot find object %d", 
                        * (uint *) payload);
    }

    salt_object_print(_rt, obj);
    return ++pos;
}

uint exec_retrn(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    struct StackFrame *frame = callstack_peek(_rt);
    if (frame == NULL) {
        pos = module->instruction_amount - 1;
    }
    else {
        pos = frame->line + 1;
        if (module->instruction_amount <= pos - 1) {
            pos--;
        }
    }
    dprintf("Jumping back to [%d]\n", pos);
    callstack_pop(_rt);
    return pos;
}

uint exec_rpush(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
    uint8_t r = * (uint8_t *) payload;
    uint id = * (uint *) (payload + 1);

    SaltObject *obj = module_object_find(module, id);
    if (obj == NULL) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot load object %d into the "
                "register because it does not exist");
    }

    if (r >= _rt->register_size)
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d out of bounds");

    dprintf("Pushing to register [%d] from {%d}\n", r, id);

    copy_object(_rt, &_rt->registers[r], obj);
    
    module_object_delete(_rt, module, obj->id);

    return ++pos;
}

uint exec_rgpop(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload, uint pos)
{
    uint8_t r = * (uint8_t *) payload;
    uint id = * (uint *) (payload + 1);

    SaltObject *obj = module_object_acquire(_rt, module);

    if (r >= _rt->register_size)
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d out of bounds");

    dprintf("Pulling from register [%d] to {%d}\n", r, id);

    copy_object(_rt, obj, &_rt->registers[r]);
    obj->id = id;

    vmfree(_rt->registers[r].value, _rt->registers[r].size);
    _rt->registers[r].value = NULL;
    _rt->registers[r].size = 0;

    return ++pos;  
}

uint exec_sleep(SVMRuntime *_rt, struct SaltModule *__restrict module, 
                byte *__restrict payload,  uint pos)
{
#if defined(_WIN32)
    /* The windows sleep is a bit easier because it uses miliseconds by 
     default. */
    Sleep(* (int *) payload);

#elif defined(__linux__)
    /* The linux sleep on the other hand takes seconds, so we must use nanosleep
     here. The bad thing is usleep has been deprecated so we have to use some
     timespec magic. */
     
    exception_throw(_rt, EXCEPTION_RUNTIME, "Sleep does not work on linux yet. "
                    "Too bad.");

#endif
    return ++pos;
}

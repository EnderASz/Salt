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
 * exec.h implementation
 *
 * @author bellrise, 2021
 */
#include "../include/svm.h"
#include "../include/exec.h"
#include "../include/exception.h"
#include "../include/callstack.h"
#include "../include/utils.h"
#include "../include/module.h"
#include "../include/loader.h"

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
    {"RDUMP", exec_rdump, 1},
    {"EXITE", exec_exite, 0},
    {"IVADD", exec_ivadd, 8},
    {"IVSUB", exec_ivsub, 8},
    {"RETRN", exec_retrn, 0},
    {"JMPTO", exec_jmpto, 0},
    {"JMPFL", exec_jmpfl, 0},
    {"JMPNF", exec_jmpnf, 0},
    {"CALLF", exec_callf, 0},
    {"CALLX", exec_callx, 0},
    {"EXTLD", exec_extld, 0},
    {"OBJMK", exec_objmk, 7},
    {"OBJDL", exec_objdl, 4},
    {"PRINT", exec_print, 4},
    {"MLMAP", exec_mlmap, 0},
    {"TRACE", exec_trace, 0},
    {"CXXEQ", exec_cxxeq, 8},
    {"CXXLT", exec_cxxlt, 8},
    {"RNULL", exec_rnull, 0},
    {"SLEEP", exec_sleep, 4},
    {"PASSL", exec_passl, 0}

};

const u32 g_exec_amount = 24;

// ----------------------------------------------------------------------------
// Utility & loop functions
// ----------------------------------------------------------------------------

static u32 find_label(SVMRuntime *_rt, struct SaltModule *module, char *label)
{
    for (u32 i = 0; i < module->label_amount; i++) {
        char *content = module->instructions[module->labels[i]].content;
        if (strncmp(content + 1, label, strlen(label)) == 0)
            return module->labels[i];
    }
    exception_throw(_rt, EXCEPTION_LABEL, "Cannot find '%s' label", label);
    return 0;
}

inline static SaltObject *fetch_from_tape(SVMRuntime *_rt, struct SaltModule 
                          *module, u32 id)
{
    dprintf("Looking up object {%d} in '%s'", id, module->name);
    SaltObject *obj = module_object_find(module, id);
    if (!obj) {
        exception_throw(_rt, EXCEPTION_NULLPTR, "Object %d not found", id);
    }
    return obj;
}

/* main exec */

i32 exec(SVMRuntime *_rt, struct SaltModule *main, const char *start)
{
    dprintf("Executing '%s'", main->name);

    u32 i = find_label(_rt, main, (char *) start);

    // "Call" the main instruction
    u32 initial_depth = _rt->callstack_size;
    callstack_push(_rt, i, main, (char *) start);

    for (; i < main->instruction_amount;) {
    
        if (main->instructions[i].content[0] == '@') {
            i++;
            continue;
        }

        dprintf("[%d] \033[95m%.5s\033[0m", i, main->instructions[i].content);
    
        const struct SVMCall *exec = lookup_exec(main->instructions[i].content);
        i = exec->f_exec(_rt, main, (u8 *) main->instructions[i].content + 5, i);
    
        if (_rt->callstack_size == initial_depth)
            return 0;
    } 
    
    return 0;
}

const struct SVMCall *lookup_exec(char *title)
{
    register i32 itext = * (i32 *) title;

    for (u32 i = 0; i < g_exec_amount; i++) {

        /* This magic comparison without using strncmp is done by casting 
         * the first 4 chars into a 32 bit (4 byte) integer, and then comparing
         * it to the 4 chars of the instruction. This is a lot faster than
         * using the strncmp loop. The last char is also just compared using a
         * simple == operator. 
         *
         * Note that this had to be done because this might be the most called 
         * function in the whole SVM, so it should be optimized.
         */
        if (itext != * (i32 *) g_execs[i].instruction) {
            continue;
        }

        if (title[4] == g_execs[i].instruction[4])
           return &g_execs[i]; 

    }
    return &g_execs[0];
}

void register_control(SVMRuntime *_rt, u8 size)
{
    if (_rt->register_size < size) {
        dprintf("Changing %d to %d", _rt->register_size, size);
        _rt->registers = vmrealloc(
            _rt->registers, 
            sizeof(SaltObject) * _rt->register_size, 
            sizeof(SaltObject) * size
        );
        _rt->register_size = size;
        for (u32 i = 0; i < _rt->register_size; i++) {
            _rt->registers[i].ctor = salt_object_ctor;
            _rt->registers[i].dtor = salt_object_dtor;

            _rt->registers[i].ctor(_rt, &_rt->registers[i]);
        }
    }
}

void register_clear(SVMRuntime *_rt)
{
    for (u32 i = 0; i < _rt->register_size; i++) {
        vmfree(_rt->registers[i].value, _rt->registers[i].size);
        _rt->registers[i].value = NULL;
        _rt->registers[i].size = 0;
    }
    vmfree(_rt->registers, sizeof(SaltObject) * _rt->register_size);
}

static void copy_object(SVMRuntime *_rt, SaltObject *dest, SaltObject *src)
{
    dest->id = src->id;
    dest->type = src->type;
    dest->readonly = src->readonly;

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

/* Type comparison functions. These are here to make the code more readable for
   the user.  */

#define COMPARISON_NOT_EQUAL    0
#define COMPARISON_EQUAL        1
#define COMPARISON_LESSER       2
#define COMPARISON_GREATER      3

static u8 compare_floats(float *v1, float *v2)
{
    if (*v1 == *v2)
        return COMPARISON_EQUAL;
    if (*v1 < *v2)
        return COMPARISON_LESSER;
    return COMPARISON_GREATER;
}

static u8 compare_ints(int *v1, int *v2)
{
    if (*v1 == *v2)
        return COMPARISON_EQUAL;
    if (*v1 < *v2)
        return COMPARISON_LESSER;
    return COMPARISON_GREATER;
}

static u8 compare_strings(u32 len, char *v1, char *v2)
{
    u32 i = 0;
    for (; i < len; i++) {
        if (v1[i] != v2[i])
             goto compare_distace;
    }
    return COMPARISON_EQUAL;

compare_distace:
    if (v1[i] < v2[i])
        return COMPARISON_LESSER;
    return COMPARISON_GREATER;
}

static u8 compare_objects(SVMRuntime *_rt, SaltObject *o1, SaltObject *o2)
{
    dprintf("Comparing {%d} and {%d}", o1->id, o2->id);

    if (o1 == NULL || o2 == NULL)
        exception_throw(_rt, EXCEPTION_NULLPTR, "Cannot find object");

    if (o1->type != o2->type)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot compare two different types");

    switch (o1->type) {

        /* Compare two boolean values. Return COMPARISON_EQUAL if they are
           equal. Otherwise, if the first value is false and the second
           value is true, return COMPARISON_LESSER. Return COMPARISON_GREATER
           for the opposite. */

        case SALT_TYPE_BOOL:
        {
            if (* (u8 *) o1->value == * (u8 *) o2->value)
                return COMPARISON_EQUAL;
            if (* (u8 *) o1->value < * (u8 *) o2->value)
                return COMPARISON_LESSER;
            return COMPARISON_GREATER;
        }

        /* Standard rules apply to the float & int comparison. */

        case SALT_TYPE_FLOAT:
        {
            return compare_floats((float *) o1->value, (float *) o2->value);
        }
        case SALT_TYPE_INT:
        {
            return compare_ints((int *) o1->value, (int *) o2->value);
        }

        /* Comparing string is a little more complex. Because we strive for
           a small amount of instructions in the SVM but each rich with
           functionality, comparing strings have their own set of special
           rules.

           First, check if both strings are the same length. If not,
           return the comparison of the lengths. This is also done to reduce
           computation time, because if the strings are not equal we already
           know they won't be equal. Then check the single chars, and return
           COMPARISON_NOT_EQUAL if the strings do not match. */

        case SALT_TYPE_STRING:
        {
            if (o1->size != o2->size) {
                if (o1->size > o2->size)
                    return COMPARISON_GREATER;
                return COMPARISON_LESSER;
            }

            return compare_strings(o1->size, (char *) o1->value, (char *) o2->value);
        }

        /* Crash if we try to compare an unsupported type. */
        default:
        {
            exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot compare object"
                    " %u with %u", o1->id, o2->id);
            return COMPARISON_NOT_EQUAL;
        }
    }
}

// ----------------------------------------------------------------------------
// Instruction implementations
// ----------------------------------------------------------------------------

__SVMCALL (callf) /* _rt, module, payload, pos */
{
    dprintf("Calling '%s'", payload);
    exec(_rt, module, (char *) payload);
    return ++pos;
}

__SVMCALL (callx) /* _rt, module, payload, pos */
{
    char *mod_name  = (char *) payload;
    char *func_name = ((char *) payload) + strlen(mod_name) + 1;

    dprintf("Calling external '%s.%s'", mod_name, func_name);

    struct SaltModule *mod = NULL;
    // Find the module
    for (u32 i = 0; i < module->module_amount; i++) {
        if (strncmp(module->modules[i]->name, mod_name, strlen(mod_name)) == 0)
            mod = module->modules[i];
    }

    if (!mod)
        exception_throw(_rt, EXCEPTION_RUNTIME, "'%s' has not been imported", mod_name);

    dprintf("Found '%s' module, now looking for '%s' function", mod_name, func_name);

    // Call the external function
    exec(_rt, mod, func_name);

    return ++pos;
}

__SVMCALL (cxxeq) /* _rt, module, payload, pos */
{
    SaltObject *o1 = module_object_find(module, * (u32 *) payload);
    SaltObject *o2 = module_object_find(module, * (u32 *) (payload + 4));

    _rt->flag_comparison = 0;
    if (compare_objects(_rt, o1, o2) == COMPARISON_EQUAL)
        _rt->flag_comparison = 1;

    return ++pos;
}

__SVMCALL (cxxlt) /* _rt, module, payload, pos */
{
    SaltObject *o1 = module_object_find(module, * (u32 *) payload);
    SaltObject *o2 = module_object_find(module, * (u32 *) (payload + 4));

    _rt->flag_comparison = 1;
    if (compare_objects(_rt, o1, o2) == COMPARISON_LESSER)
        _rt->flag_comparison = 1;

    return ++pos;
}

__SVMCALL (exite) /* _rt, module, payload, pos */
{
    return module->instruction_amount - 1;
}

__SVMCALL (extld) /* _rt, module, payload, pos */
{
    const char *name = (char *) payload;
    struct SaltModule *mod = NULL;
    
    /* If the module already exists in the runtime array, don't load
     the module again, just assign a new pointer in the local module 
     imports array. */ 
    for (u32 i = 0; i < _rt->module_size; i++) {
        if (strncmp(_rt->modules[i]->name, name, strlen(name)) == 0)
            mod = _rt->modules[i];
    }
    
    if (!mod)
        mod = ext_load(_rt, (char *) name);

    /* Don't check if the import exists in the local table, just append
     it onto the local module list. */
    module->modules = vmrealloc(
        module->modules,
        sizeof(struct SaltModule *) * module->module_amount,
        sizeof(struct SaltModule *) * (module->module_amount + 1)
    );
    module->module_amount++;

    module->modules[module->module_amount - 1] = mod;

    /* Execute the private %__load function before returning to the program.
     This is done to load any globals the user would want to load beforehand,
     or execute some setup code that has to be done only once. */
    exec(_rt, mod, "%__load");    

    dprintf("%d", _rt->module_size);
    return ++pos;
}

__SVMCALL (ivadd) /* _rt, module, payload, pos */
{
    SaltObject *obj = fetch_from_tape(_rt, module, * (u32 *) payload);
    if (obj->type != SALT_TYPE_INT)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot add to non-i32 type");

    if (obj->readonly)
        exception_throw(_rt, EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Adding %d", * (i32 *) (payload + 4));
    * (i32 *) obj->value += * (i32 *) (payload + 4);

    return ++pos;
}

__SVMCALL (ivsub) /* _rt, module, payload, pos */
{
    SaltObject *obj = fetch_from_tape(_rt, module, * (u32 *) payload);
    if (obj->type != SALT_TYPE_INT)
        exception_throw(_rt, EXCEPTION_TYPE, "Cannot subtract from non-i32 type");
    
    if (obj->readonly)
        exception_throw(_rt, EXCEPTION_READONLY, "Cannot mutate read-only object");

    dprintf("Subtracting %d", * (i32 *) (payload + 4));
    * (i32 *) obj->value -= * (i32 *) (payload + 4);

    return ++pos;
}

__SVMCALL (jmpfl) /* _rt, module, payload, pos */
{
    dprintf("Compare flag on %02hx", _rt->flag_comparison);
    if (_rt->flag_comparison)
        return find_label(_rt, module, (char *) payload);

    return ++pos;
}

__SVMCALL (jmpnf) /* _rt, module, payload, pos */
{
    dprintf("Compare flag on %02hx", _rt->flag_comparison);
    if (!_rt->flag_comparison)
        return find_label(_rt, module, (char *) payload);

    return ++pos;
}

__SVMCALL (jmpto) /* _rt, module, payload, pos */
{
    return find_label(_rt, module, (char *) payload);
}

__SVMCALL (killx) /* _rt, module, payload, pos */
{
    core_exit(_rt);
    return ++pos;
}

__SVMCALL (mlmap) /* _rt, module, payload, pos */
{
    if (!_rt->arg_allow_debug)
        return ++pos;

    struct SaltObjectNode *node = module->head;
    while (node != NULL) {

        printf("SaltObjectNode[%d] %p : %p : %p\n", node->data.id, 
              (void *) node->previous, (void *) node, (void *) node->next);

        node = node->next;
    }

    return ++pos;
}


__SVMCALL (objmk) /* _rt, module, payload, pos */
{
    SaltObject *obj = module_object_acquire(_rt, module);
    salt_object_define(_rt, obj, payload);
    return ++pos;
}

__SVMCALL (objdl) /* _rt, module, payload, pos */
{
    module_object_delete(_rt, module, * (u32 *) payload);
    return ++pos;
}

__SVMCALL (passl) /* _rt, module, payload, pos */
{
    return ++pos;
}

__SVMCALL (print) /* _rt, module, payload, pos */
{
    SaltObject *obj = module_object_find(module, * (u32 *) payload);
    if (obj == NULL) {
        exception_throw(_rt, EXCEPTION_NULLPTR, "Cannot find object %d", 
                        * (u32 *) payload);
    }

    salt_object_print(_rt, obj);
    return ++pos;
}

__SVMCALL (rdump) /* _rt, module, payload, pos */
{
    u32 r = * (u32 *) payload;
    if (r >= _rt->register_size)
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d out of bounds", r);

    /* If the value is set to NULL and the type is not null, that means the 
     object has been removed. */    
    if (_rt->registers[r].value != NULL 
     && _rt->registers[r].type != SALT_TYPE_NULL)
        salt_object_print(_rt, &_rt->registers[r]);
    else
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d is empty", r);

    return ++pos;
}

__SVMCALL (retrn) /* _rt, module, payload, pos */
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
    dprintf("Jumping back to [%d]", pos);
    callstack_pop(_rt);
    return pos;
}

__SVMCALL (rgpop) /* _rt, module, payload, pos */
{
    u32 r = * (u32 *) payload;
    u32 id = * (u32 *) (payload + 1);

    SaltObject *obj = module_object_acquire(_rt, module);

    if (r >= _rt->register_size)
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d out of bounds", r);

    dprintf("Pulling from register [%d] to {%d}", r, id);

    copy_object(_rt, obj, &_rt->registers[r]);
    obj->id = id;

    vmfree(_rt->registers[r].value, _rt->registers[r].size);
    _rt->registers[r].value = NULL;
    _rt->registers[r].size = 0;

    return ++pos;  
}

__SVMCALL (rnull) /* _rt, module, payload, pos */
{
    for (u32 i = 0; i < _rt->register_size; i++) {
        vmfree(_rt->registers[i].value, _rt->registers[i].size);
        _rt->registers[i].value = NULL;
        _rt->registers[i].size = 0;
        
        /* Set it to a bool type so RDUMP knows this object is unprintable */
        _rt->registers[i].type = SALT_TYPE_BOOL;
    }
    return ++pos;
}

__SVMCALL (rpush) /* _rt, module, payload, pos */
{
    u32 r = * (u32 *) payload;
    u32 id = * (u32 *) (payload + 1);

    SaltObject *obj = module_object_find(module, id);
    if (obj == NULL) {
        exception_throw(_rt, EXCEPTION_RUNTIME, "Cannot load object %d into the "
                "register because it does not exist");
    }

    if (r >= _rt->register_size)
        exception_throw(_rt, EXCEPTION_REGISTER, "Register %d out of bounds");

    dprintf("Pushing to register [%d] from {%d}", r, id);

    copy_object(_rt, &_rt->registers[r], obj);
    
    module_object_delete(_rt, module, obj->id);

    return ++pos;
}

__SVMCALL (sleep) /* _rt, module, payload, pos */
{
#if defined(_WIN32)
    /* The windows sleep is a bit easier because it uses miliseconds by 
     default. */
    Sleep(* (i32 *) payload);

#elif defined(__linux__)
    /* The linux sleep on the other hand takes seconds, so we must use nanosleep
     here. The bad thing is usleep has been deprecated so we have to use some
     timespec magic. */
     
    exception_throw(_rt, EXCEPTION_RUNTIME, "Sleep does not work on linux yet. "
                    "Too bad.");

#endif
    return ++pos;
}

__SVMCALL (trace) /* _rt, module, payload, pos */
{
    if (!_rt->arg_allow_debug)
        return ++pos;

    for (u32 i = 0; i < _rt->callstack_size; i++) {
        printf("[%d](%d, %s, %s)\n", i, _rt->callstack[i].line, 
                _rt->callstack[i].module->name, _rt->callstack[i].function);  
    }

    return ++pos;
}

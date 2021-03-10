/**
 * exec.h implementation
 *
 * @author bellrise
 */
#include "../include/exec.h"
#include "../include/os.h"
#include "../include/utils.h"

#include <time.h>

const struct SaltExec salt_execs[] = {
    {"RXNEW", exec_rxnew},
    {"RXSET", exec_rxset},
    {"RXDEL", exec_rxdel},
    {"DUMPV", exec_dumpv},
    {"PRINT", exec_print},
    {"SLEEP", exec_sleep},
    {"DUMPI", exec_dumpi},
    {"KILLX", exec_killx}
};

static short salt_execs_l = 8;

uint exec_label_init = 0;
uint exec_label_end  = 0;

int exec(char **code)
{
    for (uint i = exec_label_init + 1; i != exec_label_end; i++) {

        // the signal recieved from any SaltExec function
        byte signal = EXEC_SIGPASS;

        for (short j = 0; j < salt_execs_l; j++) {
            
            if (strncmp(salt_execs[j].instruction, code[i], 5) == 0) {
                signal = salt_execs[j].f_exec((byte *) code[i] + 5);
                break;
            }

        }

        if (signal == EXEC_SIGERR)
            printf("svm: exec_ failed\n");

        if (signal == EXEC_SIGKILL)
            return 1;

    }

    return 0;
}

void preload(char **code)
{
    for (uint i = 0; i < svm_instructions; i++) {

        if (strncmp(code[i], "@$__INIT__", 11) == 0) {
            exec_label_init = i;
        }

        if (strncmp(code[i], "@$__END__", 10) == 0) {
            exec_label_end = i;
        }

    }
}

byte exec_dumpi(byte *data)
{
    uint id = * (uint *) data;
    uint oid = xregister.array[id].id;

    printf("%d", oid);

    return EXEC_SIGPASS;
}

byte exec_dumpv(byte *data)
{
    uint id = * (uint *) data;
    SaltObject *obj = xregister_find(id);
    if (obj != NULL)
        util_print_object(obj);
    
    return EXEC_SIGPASS;
}

byte exec_killx(byte *data)
{
    return EXEC_SIGKILL;   
}

byte exec_print(byte *data)
{
    uint pos = * (uint *) data - 1;
    printf("%s", (char *) salt_const_strings[pos].data);

    return EXEC_SIGPASS;
}

byte exec_sleep(byte *data)
{
    os_sleep(* (uint *) data); 

    return EXEC_SIGPASS;
}

byte exec_rxcpy(byte *data)
{
    SaltObject new_ = *xregister_find(* (uint *) data);
    new_.id = * (uint *) data;

    xregister_add(new_);

    return EXEC_SIGPASS;
}

byte exec_rxdel(byte *data)
{
    xregister_remove(* (uint *) data);

    return EXEC_SIGPASS;
}

byte exec_rxnew(byte *data)
{
    uint id     = * (uint *) data;
    byte const_ = *          (data + 4);
    byte type   = *          (data + 5);

    SaltObject obj = salt_object_create(id, type, PERM_USER, 0, NULL, 
                     NULL, 0, 0);

    void *val;
    if (type == SALT_STRING) {
        val = util_generate_data(type, data + 10); 
        memcpy(obj.typeinfo, data + 6, 4);
    }
    else {
        val = util_generate_data(type, data + 6);
    }

    obj.data = val;

    xregister_add(obj);

    return EXEC_SIGPASS;
}

byte exec_rxset(byte *data)
{
    uint id   = * (uint *) data;
    byte type = *          data + 8; 

    // TODO: other types
    int *val = vmalloc(sizeof(int), 1);

    SaltObject *obj = xregister_find(id);
    if (obj->type != type)
        return EXEC_SIGERR;

    vmfree(obj->data, util_get_size(obj));
    obj->data = val;

    return EXEC_SIGPASS;
}

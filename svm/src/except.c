/**
 * except.h implementation
 *
 * @author bellrise
 */
#include "../include/except.h"
#include <string.h>


void except_throw()
{
    printf("Exception occured : ");
    util_print_object(xregister_find(9));
    printf("\n\n ~ ");
    util_print_object(xregister_find(10));
    printf("\n");
    
    exit(1);
}

void except_set(const char *title, const char *msg)
{
    dprintf("except_set: lookup error registers\n");

    SaltObject *err_title = xregister_find(9);
    SaltObject *err_text  = xregister_find(10);

    vmrealloc(err_title->data, * (uint *) err_title->typeinfo, strlen(title));
    vmrealloc(err_text->data,  * (uint *) err_text->typeinfo,  strlen(msg));

    dprintf("except_set: copying memory\n");

    int title_l = (int) strlen(title);
    for (short i = 0; i < 4; i++)
        err_title->typeinfo[i] = ((char *) &title_l)[i];

    int msg_l = (int) strlen(msg);
    for (short i = 0; i < 4; i++)
        err_text->typeinfo[i] = ((char *) &msg_l)[i];

    strncpy(err_title->data, title, strlen(title));
    strncpy(err_text->data, msg, strlen(msg));
}

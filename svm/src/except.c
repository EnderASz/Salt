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

    salt_string_set(err_title, (char *) title);
    salt_string_set(err_text, (char *) msg);
}

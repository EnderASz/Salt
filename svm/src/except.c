/**
 * except.h implementation
 *
 * @author bellrise
 */
#include "../include/except.h"


void except_throw(char *title, char *msg)
{
    printf("Exception occured : ");
    util_print_object(xregister_find(9));
    printf("\n\n ~ ");
    util_print_object(xregister_find(10));
    printf("\n");
    
    exit(1);
}

/**
 * utils.h implementation
 *
 */
#include "../include/utils.h"
#include <stdlib.h>

/* Salt allocation function, which may be replaced later with some special 
 * functionality. This is used instead of the basic malloc or calloc.
 */
void* smalloc(size_t _size, uint _elem)
{
    return calloc(_size, _elem);
}

/* Print _n amount of bytes grouped into 2 bytes starting from the mptr_t byte
 * location. This is used for runtime memory debugging in the current given
 * point.
 */
void utils_hexdump(mptr_t byte, uint _n)
{
    uint rest = _n % 8;    
    uint rows = (_n - rest) / 8;

    for (uint i = 0; i < rows; i++) {
        printf("\033[90m%p  \033[0m", byte + (i * 8));
        
        for (uint k = 0; k < 8; k++) {
            printf("%02hhx", *(byte + (i * 8) + k));
            // Print space on group split
            if (k % 2 != 0) printf(" ");
        }
        printf("\033[90m ");
        utils_hexdump_c(byte + (i * 8), 8);
        printf("\033[0m\n");
    }

    // If no bytes are not 8 aligned, exit 
    if (!rest)
        return;
    
    printf("\033[90m%p  \033[0m", byte + (rows * 8));
    
    for (uint i = 0; i < rest; i++) {
        printf("%02hhx", *(byte + i + (rows * 8)));
        // Print space on group split
        if (i % 2 != 0) printf(" ");
    }
    printf("\n");
}

/* Print _n amount of bytes in ASCII form from the given memory pointer.
 * It only prints characters that are in the visual ASCII range, printing 
 * the rest as single spaces.
 */
void utils_hexdump_c(mptr_t byte, uint _n)
{
    char bv;
    for (uint i = 0; i < _n; i++) {
        bv = *(byte + i);
        
        if ((bv >= 0x20) && (bv <= 0x7e)) {
            printf("%c", bv);
        }
        else {
            printf(" ");
        }
    }
}

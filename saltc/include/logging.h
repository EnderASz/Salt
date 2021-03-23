/** logging.h is the header file contains macros for info, warnings, errors and
 * debug (at this moment only debug) print functions.
 */ 
#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef DEBUG
    #include <cstdio>

    #if defined(_WIN32)

        #include <windows.h>

        #define dprintf(...)                                                       \
        {                                                                          \
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);                         \
            SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (3 & 0x0F));         \
            printf("%s", __FILE__);                                                \
            SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (1 & 0x0F));         \
            printf("::");                                                          \
            SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (2 & 0x0F));         \
            printf("%s: ", __FUNCTION__);                                          \
            SetConsoleTextAttribute(hOut, ((0 & 0x0F) << 4) + (15 & 0x0F));        \
            printf(__VA_ARGS__);                                                   \
        }
    #elif defined(__linux__)

        #define dprintf(...)                                                       \
        {                                                                          \
            printf("\033[96m%s\033[34m::\033[92m%s: \033[0m",                      \
            __FILE__, __FUNCTION__);                                               \
            printf(__VA_ARGS__);                                                   \
        }
    #endif

#endif // DEBUG
#ifndef dprintf
    #define dprintf(...)
#endif

#endif // LOGGING_H_
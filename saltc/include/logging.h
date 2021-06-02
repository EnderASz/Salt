/**
 * logging.h is the header file contains macros for info, warnings, errors and
 * debug (at this moment only debug) print functions.
 */ 
#ifndef LOGGING_H_
#define LOGGING_H_

#if (defined(_WIN32) || defined(__linux__))
    #include <cstdio>
    #include <map>
    #include <utility>
    #include "utils.h"
    #include "error.h"

    #ifndef __FILENAME__
        #define __FILENAME__ __FILE__
    #endif

    #if defined(_WIN32)
        #include <winbase.h>
        #include <wincon.h>
        #undef ERROR
        
        namespace font_color
        {
            const uint BLACK = 0 & 0x0F;
            const uint BLUE = 1 & 0x0F;
            const uint GREEN = 2 & 0x0F;
            const uint CYAN = 3 & 0x0F;
            const uint RED = 4 & 0x0F;
            const uint MAGENTA = 5 & 0x0F;
            const uint BROWN = 6 & 0x0F;
            const uint LIGHT_GRAY = 7 & 0x0F;
            const uint DARK_GRAY = 8 & 0x0F;
            const uint LIGHT_BLUE = 9 & 0x0F;
            const uint LIGHT_GREEN = 10 & 0x0F;
            const uint LIGHT_CYAN = 11 & 0x0F;
            const uint LIGHT_RED = 12 & 0x0F;
            const uint LIGHT_MAGENTA = 13 & 0x0F;
            const uint YELLOW = 14 & 0x0F;
            const uint WHITE = 15 & 0x0F;
        };
        
        namespace bg_color
        {
            const uint BLACK = (0 & 0x0F) << 4;
            const uint BLUE = (1 & 0x0F) << 4;
            const uint GREEN = (2 & 0x0F) << 4;
            const uint CYAN = (3 & 0x0F) << 4;
            const uint RED = (4 & 0x0F) << 4;
            const uint MAGENTA = (5 & 0x0F) << 4;
            const uint BROWN = (6 & 0x0F) << 4;
            const uint LIGHT_GRAY = (7 & 0x0F) << 4;
            const uint DARK_GRAY = (8 & 0x0F) << 4;
            const uint LIGHT_BLUE = (9 & 0x0F) << 4;
            const uint LIGHT_GREEN = (10 & 0x0F) << 4;
            const uint LIGHT_CYAN = (11 & 0x0F) << 4;
            const uint LIGHT_RED = (12 & 0x0F) << 4;
            const uint LIGHT_MAGENTA = (13 & 0x0F) << 4;
            const uint YELLOW = (14 & 0x0F) << 4;
            const uint WHITE = (15 & 0x0F) << 4;
        };

        typedef std::pair<std::string, const uint> LogLevelPrefix;
    #elif defined(__linux__)

        #include <string>

        extern const char* RESET_ALL;           // [0m
        namespace font_styles {
            extern const char* BOLD;            // [1m
            extern const char* BLACK;           // [30m
            extern const char* RED;             // [31m
            extern const char* GREEN;           // [32m
            extern const char* YELLOW;          // [33m
            extern const char* BLUE;            // [34m
            extern const char* MAGENTA;         // [35m
            extern const char* CYAN;            // [36m
            extern const char* WHITE;           // [37m
            extern const char* GREY;            // [39m
            extern const char* LIGHT_BLACK;     // [90m
            extern const char* LIGHT_RED;       // [91m
            extern const char* LIGHT_GREEN;     // [92m
            extern const char* LIGHT_YELLOW;    // [93m
            extern const char* LIGHT_BLUE;      // [94m
            extern const char* LIGHT_MAGENTA;   // [95m
            extern const char* LIGHT_CYAN;      // [96m
            extern const char* LIGHT_WHITE;     // [97m
        };
        namespace bg_colors {
            extern const char* BLACK;           // [40m
            extern const char* RED;             // [41m
            extern const char* GREEN;           // [42m
            extern const char* YELLOW;          // [43m
            extern const char* BLUE;            // [44m
            extern const char* MAGENTA;         // [45m
            extern const char* CYAN;            // [46m
            extern const char* WHITE;           // [47m
            extern const char* LIGHT_BLACK;     // [100m
            extern const char* LIGHT_RED;       // [101m
            extern const char* LIGHT_GREEN;     // [102m
            extern const char* LIGHT_YELLOW;    // [103m
            extern const char* LIGHT_BLUE;      // [104m
            extern const char* LIGHT_MAGENTA;   // [105m
            extern const char* LIGHT_CYAN;      // [106m
            extern const char* LIGHT_WHITE;     // [107m
        };

        typedef std::pair<std::string, const char*> LogLevelPrefix;
    #endif

    void print_log_location(const char* file, const char* function);

    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };

    extern std::map<LogLevel, LogLevelPrefix> log_level_prefix;

    void print_log_prefix(LogLevel log_level);

    extern size_t print_max_right;

    void set_print_padding(size_t padding = 0);
    #define reset_print_padding() set_print_padding()

    #if defined(SALT_DEBUG)
        #define dprint(...)                                                   \
        {                                                                     \
            print_log_location(__FILENAME__, __FUNCTION__);                   \
            print_log_prefix(DEBUG);                                          \
            printf(__VA_ARGS__);                                              \
            printf("\n");                                                     \
        }
    #else
        #define dprint(...)
    #endif
    #define iprint(...)                                                       \
    {                                                                         \
        print_log_location(__FILENAME__, __FUNCTION__);                       \
        print_log_prefix(INFO);                                               \
        printf(__VA_ARGS__);                                                  \
        printf("\n");                                                         \
    }
    #define wprint(...)                                                       \
    {                                                                         \
        print_log_location(__FILENAME__, __FUNCTION__);                       \
        print_log_prefix(WARNING);                                            \
        printf(__VA_ARGS__);                                                  \
        printf("\n");                                                         \
    }
    void _eprint(salt::BaseError* error, const char* file, const char* location);
    #define eprint(ERR) _eprint(ERR, __FILENAME__, __FUNCTION__)
#else
    #define dprint(...)
    #define iprint(...)
    #define wprint(...)
    #define eprint(...)
#endif

#endif // LOGGING_H_
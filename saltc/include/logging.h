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

        const char* RESET_ALL = "\e[0m";
        namespace font_styles {
            const char* BOLD = "\e[1m";
            const char* BLACK = "\e[30m";
            const char* RED = "\e[31m";
            const char* GREEN = "\e[32m";
            const char* YELLOW = "\e[33m";
            const char* BLUE = "\e[34m";
            const char* MAGENTA = "\e[35m";
            const char* CYAN = "\e[36m";
            const char* WHITE = "\e[37m";
            const char* GREY = "\e[39m";
            const char* LIGHT_BLACK = "\e[90m";
            const char* LIGHT_RED = "\e[91m";
            const char* LIGHT_GREEN = "\e[92m";
            const char* LIGHT_YELLOW = "\e[93m";
            const char* LIGHT_BLUE = "\e[94m";
            const char* LIGHT_MAGENTA = "\e[95m";
            const char* LIGHT_CYAN = "\e[96m";
            const char* LIGHT_WHITE = "\e[97m";
        };
        namespace bg_colors {
            const char* BLACK = "\e[40m";
            const char* RED = "\e[41m";
            const char* GREEN = "\e[42m";
            const char* YELLOW = "\e[43m";
            const char* BLUE = "\e[44m";
            const char* MAGENTA = "\e[45m";
            const char* CYAN = "\e[46m";
            const char* WHITE = "\e[47m";
            const char* LIGHT_BLACK = "\e[100m";
            const char* LIGHT_RED = "\e[101m";
            const char* LIGHT_GREEN = "\e[102m";
            const char* LIGHT_YELLOW = "\e[103m";
            const char* LIGHT_BLUE = "\e[104m";
            const char* LIGHT_MAGENTA = "\e[105m";
            const char* LIGHT_CYAN = "\e[106m";
            const char* LIGHT_WHITE = "\e[107m";
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

    #if defined(SALT_DEBUG)
        #define dprint(...)                                                   \
        {                                                                     \
            print_log_location(__FILE__, __FUNCTION__);                       \
            print_log_prefix(DEBUG);                                          \
            printf(__VA_ARGS__);                                              \
            printf("\n");                                                     \
        }
    #else
        #define dprint(...)
    #endif
    #define iprint(...)                                                       \
    {                                                                         \
        print_log_location(__FILE__, __FUNCTION__);                           \
        print_log_prefix(INFO);                                               \
        printf(__VA_ARGS__);                                                  \
        printf("\n");                                                         \
    }
    #define wprint(...)                                                       \
    {                                                                         \
        print_log_location(__FILE__, __FUNCTION__);                           \
        print_log_prefix(WARNING);                                            \
        printf(__VA_ARGS__);                                                  \
        printf("\n");                                                         \
    }
    void _eprint(salt::BaseError* error, const char* file, const char* location);
    #define eprint(ERR) _eprint(ERR, __FILE__, __FUNCTION__)
#else
    #define dprint(...)
    #define iprint(...)
    #define wprint(...)
    #define eprint(...)
#endif

#endif // LOGGING_H_
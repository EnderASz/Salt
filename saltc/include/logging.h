/**
 * logging.h is the header file contains macros for info, warnings, errors and
 * debug (at this moment only debug) print functions.
 */ 
#ifndef LOGGING_H_
#define LOGGING_H_

#if (defined(_WIN32) || defined(__linux__))
    #include <map>
    #include <string>
    #include "utils.h"
    #include "error.h"

    using std::string;

    #ifndef __FILENAME__
        #define __FILENAME__ __FILE__
    #endif

    #if defined(_WIN32)
        #include <winbase.h>
        #include <wincon.h>
        #undef ERROR    // It's here because stupid WinAPI defineds it,
                        // like many others constants that I won't ever need,
                        // and it's breaking my code. Freaking Windows :/
        
        /**
         * Setups output styling for windows platform.
         */
        #define setup_windows_logging()                                       \
        {                                                                     \
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);                    \
            DWORD mode = 0;                                                   \
            GetConsoleMode(hOut, &mode);                                      \
            mode |= 4;                                                        \
            SetConsoleMode(hOut, mode);                                       \
        }
    #endif

    extern size_t print_max_right;
    void set_print_padding(size_t padding);
    #define reset_print_padding() set_print_padding(0)


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


    /**
     *      @tparam Value1
     *          String it's printing before log message.
     *      @tparam Value2
     *          const char* unix text styling for log level print.
     *          Mainly from `font_styles` or `bg_colors` namespace
     */
    typedef std::pair<string, const char*> LogLevelPrefix;
    enum LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    extern std::map<LogLevel, LogLevelPrefix> log_level_prefix;
    void print_log_prefix(LogLevel log_level);


    void print_log_location(const char* file, const char* function);


    #if defined(SALT_DEBUG)
        #define dprint(...)                                                   \
        {                                                                     \
            print_log_location(__FILENAME__, __FUNCTION__);                   \
            print_log_prefix(DEBUG);                                          \
            printf(__VA_ARGS__);                                              \
            printf("\n");                                                     \
        }
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
    void _eprint(
        salt::BaseError* error,
        const char* file,
        const char* location);
    #define eprint(ERR) _eprint(ERR, __FILENAME__, __FUNCTION__)
#endif

#ifndef setup_windows_logging
    #define setup_windows_logging() {}
#endif
#ifndef dprint
    #define dprint(...)
#endif
#ifndef iprint
    #define iprint(...)
#endif
#ifndef wprint
    #define wprint(...)
#endif
#ifndef eprint
    #define eprint(...)
#endif

#endif // LOGGING_H_
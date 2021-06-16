/**
 * logging.h is the header file contains macros for info, warnings, errors and
 * debug (at this moment only debug) print functions.
 *
 * @author EnderAsz
 */
#ifndef LOGGING_H_
#define LOGGING_H_

#include "utils.h"
#include "error.h"

#include <string>
#include <memory>
#include <map>

#ifndef __FILENAME__
    #define __FILENAME__ __FILE__
#endif

#if defined(_WIN32)
    #include <winbase.h>
    #include <wincon.h>
    #undef ERROR    // It's here because stupid WinAPI defines it,
                    // like many other constants that I won't ever need,
                    // and it's breaking my code. Freaking Windows :/

    // Set up output styling for Windows
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

const char* RESET_ALL           = "\e[0m";

namespace font_colors
{
    const char* BOLD            = "\e[1m";
    const char* BLACK           = "\e[30m";
    const char* RED             = "\e[31m";
    const char* GREEN           = "\e[32m";
    const char* YELLOW          = "\e[33m";
    const char* BLU             = "\e[34m";
    const char* MAGENTA         = "\e[35m";
    const char* CYAN            = "\e[36m";
    const char* WHITE           = "\e[37m";
    const char* GREY            = "\e[39m";
    const char* LIGHT_BLACK     = "\e[90m";
    const char* LIGHT_RED       = "\e[91m";
    const char* LIGHT_GREEN     = "\e[92m";
    const char* LIGHT_YELLOW    = "\e[93m";
    const char* LIGHT_BLUE      = "\e[94m";
    const char* LIGHT_MAGENTA   = "\e[95m";
    const char* LIGHT_CYAN      = "\e[96m";
    const char* LIGHT_WHITE     = "\e[97m";
};

namespace bg_colors
{
    const char* BLACK           = "\e[40m";
    const char* RED             = "\e[41m";
    const char* GREEN           = "\e[42m";
    const char* YELLOW          = "\e[43m";
    const char* BLUE            = "\e[44m";
    const char* MAGENTA         = "\e[45m";
    const char* CYAN            = "\e[46m";
    const char* WHITE           = "\e[47m";
    const char* LIGHT_BLACK     = "\e[100m";
    const char* LIGHT_RED       = "\e[101m";
    const char* LIGHT_GREEN     = "\e[102m";
    const char* LIGHT_YELLOW    = "\e[103m";
    const char* LIGHT_BLUE      = "\e[104m";
    const char* LIGHT_MAGENTA   = "\e[105m";
    const char* LIGHT_CYAN      = "\e[106m";
    const char* LIGHT_WHITE     = "\e[107m";
};


/**
 * @tparam Value1
 *         String it's printing before log message.
 * @tparam Value2
 *         const char* unix text styling for log level print.
 *         Mainly from `font_styles` or `bg_colors` namespace
 */
typedef std::pair<std::std::string, const char*> LogLevelPrefix;

enum LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

extern const std::map<LogLevel, LogLevelPrefix> log_level_prefix;
void print_log_prefix(LogLevel log_level);
void print_log_location(const char* file, const char* function);

/*
 * Debug functions.
 */

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
#define eprint(ERR_CLASS, ...)                                            \
{                                                                         \
    std::unique_ptr<salt::BaseError> error(                               \
        new salt::ERR_CLASS(__VA_ARGS__));                                \
    print_log_location(__FILENAME__, __FUNCTION__);                       \
    print_log_prefix(ERROR);                                              \
    printf(error->getMessage().c_str());                                  \
    printf("\n");                                                         \
    exit(EXIT_FAILURE);

#endif
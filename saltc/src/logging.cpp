/**
 * logging.h implementation
 *
 * @author EnderASz
 */
#include "../include/logging.h"

#if defined(_WIN32) || defined(__linux__)

#include <cstdlib>
#include <map>
#include <string>
#include "../include/error.h"

using std::string;

size_t print_max_right = 0;
void set_print_padding(size_t padding) {print_max_right = padding;}


const char* RESET_ALL =         "\e[0m";
namespace font_styles {
    const char* BOLD =          "\e[1m";
    const char* BLACK =         "\e[30m";
    const char* RED =           "\e[31m";
    const char* GREEN =         "\e[32m";
    const char* YELLOW =        "\e[33m";
    const char* BLUE =          "\e[34m";
    const char* MAGENTA =       "\e[35m";
    const char* CYAN =          "\e[36m";
    const char* WHITE =         "\e[37m";
    const char* GREY =          "\e[39m";
    const char* LIGHT_BLACK =   "\e[90m";
    const char* LIGHT_RED =     "\e[91m";
    const char* LIGHT_GREEN =   "\e[92m";
    const char* LIGHT_YELLOW =  "\e[93m";
    const char* LIGHT_BLUE =    "\e[94m";
    const char* LIGHT_MAGENTA = "\e[95m";
    const char* LIGHT_CYAN =    "\e[96m";
    const char* LIGHT_WHITE =   "\e[97m";
};
namespace bg_colors {
    const char* BLACK =         "\e[40m";
    const char* RED =           "\e[41m";
    const char* GREEN =         "\e[42m";
    const char* YELLOW =        "\e[43m";
    const char* BLUE =          "\e[44m";
    const char* MAGENTA =       "\e[45m";
    const char* CYAN =          "\e[46m";
    const char* WHITE =         "\e[47m";
    const char* LIGHT_BLACK =   "\e[100m";
    const char* LIGHT_RED =     "\e[101m";
    const char* LIGHT_GREEN =   "\e[102m";
    const char* LIGHT_YELLOW =  "\e[103m";
    const char* LIGHT_BLUE =    "\e[104m";
    const char* LIGHT_MAGENTA = "\e[105m";
    const char* LIGHT_CYAN =    "\e[106m";
    const char* LIGHT_WHITE =   "\e[107m";
};

const std::map<LogLevel, LogLevelPrefix> log_level_prefix {
    {DEBUG, {"Debug", font_styles::YELLOW}},
    {INFO, {"Info", font_styles::LIGHT_CYAN}},
    {WARNING, {"Warning", font_styles::MAGENTA}},
    {ERROR, {"Error", font_styles::LIGHT_RED}}
};

void print_log_prefix(LogLevel log_level) {
    LogLevelPrefix prefix = log_level_prefix.at(log_level);

    printf(
        "[%s%s%s] ",
        prefix.second,          // Prefix styling
        prefix.first.c_str(),   // Prefix string
        RESET_ALL
    );
}

void print_log_location(const char* file, const char* function) {
    printf(
        "%s%s%s::%s%s: ",
        font_styles::LIGHT_CYAN,
        file,
        font_styles::BLUE,
        function,
        RESET_ALL
    );
    size_t location_len = (string(file) + function).size();
    for(; location_len < print_max_right; location_len++) printf(" ");
    print_max_right = location_len;
}

#endif
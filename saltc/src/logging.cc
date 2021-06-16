/**
 * logging.h implementation
 *
 * @author EnderASz
 */
#include <saltc/logging.h>
#include <saltc/error.h>

#include <cstdlib>

static size_t print_max_right = 0;

void set_print_padding(size_t padding) {
    print_max_right = padding;
}

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
        prefix.second,
        prefix.first.c_str(),
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
    size_t location_len = (std::string(file) + function).size();
    for(; location_len < print_max_right; location_len++)
        printf(" ");
    print_max_right = location_len;
}

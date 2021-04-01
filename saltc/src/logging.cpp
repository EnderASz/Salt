/**
 * logging.h implementation
 *
 * @author EnderASz
 */
#include "../include/logging.h"

#if defined(_WIN32) || defined(__linux__)

void _eprint(salt::BaseError* error, const char* file, const char* location) {
    print_log_location(file, location);
    print_log_prefix(ERROR);
    printf(error->getMessage().c_str());
    delete error;
    printf("\n");
    exit(EXIT_FAILURE);   
}

#if defined(_WIN32)
    void print_log_location(const char* file, const char* function) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hOut, bg_color::BLACK + font_color::CYAN);
        printf("%s", file);
        SetConsoleTextAttribute(hOut, bg_color::BLACK + font_color::BLUE);
        printf("::"); 
        SetConsoleTextAttribute(hOut, bg_color::BLACK + font_color::GREEN);
        printf("%s: ", function);
        SetConsoleTextAttribute(
            hOut,
            bg_color::BLACK + font_color::LIGHT_GRAY);
    }

    void print_log_prefix(LogLevel log_level) {
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        LogLevelPrefix prefix = log_level_prefix.at(log_level);

        printf("[");
        SetConsoleTextAttribute(hOut, bg_color::BLACK + prefix.second);
        printf(prefix.first.c_str());
        SetConsoleTextAttribute(
            hOut,
            bg_color::BLACK + font_color::LIGHT_GRAY);
        printf("] ");
    }

    std::map<LogLevel, LogLevelPrefix> log_level_prefix = {
        {DEBUG, {"Debug", font_color::YELLOW}},
        {INFO, {"Info", font_color::LIGHT_CYAN}},
        {WARNING, {"Warning", font_color::MAGENTA}},
        {ERROR, {"Error", font_color::LIGHT_RED}}
    };
#elif defined(__linux__)
    void print_log_location(const char* file, const char* function) {
        printf(
            "%s%s%s::%s%s: ",
            font_styles::LIGHT_CYAN,
            file,
            font_styles::BLUE,
            function,
            RESET_ALL
        );
    }

    void print_log_prefix(LogLevel log_level) {
        LogLevelPrefix prefix = log_level_prefix.at(log_level);

        printf(
            "[%s%s%s]",
            prefix.second,
            prefix.first.c_str(),
            RESET_ALL
        );
    }

    std::map<LogLevel, LogLevelPrefix> log_level_prefix = {
        {DEBUG, {"Debug", font_styles::YELLOW}},
        {INFO, {"Info", font_styles::LIGHT_CYAN}},
        {WARNING, {"Warning", font_styles::MAGENTA}},
        {ERROR, {"Error", font_styles::LIGHT_RED}}
    };
#endif

#endif
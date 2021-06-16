/**
 * Program utilities, which are included in many files.
 *
 * @author EnderASz
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <stdint.h>

#include <string>
#include <queue>
#include <array>

// Type definitions
typedef unsigned int uint;
typedef char byte;

template<typename T, class Q = std::queue<T>>
T pop(Q& container) {
    T value = container.front();
    container.pop();
    return value;
}

std::string load_file(std::string filepath);

template<typename T, size_t N, class A = std::array<T, N>>
A ptr_to_array(T* data) {
    A array;
    std::copy(data, data+N, array.begin());
    return array;
}

/**
 * Stucture storage informations about position e.g in file.
 */
struct InStringPosition {
    size_t line_idx;
    size_t idx;
    size_t inline_idx;

    InStringPosition(std::string& str, std::string::iterator iterator);
    InStringPosition(size_t line_idx, size_t inline_idx, size_t idx);
};

/**
 * Parse octal number std::string into int.
 * This function parses only first 8 characters in a std::string.
 * If std::string is shorter than 8 characters it's stops parsing and
 * returns result on the end of std::string or when it encounters an inconsistent
 * character with the octal digit.
 *
 * Example results:
 * 54362 -> 22,770
 * 23 -> 19
 * 5 -> 5
 * 100000000 -> 2,097,152 instead 16,777,216
 * 81 -> 0
 * 158 -> 13
 * 15u -> 13
 */
uint parse_oct(std::string str);

/**
 * Parse hexadecimal number std::string into int.
 * This function parses only first 8 characters in a std::string.
 * If std::string is shorter than 8 characters it's stops parsing and
 * returns result on the end of std::string or when it encounters an inconsistent
 * character with the hexadecimal digit.
 *
 * Example results:
 * 54CD -> 21,709
 * 1A -> 26
 * F -> 15
 * a -> 10
 * 100000000 -> 268,435,456 instead 4,294,967,296
 * H -> 0
 * 13P -> 19
 * 15u -> 21
 */
uint parse_hex(std::string str);

/* Return true if chr parameter is octal digit character. */
bool isodigit(char chr);

/**
 * 
 */
bool isalpha(std::string str);

bool islower(std::string str);

bool isnum(std::string str);

bool isfnum(std::string str);

bool isalnum(std::string str);

bool isstropen(char chr);


#endif // UTILS_H_
/**
 * Program utilities, which are included in many files.
 *
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <queue>
#include <string>
#include <array>
#include <stdint.h>

using std::string;

// Type definitions
typedef unsigned int uint;
typedef char byte;

template<typename T, class Q = std::queue<T>>
T pop(Q& container) {
    T value = container.front();
    container.pop();
    return value;
}

string load_file(string filepath);

template<typename T, size_t N, class A = std::array<T, N>>
A ptr_to_array(T* data) {
    A array;
    std::copy(data, data+N, array.begin());
    return array;
}

/*Stucture storage informations about position e.g in file. */
struct InStringPosition {
    size_t line_idx;
    size_t idx;
    size_t inline_idx;
    InStringPosition(string& str, string::iterator iterator);
    InStringPosition(size_t line_idx, size_t inline_idx, size_t idx);
};

/**
 * Parse octal number string into int.
 * This function parses only first 8 characters in a string.
 * If string is shorter than 8 characters it's stops parsing and
 * returns result on the end of string or when it encounters an inconsistent
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
uint parse_oct(string str);

/**
 * Parse hexadecimal number string into int.
 * This function parses only first 8 characters in a string.
 * If string is shorter than 8 characters it's stops parsing and
 * returns result on the end of string or when it encounters an inconsistent
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
uint parse_hex(string str);

/* Return true if chr parameter is octal digit character. */
bool isodigit(char chr);

bool isalpha(string str);

bool islower(string str);

bool isnum(string str);

bool isfnum(string str);

bool isalnum(string str);

bool isstropen(char chr);


#endif // UTILS_H_
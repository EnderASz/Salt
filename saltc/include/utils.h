/**
 * Program utilities, which are included in many files.
 *
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <queue>
#include <string>
#include <array>

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

#endif // UTILS_H_
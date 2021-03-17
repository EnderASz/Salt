/**
 * Program utilities, which are included in many files.
 *
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <queue>
#include <string>
#include <array>
#include <vector>

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

#endif // UTILS_H_
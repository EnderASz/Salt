/**
 * Program utilities, which are included in many files.
 *
 */
#ifndef UTILS_H_
#define UTILS_H_

#include <queue>
#include <string>

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

#endif // UTILS_H_
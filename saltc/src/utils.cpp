/**
 * utils.h implementation
 *
 */
#include "../include/utils.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

using std::string;

string load_file(string filepath) {
    std::ifstream file(filepath.c_str());
    if(!file.good()) {
        // TODO: Throw opening init file exception   
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    file.close();
    return buffer.str();
}

InStringPosition::InStringPosition(string& str, string::iterator iterator)
    :line_idx(std::count(str.begin(), iterator, '\n')),
    idx(std::distance(str.begin(), iterator)),
    inline_idx(idx - str.find_last_of("\n", idx) - 1) {}
InStringPosition::InStringPosition(
    size_t line_idx,
    size_t inline_idx,
    size_t idx)
        :line_idx(line_idx),
        idx(idx),
        inline_idx(inline_idx) {}


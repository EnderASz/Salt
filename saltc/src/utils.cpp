/**
 * utils.h implementation
 *
 */
#include "../include/utils.h"

#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <vector>
#include "../include/logging.h"
#include "../include/error.h"

using std::string;

string load_file(string filepath) {
    std::ifstream file(filepath.c_str());
    if(!file.good()) {
        eprint(new salt::FileOpenError("filepath")); 
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

uint parse_oct(string str) {
    std::vector<uint> oct;
    for(uint i = 0; i < (str.size() < 8 ? str.size() : 8); i++) {
        if(!isodigit(str[i])) break;
        oct.push_back(str[i] - 48);
    }
    uint result = 0;
    for(uint i = 0; i < oct.size(); i++)
        result += oct[oct.size() - 1 - i] * pow(8, i);
    return result;
}

uint parse_hex(string str) {
    std::vector<uint> hex;
    for(uint i = 0; i < (str.size() < 8 ? str.size() : 8); i++) {
        if(!isxdigit(str[i])) break;
        if(isdigit(str[i])) hex.push_back(str[i] - 48);
        else hex.push_back((str[i] - 55) % 32);
    }
    uint result = 0;
    for(uint i = 0; i < hex.size(); i++)
        result += hex[hex.size() - 1 - i] * pow(16, i);
    return result;
}

bool isodigit(char chr) {return chr >= '0' && chr < '8';}

bool isalpha(const string str) {
    for(size_t i = 0; i < str.size(); i++) if(!isalpha(str[i])) return false;
    return true;
}

bool islower(string str) {
    for(size_t i = 0; i < str.size(); i++) if(islower(str[i])) return false;
    return true;
}

bool isnum(const string str) {
    for(size_t i = 0; i < str.size(); i++) if(!(isdigit(str[i]))) return false;
    return true;
}
bool isfnum(const string str) {
    for(size_t i = 0; i < str.size(); i++)
        if(!(isdigit(str[i]) || (str[i] == '.' && i && i != str.size()-1)))
            return false;
    return true;
}
bool isalnum(const string str) {
    for(size_t i = 0; i < str.size(); i++) if(!isalnum(str[i])) return false;
    return true;
}
bool isstropen(char chr) {
    return chr == '\'' || chr == '"';
}
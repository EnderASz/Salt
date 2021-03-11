/**
 * utils.h implementation
 *
 */
#include "../include/utils.h"

#include <fstream>
#include <sstream>
#include <string>

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
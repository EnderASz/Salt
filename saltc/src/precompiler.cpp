/**
 * precompiler.h implementation
 * 
 * @author EnderASz
 */
#include "../include/precompiler.h"

#include "../include/utils.h"
#include <string>
#include <algorithm>

using std::string;

namespace salt::precompiler
{

/**
 * Paste init.salt from standard library to the code given by reference.
 * This function modifies code "in-place".
 */
void init_paste(string& code) {code.insert(0, load_file("lib/init.salt"));}

/**
 * Clean comments from code given by reference.
 * This function modifies code "in-place".
 */
void clean_comments(string& code) {
    while(true) {
        size_t start_index = code.find("#[");
        if(start_index != string::npos) {
            size_t end_index = code.find("]#", start_index+2);
            if(end_index == string::npos) {
                size_t line = std::count(
                    code.begin(),
                    code.begin()+start_index,
                    '\n');
                size_t line_start = code.find_last_of('\n', end_index)+1;
                size_t char_in_line = start_index - line_start + 2;
                // // TODO: Throw exception: Unclosed comment error at line <line> at character <start_index>
                break;
            }
            code.erase(start_index, end_index-start_index+2);
        } else break;
    }
    while(true) {
        size_t start_index = code.find('#');
        if(start_index != string::npos) {
            size_t end_index = code.find('\n', start_index+1);
            if(end_index == string::npos) end_index = code.size()-1;
            code.erase(start_index, end_index-start_index+1);
        } else break;
    }
}

} // salt::core::precompiler
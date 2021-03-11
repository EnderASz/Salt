/**
 * tokenizer.h implementation
 *
 */
#include "../include/meta.h"
#include "../include/tokenizer.h"

#include <vector>

using std::string;

namespace salt::tokenizer
{

/**
 * Tokenzizer constructor. A new instance is supposed to be created for
 * every new string that should be parsed. After initializing the object,
 * you can then call Tokenizer::render().
 */
Tokenizer::Tokenizer(string content) {
    this->original = content;
}

/**
 * Returns the array of tokens in a list object. The list object is just 
 * there so the length of the list can be easily returned. Internally it 
 * uses a std::vector<Token> or a Token*, depending on the implementation.
 */
std::vector<Token> Tokenizer::render() {
    std::vector<Token> tokens = {token_create(META_INIT)};
    
    return tokens;
}

} // salt::tokenizer
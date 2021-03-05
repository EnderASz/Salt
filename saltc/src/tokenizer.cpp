/**
 * tokenizer.h implementation
 *
 */
#include "../include/tokenizer.h"

using std::string;

namespace salt
{

/* Tokenzizer constructor. A new instance is supposed to be created for
 * every new string that should be parsed. After initializing the object,
 * you can then call Tokenizer::render().
 */
Tokenizer::Tokenizer(string content)
{
    this->original = content;
}

/* Returns the array of tokens in a list object. The list object is just 
 * there so the length of the list can be easily returned. Internally it 
 * uses a std::vector<Token> or a Token*, depending on the implementation.
 */
std::list<Token> Tokenizer::render()
{
    // Initial code, only temporary
    std::list<Token> list(1);
    Token tok = token_create(nullptr, TOK_0);
    list.assign(0, tok);
    return list;
}

} // salt
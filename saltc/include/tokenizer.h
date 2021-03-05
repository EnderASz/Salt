/**
 * This is the tokenizer class, which is a contained class for parsing and
 * tokenizing any given string.
 */
#ifndef TOKENZIER_H_
#define TOKENZIER_H_

#include "../include/utils.h"
#include "../include/token.h"
#include <string>
#include <list>

using std::string;

namespace salt
{

/* The tokenizer class is a contained class which parses the the passed string
 * (the file contents).
 */
class Tokenizer
{
public:
    /* Tokenzizer constructor. A new instance is supposed to be created for
     * every new string that should be parsed. After initializing the object,
     * you can then call Tokenizer::render().
     */
    Tokenizer(string content);
    
    /* Returns the array of tokens in a list object. The list object is just 
     * there so the length of the list can be easily returned. Internally it 
     * uses a std::vector<Token> or a Token*, depending on the implementation.
     */
    std::list<Token> render();

private:
    /* Original string from the constructor.
     */
    string original;
};


} // salt

#endif // TOKENIZER_H_
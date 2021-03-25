/**
 * This is the tokenizer class, which is a contained class for parsing and
 * tokenizing any given string.
 * This also includes all header files from 'tokenizer' directory.
 */
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "tokenizer/token.h"
#include "source_file.h"
#include <string>
#include <vector>

using std::string;

namespace salt::tokenizer
{

/**
 * The tokenizer class is a contained class which parses the the passed string
 * (the file contents).
 */
class Tokenizer
{
private:
    /* Source file from the constructor. */
    SourceFile* source;

    std::vector<Token> tokens;
    
    string::iterator current;
    struct {
        InStringPosition position;
        string str;
    } curr_token = {{0, 0, 0}, ""};
    void skipComment();

    void skipBlank();

    void jumpToEnd();

    string::iterator sourceBegin() const;

    string::iterator sourceEnd() const;

    string::iterator sourceLast() const;

    string::iterator findFirst(string value) const;

    bool isCommentChar() const;
    bool isCommentChar(string::iterator iterator) const;
    bool isCommentChar(const char chr) const;

    bool isLastChar() const;
    bool isLastChar(string::iterator iterator) const;

    bool isInRange() const;
    bool isInRange(string::iterator iterator) const;

    /**
     * Returns true if current is not last character and next character is
     * decimal digit.
     */
    bool nextIsDigit() const;

    /**
     * Returns true if current is not last character and next character is
     * hexadecimal digit.
     */
    bool nextIsXDigit() const;

    /**
     * Returns numeric literal token (TOKL_INT or TOKL_FLOAT) or null_token
     * if token on current position is not numeric literal
     */
    Token getNumToken();

    size_t getIdx() const;
    size_t getIdx(string::iterator iterator) const;

public:
    /**
     * Tokenzizer constructor. A new instance is supposed to be created for
     * every new string that should be parsed. After initializing the object,
     * you can then call Tokenizer::render().
     */
    Tokenizer(SourceFile& source_file);
    
    /**
     * Returns the array of tokens in a list object. The list object is just 
     * there so the length of the list can be easily returned. Internally it 
     * uses a std::vector<Token> or a Token*, depending on the implementation.
     */
    std::vector<Token> render();

}; //salt::tokenizer::Tokenizer


} // salt::tokenizer

#endif // TOKENIZER_H_
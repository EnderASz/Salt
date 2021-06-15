/**
 * This is the tokenizer class, which is a contained class for parsing and
 * tokenizing any given string.
 * This also includes all header files from 'tokenizer' directory.
 */
#ifndef TOKENIZER_H_
#define TOKENIZER_H_

#include "token.h"
#include "source_file.h"
#include <string>
#include <vector>
#include <functional>

using std::string;

namespace salt
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

    Token parseToken(const TokenType& type = TOK_0);

    bool skipComment();

    bool skipBlank();

    void jumpToEnd();

    void jumpTo(const size_t& idx);

    /**
     * Moves current iterator by 'move' value and returns true if current is
     * in range, otherwise false.
     */
    bool moveCurrent(const size_t& move = 1);

    /**
     * Pushes current iterator character to curr_token, moves iterator
     * forward and returns pushed character.
     */
    char pushCurrentCharacter();

    /* Returns position of current iterator as InStringPosition instance */
    InStringPosition getCurrentPosition();

    void resetCurrentToken();

    string::iterator sourceBegin() const;

    string::iterator sourceEnd() const;

    string::iterator sourceLast() const;

    /**
     * Return amount of characters left in source without current and
     * termination characters.
     */
    size_t leftToEnd() const;

    string::iterator findFirst(const string& value) const;

    bool isTokenBreakChar(char chr) const;

    bool isCommentChar() const;
    bool isCommentChar(const string::iterator& iterator) const;
    bool isCommentChar(const char& chr) const;

    bool isLastChar() const;
    bool isLastChar(const string::iterator& iterator) const;

    bool isInRange() const;
    bool isInRange(const string::iterator& iterator) const;

    /**
     * Returns true if character on current iterator is
     * equal to passed argument
     */
    bool isChar(const char& character);

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

    Token getStringLiteral();

    /**
     * Returns numeric literal token (TOKL_INT or TOKL_FLOAT) or null_token
     * if token on current position is not numeric literal.
     */
    Token getNumLiteral();

    enum NumLiteralType {
        NONE,
        DEC,
        OCT,
        HEX,
        DEC_FLOAT
    } current_num_literal = NONE;

    /**
     * Pushes decimal/decimal float/octal/hexadecimal digit from current
     * iterator to curr_token.str and returns null_token.
     * If encountered the end of the number only returns number literal
     * token (TOKL_INT or TOKL_FLOAT).
     *
     * Important! Call only when current iterator is in range.
     * It throws an error if current iterator is out of range or
     * current_num_literal value does not match called function.
     */
    Token pushDecDigit();
    Token pushFloatDecDigit();
    Token pushOctDigit(
                       size_t* digits_counter = nullptr,
                       const size_t& max_digits = 1);
    Token pushHexDigit(
                       size_t* digits_counter = nullptr,
                       const size_t& max_digits = 1);

    /** 
     * Returns a token based on static lowercase alpha word or
     * a name token starts with any alpha character or underscore and
     * consisting of alpha, underscores and/or digits characters.
     */
    Token getWordToken();

    Token getSymbolToken();

    /* Returns index of current iterator in source code */
    size_t getIdx() const;
    /* Returns index of iterator in source code */
    size_t getIdx(const string::iterator& iterator) const;

    std::array<std::function<Token(void)>, 4> token_getters = {
        std::bind(&Tokenizer::getStringLiteral, this),
        std::bind(&Tokenizer::getNumLiteral, this),
        std::bind(&Tokenizer::getWordToken, this),
        std::bind(&Tokenizer::getSymbolToken, this),
    };

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
    const std::vector<Token>& render();

    std::vector<Token> getTokens();

}; //salt::Tokenizer


} // salt

#endif // TOKENIZER_H_
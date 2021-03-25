/**
 * tokenizer.h implementation
 *
 */
#include "../include/tokenizer.h"

#include "../include/source_file.h"
#include "../include/utils.h"
#include "../include/errors/unclosed_comment_error.h"
#include <vector>
#include <cstring>
#include <algorithm>

using std::string;
using std::find;
using std::distance;

namespace salt::tokenizer
{

/**
 * Tokenzizer constructor. A new instance is supposed to be created for
 * source file instance. After initializing the object,
 * you can then call Tokenizer::render().
 */
Tokenizer::Tokenizer(SourceFile& source_file): source(&source_file) {}

/**
 * Returns the array of tokens in a list object. The list object is just 
 * there so the length of the list can be easily returned. Internally it 
 * uses a std::vector<Token> or a Token*, depending on the implementation.
 */
std::vector<Token> Tokenizer::render() {
    current = sourceBegin();

    while(isInRange(current)) {
        skipComment();
        skipBlank();
        if(!isInRange()) break;
        if(isLastChar()) {
            // Do stuff only for last char
            break;
        }
        // Do stuff only for character from begin to last-1
    }
    
    return tokens;
}

void Tokenizer::skipComment() {
    while(isInRange() && isCommentChar()) {
        if(isLastChar()) jumpToEnd();
        if(*(current+1) == '[') {
            string::iterator close = findFirst("]#");
            if(!isInRange(close))
                throw errors::UnclosedCommentError(
                    InStringPosition(source->code, current),
                    *source);
            current = close+2;
        }
        else current = findFirst("\n")+1;
    }
}

void Tokenizer::skipBlank(){
     while(isInRange() && isblank(*current)) current += 1;
}

void Tokenizer::jumpToEnd() {current = sourceEnd();}

string::iterator Tokenizer::sourceBegin() const {return source->code.begin();}

string::iterator Tokenizer::sourceEnd() const {return source->code.end();}

string::iterator Tokenizer::sourceLast() const {return source->code.end()-1;}

string::iterator Tokenizer::findFirst(string value) const {
    size_t current_idx = getIdx();
    size_t found = source->code.find(
        value.c_str(),
        current_idx,
        leftToEnd()+1
    );
    if(found == string::npos) return sourceEnd();
    return sourceBegin()+found;
}

bool Tokenizer::isCommentChar() const {return isCommentChar(current);}
bool Tokenizer::isCommentChar(string::iterator iterator) const {
    return isCommentChar(*iterator);
}
bool Tokenizer::isCommentChar(const char chr) const {return chr == '#';}

bool Tokenizer::isLastChar() const {return isLastChar(current);}
bool Tokenizer::isLastChar(string::iterator iterator) const {
    return distance(iterator, sourceEnd()-1) == 0;
}

bool Tokenizer::isInRange() const {return isInRange(current);}
bool Tokenizer::isInRange(string::iterator iterator) const {
    return distance(iterator, sourceEnd()-1) >= 0;
}

bool Tokenizer::nextIsDigit() const {
    return !isLastChar() && isdigit(*(current+1));
}

bool Tokenizer::nextIsXDigit() const {
    return !isLastChar() && isxdigit(*(current+1));
}

Token Tokenizer::getNumToken() {
    curr_token.position = InStringPosition(source->code, current);
    curr_token.str = "";
    if(!isInRange()) throw "#TODO: Tried get token after source last char.";
    if(!isdigit(*current)) return null_token;

    size_t non_decimal_digits = 0;
    enum {
        DEC,
        OCT,
        HEX,
        DEC_FLOAT
    } num_literal_type = DEC;
    if(*current == '0') {
        curr_token.str.push_back(*current);
        if(isLastChar())
            return token_create(TOKL_INT, curr_token.position, curr_token.str);
        current++;
        if(*current == 'x' || *current == 'X') {
            if(!nextIsXDigit()) throw "#TODO: Invalid hexadecimal literal.";
            num_literal_type = HEX;
        }
        else {
            if(!isodigit(*current)) throw "#TODO: Invalid octal literal.";
            num_literal_type = OCT;
        }
    }

    while(true)
        curr_token.str.push_back(*current);
        current++;
        if(!isInRange()) 
            return token_create(
                num_literal_type == DEC_FLOAT ? TOKL_FLOAT : TOKL_INT,
                curr_token.position,
                curr_token.str);
        if(num_literal_type == DEC) {
            if(*current == '.') {
                if(!nextIsDigit())
                    return token_create(
                        TOKL_INT,
                        curr_token.position,
                        curr_token.str);
                curr_token.str.push_back(*current);
                current++;
                num_literal_type == DEC_FLOAT;
            }
            if(!isblank(*current))
                return token_create(
                        TOKL_INT,
                        curr_token.position,
                        curr_token.str);
            if(!isdigit(*current))
                throw "#TODO: Invalid decimal literal.";
            curr_token.str.push_back(*current);
        }
        if(num_literal_type == DEC_FLOAT) {
            if(*current == '.')
                return token_create(
                    TOKL_FLOAT,
                    curr_token.position,
                    curr_token.str);
            if(!isblank(*current))
                return token_create(
                    TOKL_FLOAT,
                    curr_token.position,
                    curr_token.str);
            if(!isdigit(*current))
                throw "#TODO: Invalid decimal float literal.";
            curr_token.str.push_back(*current);
        }
        if(num_literal_type == OCT) {
            if(!isblank(*current))
                return token_create(
                        TOKL_INT,
                        curr_token.position,
                        curr_token.str);
            if(!isodigit(*current))
                throw "#TODO: Invalid octal literal.";
            if(non_decimal_digits > 8)
                throw "#TODO: Too long octal literal";
        }
        if(num_literal_type == HEX) {
            if(!isblank(*current))
                return token_create(
                        TOKL_INT,
                        curr_token.position,
                        curr_token.str);
            if(!isxdigit(*current))
                throw "#TODO: Invalid hexadecimal literal.";
            if(non_decimal_digits > 8)
                throw "#TODO: Too long hexadecimal literal";
        }
    }
}
} // salt::tokenizer
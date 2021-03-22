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
    return find(current, sourceEnd(), value);
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

} // salt::tokenizer
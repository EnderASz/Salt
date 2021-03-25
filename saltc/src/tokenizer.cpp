/**
 * tokenizer.h implementation
 *
 */
#include "../include/tokenizer.h"

#include "../include/source_file.h"
#include "../include/utils.h"
#include "../include/errors/unclosed_comment_error.h"
#include "../include/tokenizer/special_char.h"

#include <vector>
#include <cstring>
#include <algorithm>
#include <cctype>

using std::string;
using std::find;
using std::distance;
using std::min;

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
        Token parsed_token = null_token;
        while(skipComment() || skipBlank()) {}
        if(!isInRange()) break;
        if(!isLastChar())
            parsed_token = getNonOperativeToken();
            if(!parsed_token.isNothing()) {
                tokens.push_back(parsed_token);
                continue;
            }
            parsed_token = getOperativeToken();
            if(parsed_token.isNothing())
                throw "#TODO: Unrecognized token error";
            tokens.push_back(parsed_token);
        // Do stuff for every character
        if(isLastChar()) {
            // Do stuff only for last char
            break;
        }
        // Do stuff only for character from begin to last-1
    }
    parseToken();
    
    return tokens;
}

bool Tokenizer::skipComment() {
    bool skipped = false;
    while(isInRange() && isCommentChar()) {
        skipped = true;
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
    return skipped;
}

bool Tokenizer::skipBlank(){
    bool skipped = false;
    while(isInRange() && isblank(*current)) {
        skipped = true;
        current += 1;
    }
    return skipped;
}

void Tokenizer::jumpToEnd() {current = sourceEnd();}

void Tokenizer::jumpTo(size_t idx) {current = sourceBegin()+idx;}

string::iterator Tokenizer::sourceBegin() const {return source->code.begin();}

string::iterator Tokenizer::sourceEnd() const {return source->code.end();}

string::iterator Tokenizer::sourceLast() const {return source->code.end()-1;}

size_t Tokenizer::leftToEnd() const {
    return distance(current, sourceEnd())-1;
}

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

char Tokenizer::getSpecialCharacter() {
    auto found = special_chars.find(*current);
    if(found!=special_chars.end()) return found->second;

    uint (*f_parser)(std::string) = nullptr;
    if(*current == '0') f_parser = parse_oct;
    else if(*current == 'x') f_parser = parse_hex;
    else throw "#TODO: Tried to escape an unescapable character.";

    int to_proceed = min((int) leftToEnd()-1, 2);
    current++;
    // Close should be next char
    if(!to_proceed) {
        return 0;
    }
    // Current char should be close
    if(to_proceed == -1) throw "#TODO: Unclosed String Error";

    char result = static_cast<char>(
        f_parser(string(current, current+to_proceed)));
    current += to_proceed;
    return result;
}

Token Tokenizer::getStringLiteral(bool raw /*=false*/) {
    curr_token.position = InStringPosition(source->code, current);
    char open = *current;
    string value;
    if(!(open == '"' || open == '\'')) return null_token;
    current++;
    bool closed = false;
    while(isInRange()) {
        if(*current == open) {
            closed = true;
            break;
        }
        if(isLastChar()) break;
        if(*current == '\\') {
            current++;
            if(isLastChar()) break;
            value.push_back(getSpecialCharacter());
            continue;
        }
        value.push_back(*current);
        current++;
    }
    if(!closed) throw "#TODO: Unclosed String Error";
    return token_create(TOKL_STRING, curr_token.position, value);
}

Token Tokenizer::getNonOperativeToken() {
    Token parsed_token = null_token;
    if(isdigit(*current)) parsed_token = getNumToken();
    if(parsed_token.isNothing() && islower(*current)) {
        jumpTo(curr_token.position.idx);
        
    }


    while(isInRange()) {
        if(isdigit(*current) && word.empty()) {
            
        }




        if(!islower(*current)) {
            auto found = no_value_token_types.find(word);
            if(found!=no_value_token_types.end())
                return token_create(found->second, curr_token.position, word);
            if (isdigit(*current) && !word.empty())
            while(isInRange()) {
                if(isalnum(*current) || *current == '_') {}
            }
            
            current = sourceBegin() + curr_token.position.idx;
            return null_token;
        }
        curr_token.str.push_back(*current);
        current++;
        
        
        current++;
    }
    if(!isInRange()) throw "#TODO: Expected ';' token.";
    
    
    // if(isalpha(raw.str) && islower(raw.str)) {
    //     auto found = no_value_token_types.find(raw.str);
    //     if(found!=no_value_token_types.end())
    //         return token_create(found->second, raw.position, raw.str);
    // }
    // return null_token;
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

    while(true) {
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

size_t Tokenizer::getIdx() const {return getIdx(current);}
size_t Tokenizer::getIdx(string::iterator iterator) const {
    return distance(sourceBegin(), iterator);
}

void Tokenizer::parseToken() {
    if(curr_token.str.empty()) return;
    string token_str = curr_token.str;
    Token token = null_token;
    curr_token.str = "";

    tokens.push_back(token);
}

} // salt::tokenizer
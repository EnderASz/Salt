/**
 * tokenizer.h implementation
 *
 */
#include "../include/tokenizer.h"

#include "../include/source_file.h"
#include "../include/utils.h"
#include "../include/special_char.h"
#include "../include/logging.h"
#include "../include/error.h"

#include <vector>
#include <cstring>
#include <algorithm>
#include <cctype>
#include <map>
#include <array>
#include <functional>

using std::string;
using std::find;
using std::distance;
using std::min;

namespace salt
{

/**
 * Tokenzizer constructor. A new instance is supposed to be created for
 * source file instance. After initializing the object,
 * you can then call Tokenizer::render().
 */
Tokenizer::Tokenizer(SourceFile& source_file): source(&source_file) {
    render();
}

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

        curr_token.position = InStringPosition(source->code, current);
        
        for(auto i = token_getters.begin(); i!=token_getters.end(); i++) {
            resetCurrentToken();
            parsed_token = (*i)();
            if(!parsed_token.isNothing()) break;
        }
        if(parsed_token.isNothing())
            eprint(new UnknownTokenError(
                curr_token.position,
                *source,
                curr_token.str));
        tokens.push_back(parsed_token);
        dprint(
            "%s parsed to %s token",
            tokens.rbegin()->value.c_str(),
            token_names[tokens.rbegin()->type].c_str());
    }
    
    return tokens;
}

std::vector<Token> Tokenizer::getTokens() {
    return tokens;
}

Token Tokenizer::parseToken(TokenType type) {
    if(type == TOK_0) return null_token;
    return token_create(type, curr_token.position, curr_token.str);
}

bool Tokenizer::skipComment() {
    bool skipped = false;
    while(isInRange() && isCommentChar()) {
        skipped = true;
        if(isLastChar()) jumpToEnd();
        if(*(current+1) == '[') {
            string::iterator close = findFirst("]#");
            if(!isInRange(close)) eprint(new UnclosedCommentError(
                InStringPosition(source->code, current),
                *source));
            current = close+2;
        }
        else current = findFirst("\n")+1;
        dprint("Comment skipped");
    }
    return skipped;
}

bool Tokenizer::skipBlank(){
    bool skipped = false;
    while(isInRange() && isspace(*current)) {
        if(!skipped) dprint("Blank space skipped");
        skipped = true;
        current += 1;
    }
    return skipped;
}

void Tokenizer::jumpToEnd() {current = sourceEnd();}

void Tokenizer::jumpTo(size_t idx) {current = sourceBegin()+idx;}

/* move default: 1 */
bool Tokenizer::moveCurrent(size_t move) {
    current += move;
    return isInRange();
}

char Tokenizer::pushCurrentCharacter() {
    curr_token.str.push_back(*current);
    char pushed = *current;
    moveCurrent();
    return pushed;
}

InStringPosition Tokenizer::getCurrentPosition() {
    return InStringPosition(source->code, current);
}

void Tokenizer::resetCurrentToken() {
    curr_token.str = "";
}

string::iterator Tokenizer::sourceBegin() const {return source->code.begin();}

string::iterator Tokenizer::sourceEnd() const {return source->code.end();}

string::iterator Tokenizer::sourceLast() const {return source->code.end()-1;}

size_t Tokenizer::leftToEnd() const {
    return distance(current, sourceEnd())-1;
}

string::iterator Tokenizer::findFirst(string value) const {
    size_t current_idx = getIdx();
    size_t found = source->code.find(value, current_idx);
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

bool Tokenizer::isChar(char character) {return *current == character;}

bool Tokenizer::nextIsDigit() const {
    return !isLastChar() && isdigit(*(current+1));
}

bool Tokenizer::nextIsXDigit() const {
    return !isLastChar() && isxdigit(*(current+1));
}

Token Tokenizer::getStringLiteral() {
    if(!isInRange()) eprint(new OutOfSourceRangeError(*source));
    resetCurrentToken();
    if(isChar('r')) {
        if(isLastChar()) return null_token;
        pushCurrentCharacter();
    }
    char open = *current;
    if(!isstropen(open)) return null_token;
    pushCurrentCharacter();
    bool closed = false;
    while(isInRange()) {
        if(*current == open) {
            closed = true;
            pushCurrentCharacter();
            break;
        }
        if(isLastChar()) break;
        if(isChar('\\'))
            pushCurrentCharacter();
        pushCurrentCharacter();
    }
    if(!closed) eprint(new UnclosedStringError(curr_token.position, *source));
    return token_create(TOKL_STRING, curr_token.position, curr_token.str);
}

Token Tokenizer::getNumLiteral() {
    if(!isInRange()) eprint(new OutOfSourceRangeError(*source));
    if(!isdigit(*current)) return null_token;
    resetCurrentToken();

    size_t non_decimal_digits = 0;
    current_num_literal = DEC;
    if(isChar('0')) {
        pushCurrentCharacter();
        if(!isInRange())
            return token_create(TOKL_INT, curr_token.position, curr_token.str);
        if(isChar('x') || isChar('X')) {
            if(!nextIsXDigit())
                eprint(new InvalidLiteralError(
                    curr_token.position,
                    *source,
                    "hexadecimal number"));
            pushCurrentCharacter();
            current_num_literal = HEX;
        } else {
            if(!isodigit(*current))
                return token_create(
                    TOKL_INT,
                    curr_token.position,
                    curr_token.str);
            current_num_literal = OCT;
        }
        pushCurrentCharacter();
        non_decimal_digits++;
    }

    Token getted_token = null_token;
    while(getted_token.isNothing()) {
        if(!isInRange()) 
            return token_create(
                current_num_literal == DEC_FLOAT ? TOKL_FLOAT : TOKL_INT,
                curr_token.position,
                curr_token.str);
        switch(current_num_literal) {
            case DEC:
                getted_token = pushDecDigit();
                break;
            case DEC_FLOAT:
                getted_token = pushFloatDecDigit();
                break;
            case OCT:
                getted_token = pushOctDigit(&non_decimal_digits, 8);
                break;
            case HEX:
                getted_token = pushHexDigit(&non_decimal_digits, 8);
                break;
            default: break;
        }
    }
    current_num_literal = NONE;
    return getted_token;
}

Token Tokenizer::pushDecDigit() {
    if(!isInRange() || current_num_literal != DEC)
        throw "#TODO: Method pushDecDigit can not be called now.";
    if(isChar('.')) {
        if(!nextIsDigit())
            return token_create(
                TOKL_INT,
                curr_token.position,
                curr_token.str);
        current_num_literal = DEC_FLOAT;
    }
    else if(!isdigit(*current)) {
        if(curr_token.str.empty())
            eprint(new InvalidLiteralError(
                curr_token.position,
                *source,
                "decimal number"));
        return token_create(
                TOKL_INT,
                curr_token.position,
                curr_token.str);
    }
    pushCurrentCharacter();
    return null_token;
}

Token Tokenizer::pushFloatDecDigit() {
    if(!isInRange() || current_num_literal != DEC_FLOAT)
        throw "#TODO: Method pushFloatDecDigit can not be called now.";
    if(!isdigit(*current)) {
        if(curr_token.str.empty())
            eprint(new InvalidLiteralError(
                curr_token.position,
                *source,
                "decimal float number"));
        return token_create(
            TOKL_FLOAT,
            curr_token.position,
            curr_token.str);
    }
    pushCurrentCharacter();
    return null_token;
}

/**
 * digits_counter default: nullptr
 * max_digits default: 1
 */
Token Tokenizer::pushOctDigit(size_t* digits_counter, size_t max_digits) {
    if(!isInRange() || current_num_literal != OCT)
        throw "#TODO: Method pushOctDigit can not be called now.";
    if(!isodigit(*current)) {
        if(curr_token.str.empty())
            eprint(new InvalidLiteralError(
                curr_token.position,
                *source,
                "octal number"));
        return token_create(
            TOKL_INT,
            curr_token.position,
            curr_token.str);
    }
    if(digits_counter){
        if(*digits_counter >= max_digits)
            throw "#TODO: Too long octal literal";
        (*digits_counter)++;
    }
    pushCurrentCharacter();
    return null_token;
}

/**
 * digits_counter default: nullptr
 * max_digits default: 1
 */
Token Tokenizer::pushHexDigit(size_t* digits_counter, size_t max_digits) {
    if(!isInRange() || current_num_literal != HEX)
        throw "#TODO: Method pushHexDigit can not be called now.";
    if(!isxdigit(*current)) {
        if(curr_token.str.empty())
            eprint(new InvalidLiteralError(
                curr_token.position,
                *source,
                "hexadecimal number"));
        return token_create(
            TOKL_INT,
            curr_token.position,
            curr_token.str);
    }
    if(digits_counter){
        if(*digits_counter >= max_digits)
            throw "#TODO: Too long hexadecimal literal";
        (*digits_counter)++;
    }
    pushCurrentCharacter();
    return null_token;
}

Token Tokenizer::getWordToken() {
    if(!isInRange()) eprint(new OutOfSourceRangeError(*source));
    if(!(isalpha(*current) || isChar('_'))) return null_token;
    resetCurrentToken();
    pushCurrentCharacter();
    while(isInRange() && (isalnum(*current) || isChar('_')))
        pushCurrentCharacter();
    auto found = static_word_token_types.find(curr_token.str);
    return token_create(
        found!=static_word_token_types.end() ? found->second : TOK_NAME,
        curr_token.position,
        curr_token.str
    );
}

Token Tokenizer::getSymbolToken() {
    if(!isInRange()) eprint(new OutOfSourceRangeError(*source));
    if(!ispunct(*current)) return null_token;
    resetCurrentToken();
    
    std::map<char, TokenType> search_for = bracket_token_types;
    search_for.insert(
        single_symbol_token_types.begin(),
        single_symbol_token_types.end());
    auto found = search_for.find(*current);
    if(found!=search_for.end()) {
        pushCurrentCharacter();
        return token_create(found->second, curr_token.position, curr_token.str);
    }

    switch(*current) {
        case '+':
            pushCurrentCharacter();
            if(isChar('+')) {
                pushCurrentCharacter();
                return parseToken(ASOP_INCR);           // ++
            }
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSSUM);         // +=
            }
            return parseToken(AOP_ADD);                 // +
        case '-':
            pushCurrentCharacter();
            if(isChar('-')) {
                pushCurrentCharacter();
                return parseToken(ASOP_DECR);           // --
            }
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSDIFF);        // -=
            }
            if(isChar('>')) {
                pushCurrentCharacter();
                return parseToken(OP_ARROW);            // ->
            }
            return parseToken(AOP_SUB);                 // -
        case '/':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSQUOT);        // /=
            }
            return parseToken(AOP_DIV);                 // /
        case '%':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSMOD);         // %=
            }
            return parseToken(AOP_MOD);                 // %
        case '^':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSPOW);         // ^=
            }
            return parseToken(AOP_POW);                 // ^
        case '|':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSBOR);         // |=
            }
            if(isChar('|')) {
                pushCurrentCharacter();
                return parseToken(LOP_OR);              // ||
            }
            return parseToken(BOP_OR);                  // |
        case '&':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSBAND);        // &=
            }
            if(isChar('&')) {
                pushCurrentCharacter();
                return parseToken(LOP_AND);             // &&
            }
            return parseToken(BOP_AND);                 // &
        case '~':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSBXOR);        // ~=
            }
            return parseToken(BOP_XOR);                 // ~
        case '<':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(COP_LOREQ);           // <=
            }
            if(isChar('<')) {
                pushCurrentCharacter();
                if(isChar('=')) {
                    pushCurrentCharacter();
                    return parseToken(ASOP_ASSBLS);     // <<=
                } 
                return parseToken(BOP_LS);              // <<
            }
            return parseToken(COP_LT);                  // <
        case '>':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(COP_GOREQ);           // >=
            }
            if(isChar('>')) {
                pushCurrentCharacter();
                if(isChar('=')) {
                    pushCurrentCharacter();
                    return parseToken(ASOP_ASSBRS);     // >>=
                } 
                return parseToken(BOP_RS);              // >>
            }
            return parseToken(COP_GT);                  // >
        case '=':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(COP_EQUAL);           // ==
            }
            return parseToken(ASOP_ASSIGN);             // =
        case '*':
            pushCurrentCharacter();
            if(isChar('=')) {
                pushCurrentCharacter();
                return parseToken(ASOP_ASSPROD);        // *=
            }
            return parseToken(MULTOP_STAR);             // *
    }
    current = sourceBegin() + curr_token.position.idx;
    return null_token;
}

size_t Tokenizer::getIdx() const {return getIdx(current);}
size_t Tokenizer::getIdx(string::iterator iterator) const {
    return distance(sourceBegin(), iterator);
} // salt::Tokenizer

} // salt
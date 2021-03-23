/**
 * tokenizer/token.h implementation
 *
 */
#include "../../include/tokenizer/token.h"

#include "../../include/utils.h"

namespace salt::tokenizer
{

std::map<string, TokenType> no_value_token_types {
    // Access keywords
    {"public", KW_PUBLIC},
    {"private", KW_PRIVATE},

    // Flow control keywords
    {"if", KW_IF},
    {"else", KW_ELSE},
    {"elif", KW_ELIF},
    {"while", KW_WHILE},
    {"for", KW_FOR},
    {"break", KW_BREAK},   
    {"continue", KW_CONTINUE},

    // Import keywords
    {"import", KW_IMPORT},
    {"as", KW_AS},
    {"dynamic", KW_DYNAMIC},

    // Variable manipulation keywords
    {"const", KW_CONST},
    {"del", KW_DEL},

    // Keywords
    {"return", KW_RETURN},
    {"throw", KW_THROW},

    // Base types
    {"bool", TYPE_BOOL},
    {"int", TYPE_INT},
    {"float", TYPE_FLOAT},
    {"string", TYPE_STRING},

    // Bool literals
    {"true", TOKL_BOOL},
    {"false", TOKL_BOOL},

    // Null literal
    {"null", TOKL_NULL}
};

bool Token::isNothing() const {return type == TOK_0;}

/* Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(TokenType _tok, InStringPosition _position, string _val)
{
    Token tok = {_tok, _val, _position};
    return tok;
}

} // salt
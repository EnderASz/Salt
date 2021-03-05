/**
 * The token class contains a single token 
 */
#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
using std::string;

enum TokenType
{
    // Keywords
    KW_PUBLIC,      // public
    KW_PRIVATE,     // private
    KW_INTERNAL,    // internal
    KW_IMPORT,      // import
    KW_UNIT,        // unit
    KW_DEL,         // del

    // Control keywords
    KW_RETURN,      // return
    KW_IF,          // if
    KW_WHILE,       // while
    KW_FOR,         // for
    KW_THROW,       // throw

    // Brackets
    TOK_LBRACKET,   // (
    TOK_RBRACKET,   // )
    TOK_LCULR,      // {
    TOK_RCURL,      // }
    TOK_LSQUARE,    // [
    TOK_RSQUARE,    // ]

    TOK_DOT,        // .
    TOK_SYMBOL,     // any_symbol
    TOK_ARROW,      // ->
    TOK_NEWLINE,    // \n

    // Base types
    TOK_INT,        // int
    TOK_STRING,     // string
    TOK_NULL,       // null
    TOK_CHAR,       // char
    TOK_BOOL,       // bool
    TOK_FLOAT,      // float

    // Constant values
    TOKC_STRING,    // "string"
    TOKC_FLOAT,     // 12.4
    TOKC_INT,       // 55
    TOKC_BOOL,      // true | false
};
 
/* The token type stores a single token, which can than be turned
 * into a Token array for validating. Depending on the type, the 
 * value may be NULL or have an actual string value, like a TOK_CSTR
 * which has the actual string value.
 */
typedef struct 
{
    string    value;
    TokenType type;

} Token;

/* Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(string _val, TokenType _tok);

#endif // TOKEN_H_
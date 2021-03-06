/**
 * The token class contains a single token.
 */
#ifndef TOKEN_H_
#define TOKEN_H_

#include "utils.h"
#include <string>
#include <map>

using std::string;

namespace salt
{

/* TokenType enumerator contains token types. */
enum TokenType
{
    TOK_0,          // Literally nothing

    // Access keywords
    KW_PUBLIC,      // public
    KW_PRIVATE,     // private

    // Control keywords
    KW_IF,          // if
    KW_ELSE,        // else
    KW_ELIF,        // elif
    KW_WHILE,       // while
    KW_FOR,         // for
    KW_BREAK,       // break
    KW_CONTINUE,    // continue
    KW_RETURN,      // return
    KW_THROW,       // throw

    // Import keywords
    KW_IMPORT,      // import
    KW_AS,          // as
    KW_DYNAMIC,     // dynamic

    // Alias keyword
    KW_ALIAS,        // alias

    // Variable manipulation keywords
    KW_CONST,       // const
    KW_DEL,         // del

    // Brackets
    BKT_ROUNDL,     // (
    BKT_ROUNDR,     // )
    BKT_CULRL,      // {
    BKT_CULRR,      // }
    BKT_SQUAREL,    // [
    BKT_SQUARER,    // ]

    // Arithmetic operators
    AOP_ADD,        // +
    AOP_SUB,        // -
    AOP_DIV,        // /
    AOP_MOD,        // %
    AOP_POW,        // ^

    // Bitwise operators
    BOP_OR,         // |
    BOP_AND,        // &
    BOP_XOR,        // ~
    BOP_LS,         // <<
    BOP_RS,         // >>

    // Assigment operators
    ASOP_ASSIGN,    // =
    ASOP_INCR,      // ++
    ASOP_DECR,      // --
    ASOP_ASSSUM,    // +=
    ASOP_ASSDIFF,   // -=
    ASOP_ASSPROD,   // *=
    ASOP_ASSQUOT,   // /=
    ASOP_ASSMOD,    // %=
    ASOP_ASSPOW,    // ^=
    ASOP_ASSBOR,    // |=
    ASOP_ASSBAND,   // &=
    ASOP_ASSBXOR,   // ~=
    ASOP_ASSBLS,    // <<=
    ASOP_ASSBRS,    // >>=

    // Comparision operators
    COP_EQUAL,      // ==
    COP_LT,         // <
    COP_GT,         // >
    COP_LOREQ,      // <=
    COP_GOREQ,      // >=
    COP_TYPE,       // istype

    // Logic operators
    LOP_OR,         // ||
    LOP_AND,        // &&

    // Multifunctional operators
    MULTOP_NOT,     // !
    MULTOP_STAR,    // *

    // Other operators and symbols
    OP_COLON,      // :
    OP_SEMIC,      // ;
    OP_COMMA,      // ,
    OP_ARROW,      // ->
    OP_DOT,        // .

    // Base types
    TYPE_BOOL,       // bool
    TYPE_INT,        // int
    TYPE_FLOAT,      // float
    TYPE_STRING,     // string    

    // Literals
    TOKL_NULL,      // null
    TOKL_STRING,    // "string"
    TOKL_FLOAT,     // 12.4
    TOKL_INT,       // 55
    TOKL_BOOL,      // true | false

    // Name
    TOK_NAME       // names used to calls, accesses, imports etc.

}; // salt::tokenizer::TokenType

extern std::map<string, TokenType> static_word_token_types;
extern std::map<char, TokenType> bracket_token_types;
extern std::map<char, TokenType> single_symbol_token_types;

extern std::map<TokenType, string> token_names;
 
/**
 * The token type stores a single token, which can than be turned
 * into a Token array for validating. Depending on the type, the 
 * value may be NULL or have an actual string value, like a TOKC_STRING
 * which has the actual string value.
 */
struct Token 
{
    TokenType type;
    string    value;
    InStringPosition position;

    /* Return true if an instance of Token has TOK_0 type. */
    bool isNothing() const;
};

/**
 * Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(
    TokenType _tok,
    InStringPosition position,
    string _val = "");

/**
 * Token with type TOK_0, NULL value and NULL positions.
 * It represents null token.
 */
const Token null_token = token_create(TOK_0, {0, 0, 0}, "");

} // salt::tokenizer

#endif // TOKEN_H_
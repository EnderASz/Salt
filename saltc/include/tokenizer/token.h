/**
 * The token class contains a single token.
 */
#ifndef TOKEN_H_
#define TOKEN_H_

#include <string>
using std::string;

namespace salt::tokenizer
{

/* TokenType enumerator contains token types. */
enum TokenType
{
    TOK_0,          // Literally nothing

    // Keywords
    KW_PUBLIC,      // public
    KW_PRIVATE,     // private
    KW_CONST,       // const

    // Control keywords
    KW_IMPORT,      // import
    KW_DEL,         // del
    KW_RETURN,      // return
    KW_IF,          // if
    KW_ELSE,        // else
    KW_ELIF,        // elif
    KW_WHILE,       // while
    KW_FOR,         // for
    KW_THROW,       // throw
    KW_BREAK,       // break
    KW_CONTINUE,    // continue

    // Brackets
    BKT_ROUNDL,     // (
    BKT_ROUNDR,     // )
    BKT_CULRL,      // {
    BKT_CULRR,      // }
    BKT_SQUAREL,    // [
    BKT_SQUARER,    // ]

    // Symbols
    SYM_ANY,        // Anything, really, it can be... anything symbol you want
    SYM_COLON,      // :
    SYM_SEMIC,      // ;
    SYM_COMMA,      // ,
    SYM_ARROW,      // ->
    SYM_DOT,        // .

    // Arithmetic operators
    AOP_ADD,        // +
    AOP_SUB,        // -
    AOP_MULT,       // *
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
    ASOP_PINCR,     // ++
    ASOP_PDECR,     // --
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

    // Multifunctional operators - Yep, only 'not' is here. It is special.
    MULTOP_NOT,     // !

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
    TOK_NAME,       // names used to calls, accesses, imports etc.
}; // salt::tokenizer::TokenType
 
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
    size_t    index; // Position in code
};

/**
 * Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(TokenType _tok, size_t _idx, string _val = NULL);

} // salt::tokenizer

#endif // TOKEN_H_
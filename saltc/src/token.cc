/**
 * tokenizer/token.h implementation
 *
 * @author EnderASz
 */
#include "../include/token.h"

namespace salt
{

const std::map<std::string, TokenType> static_word_token_types {
    // Flow control keywords
    {"if",          KW_IF},
    {"else",        KW_ELSE},
    {"elif",        KW_ELIF},
    {"while",       KW_WHILE},
    {"for",         KW_FOR},
    {"break",       KW_BREAK},
    {"continue",    KW_CONTINUE},

    // Import keywords
    {"import",      KW_IMPORT},
    {"as",          KW_AS},
    {"dynamic",     KW_DYNAMIC},

    // Alias keyword
    {"alias",       KW_ALIAS},

    // Variable manipulation keywords
    {"const",       KW_CONST},
    {"del",         KW_DEL},

    // Keywords
    {"return",      KW_RETURN},
    {"throw",       KW_THROW},

    // istype operator
    {"istype",      COP_TYPE},

    // Base types
    {"bool",        TYPE_BOOL},
    {"int",         TYPE_INT},
    {"float",       TYPE_FLOAT},
    {"string",      TYPE_STRING},

    // Bool literals
    {"true",        TOKL_BOOL},
    {"false",       TOKL_BOOL},

    // Null literal
    {"null",        TOKL_NULL}
};

const std::map<char, TokenType> bracket_token_types {
    {'(',           BKT_ROUNDL},
    {')',           BKT_ROUNDR},
    {'{',           BKT_CULRL},
    {'}',           BKT_CULRR},
    {'[',           BKT_SQUAREL},
    {']',           BKT_SQUARER},
};

const std::map<char, TokenType> single_symbol_token_types {
    {':',           OP_COLON},
    {';',           OP_SEMIC},
    {',',           OP_COMMA},
    {'.',           OP_DOT},
    {'!',           MULTOP_NOT}
};

const std::map<TokenType, std::string> token_names {
    {TOK_0,         "TOK_0"},

    // Access keywords
    {KW_PUBLIC,     "KW_PUBLIC"},
    {KW_PRIVATE,    "KW_PRIVATE"},

    // Control keywords
    {KW_IF,         "KW_IF"},
    {KW_ELSE,       "KW_ELSE"},
    {KW_ELIF,       "KW_ELIF"},
    {KW_WHILE,      "KW_WHILE"},
    {KW_FOR,        "KW_FOR"},
    {KW_BREAK,      "KW_BREAK"},
    {KW_CONTINUE,   "KW_CONTINUE"},
    {KW_RETURN,     "KW_RETURN"},
    {KW_THROW,      "KW_THROW"},

    // Import keywords
    {KW_IMPORT,     "KW_IMPORT"},
    {KW_AS,         "KW_AS"},
    {KW_DYNAMIC,    "KW_DYNAMIC"},

    // Alias keyword
    {KW_ALIAS,      "KW_ALIAS"},

    // Variable manipulation keywords
    {KW_CONST,      "KW_CONST"},
    {KW_DEL,        "KW_DEL"},

    // Brackets
    {BKT_ROUNDL,    "BKT_ROUNDL"},
    {BKT_ROUNDR,    "BKT_ROUNDR"},
    {BKT_CULRL,     "BKT_CULRL"},
    {BKT_CULRR,     "BKT_CULRR"},
    {BKT_SQUAREL,   "BKT_SQUAREL"},
    {BKT_SQUARER,   "BKT_SQUARER"},

    // Arithmetic operators
    {AOP_ADD,       "AOP_ADD"},
    {AOP_SUB,       "AOP_SUB"},
    {AOP_DIV,       "AOP_DIV"},
    {AOP_MOD,       "AOP_MOD"},
    {AOP_POW,       "AOP_POW"},

    // Bitwise operators
    {BOP_OR,        "BOP_OR"},
    {BOP_AND,       "BOP_AND"},
    {BOP_XOR,       "BOP_XOR"},
    {BOP_LS,        "BOP_LS"},
    {BOP_RS,        "BOP_RS"},

    // Assigment operators
    {ASOP_ASSIGN,   "ASOP_ASSIGN"},
    {ASOP_INCR,     "ASOP_INCR"},
    {ASOP_DECR,     "ASOP_DECR"},
    {ASOP_ASSSUM,   "ASOP_ASSSUM"},
    {ASOP_ASSDIFF,  "ASOP_ASSDIFF"},
    {ASOP_ASSPROD,  "ASOP_ASSPROD"},
    {ASOP_ASSQUOT,  "ASOP_ASSQUOT"},
    {ASOP_ASSMOD,   "ASOP_ASSMOD"},
    {ASOP_ASSPOW,   "ASOP_ASSPOW"},
    {ASOP_ASSBOR,   "ASOP_ASSBOR"},
    {ASOP_ASSBAND,  "ASOP_ASSBAND"},
    {ASOP_ASSBXOR,  "ASOP_ASSBXOR"},
    {ASOP_ASSBLS,   "ASOP_ASSBLS"},
    {ASOP_ASSBRS,   "ASOP_ASSBRS"},

    // Comparision operators
    {COP_EQUAL,     "COP_EQUAL"},
    {COP_LT,        "COP_LT"},
    {COP_GT,        "COP_GT"},
    {COP_LOREQ,     "COP_LOREQ"},
    {COP_GOREQ,     "COP_GOREQ"},
    {COP_TYPE,      "COP_TYPE"},

    // Logic operators
    {LOP_OR,        "LOP_OR"},
    {LOP_AND,       "LOP_AND"},

    // Multifunctional operators
    {MULTOP_NOT,    "MULTOP_NOT"},
    {MULTOP_STAR,   "MULTOP_STAR"},

    // Other operators and symbols
    {OP_COLON,      "OP_COLON"},
    {OP_SEMIC,      "OP_SEMIC"},
    {OP_COMMA,      "OP_COMMA"},
    {OP_ARROW,      "OP_ARROW"},
    {OP_DOT,        "OP_DOT"},

    // Base types
    {TYPE_BOOL,     "TYPE_BOOL"},
    {TYPE_INT,      "TYPE_INT"},
    {TYPE_FLOAT,    "TYPE_FLOAT"},
    {TYPE_STRING,   "TYPE_STRING"},

    // Literals
    {TOKL_NULL,     "TOKL_NULL"},
    {TOKL_STRING,   "TOKL_STRING"},
    {TOKL_FLOAT,    "TOKL_FLOAT"},
    {TOKL_INT,      "TOKL_INT"},
    {TOKL_BOOL,     "TOKL_BOOL"},

    // Name
    {TOK_NAME,      "TOK_NAME"},
};

bool Token::isNothing() const {
    return type == TOK_0;
}

Token token_create(TokenType tok, InStringPosition pos, string val) {
    return (Token) {_tok, _val, _position};
}

} // namespace salt
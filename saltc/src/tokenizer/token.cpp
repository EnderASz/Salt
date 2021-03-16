/**
 * tokenizer/token.h implementation
 *
 */
#include "../../include/tokenizer/token.h"

namespace salt::tokenizer
{

/* Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(TokenType _tok, size_t _idx, string _val)
{
    Token tok = {_tok, _val, _idx};
    return tok;
}

} // salt
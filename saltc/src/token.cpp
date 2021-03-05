/**
 * token.h implementation
 *
 */
#include "../include/token.h"

/* Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(string _val, TokenType _tok)
{
    Token tok = {_val, _tok};
    return tok;
}

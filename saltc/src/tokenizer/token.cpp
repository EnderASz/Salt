/**
 * tokenizer/token.h implementation
 *
 */
#include "../../include/tokenizer/token.h"

#include "../../include/utils.h"

namespace salt::tokenizer
{

bool Token::isNothing() const {return type == TOK_0;}

/* Create a new token from the given parameters. This doesn't really
 * do anything, it's just a shorthand.
 */
Token token_create(TokenType _tok, InStringPosition _position, string _val)
{
    Token tok = {_tok, _val, _position};
    return tok;
}
Token create_null_token() {return Token({TOK_0, NULL, {NULL, NULL , NULL}});}

} // salt
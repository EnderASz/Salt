/**
 * This header file contains all error classes that can be occured in Salt
 * compilation time and error interfaces or base classes
 * such as BaseError or InSourceError.
 *
 * @author EnderASz
 */
#ifndef SALT_ERRORS_H_
#define SALT_ERRORS_H_

#include "utils.h"
#include "source_file.h"
#include "tokenizer/token.h"
#include <string>

using std::string;

namespace salt
{

class BaseError {
public:
    /** Returns an error message */
    virtual string getMessage() = 0;

    virtual ~BaseError();
};


class InSourceError : public BaseError {
protected:
    /** Position where the error occurred */
    InStringPosition position;
    
    /** Source file where the error occurred */
    SourceFile* source_file;

    /** Sets position where the error occurred */
    void setPosition(InStringPosition position);
    
    /** Sets pointer to source file where the error occurred */
    void setSource(SourceFile& source_file);

public:
    InSourceError(
        const InStringPosition position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    virtual string getMessage();

    /** Gets position where the error occured */
    InStringPosition getPosition();

    /** Gets pointer to source file where the error occurred */
    SourceFile* getSource();
};


class UnclosedCommentError : public InSourceError {
public:
    /** Unclosed Comment Error constructor */
    UnclosedCommentError(
        const InStringPosition position,
        const SourceFile& source_file
    );

    string getMessage();
};


class UnexpectedTokenError : public InSourceError {
private:
    /** The token that caused the error when encountered */
    string token_str;
    TokenType token_type;

    /** Sets token string */
    void setToken(string token, TokenType type = TOK_UNKNOWN);
    void setToken(Token token);
public:
    string getTokenStr();
    UnexpectedTokenError(
        const InStringPosition position,
        const SourceFile& source_file,
        const string str,
        const TokenType type
    );
    UnexpectedTokenError(
        const InStringPosition position,
        const SourceFile& source_file,
        const Token token
    );
};

};

#endif
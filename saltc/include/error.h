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
#include "token.h"
#include <string>

using std::string;

namespace salt
{

class BaseError {
public:
    /** Returns an error message */
    virtual string getMessage() = 0;

    /** Base Error deconstructor */
    virtual ~BaseError() = default;
};


class CustomError : public BaseError {
private:
    /** Custom error message */
    string message;
public:
    /** Custom error constructor */
    CustomError(const string& message);

    /** Inherited: Returns an error message */
    string getMessage();
};


class FileOpenError : public BaseError {
private:
    /** Path where the error occurred */
    string filepath;
public:
    FileOpenError(const string& filepath);

    /** Returns path where the error occurred */
    string getFilepath();

    /** Inherited: Returns an error message */
    string getMessage();
};


class CommandLineError : public BaseError {
public:
    /** Inherited: Returns an error message */
    virtual string getMessage();

    /** Returns string hint to use --help */
    string getHelpRecomendation();
};


class UnspecifiedMainError : public CommandLineError {
    /** Inherited: Returns an error message */
    string getMessage();
};


class UnrecognizedOptionError : public CommandLineError {
private:
    string option;
public:
    /** Unrecognized Option Error constructor */
    UnrecognizedOptionError(const string& option);

    /** Inherited: Returns an error message */
    virtual string getMessage();

    /** Returns unrecognized option */
    string getOption();
};


class SourceError : public BaseError {
protected:
    /** Source file where the error occurred */
    SourceFile* source_file;
public:
    /** Source Error constructor */
    SourceError(const SourceFile& source_file);

    /** Inherited: Returns an error message */
    virtual string getMessage();

    /** Returns pointer to source file where the error occurred */
    SourceFile* getSource();
};


class InSourceError : public SourceError {
protected:
    /** Position where the error occurred */
    InStringPosition position;

public:
    InSourceError(
        const InStringPosition& position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    virtual string getMessage();

    /** Returns position where the error occured */
    InStringPosition getPosition();

    string getLocationString();
};


class OutOfSourceRangeError : public SourceError {
public:
    /** Out of Source Range Token Get Error constructor */
    OutOfSourceRangeError(const SourceFile& source_file);

    /** Inherited: Returns an error message */
    virtual string getMessage();
};


class UnclosedCommentError : public InSourceError {
public:
    /** Unclosed Comment Error constructor */
    UnclosedCommentError(
        const InStringPosition& position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    string getMessage();
};


class UnclosedStringError : public InSourceError {
public:
    /** Unclosed String Literal Error constructor */
    UnclosedStringError(
        const InStringPosition& position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    string getMessage();
};


class UnexpectedTokenError : public InSourceError {
private:
    /** The token that caused the error when encountered */
    string token_str;
    TokenType token_type;

    /** Sets token string */
    void setToken(const string& token, TokenType type = TOK_0);
    void setToken(Token token = null_token);
public:
    /** Unexpected Token Error constructors */
    UnexpectedTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const string& str,
        const TokenType type
    );
    UnexpectedTokenError(
        const SourceFile& source_file,
        const Token& token
    );

    /** Returns token value string */
    string getTokenStr();

    /** Returns token type */
    TokenType getTokenType();

    /** Inherited: Returns an error message */
    string getMessage();
};


class UnknownTokenError : public InSourceError {
private:
    /**
     * The unrecognized token string that can not be parsed to
     * any known token
     */
    string token_str;

public:
    UnknownTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const string& str
    );

    /** Returns unrecognized token string */
    string getTokenStr();

    /** Inherited: Returns an error message */
    string getMessage();
};


class InvalidLiteralError : public InSourceError {
protected:
    string literal_name;
public:
    /** Invalid Literal Error constructors */
    InvalidLiteralError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const string& literal_type_name);

    /** Returns literal name */
    string getLiteralName();
    
    /** Inherited: Returns an error message */
    string getMessage();
};


class TooLongLiteralError : public InvalidLiteralError {
private:
    int max_lenght;
public:
    /** Too Long Literal Error constructor */
    TooLongLiteralError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const string& literal_type_name,
        const int max_lenght);

    /** Returns max literal lenght */
    int getMaxLenght();

    /** Inherited: Returns an error message */
    string getMessage();
};

};

#endif
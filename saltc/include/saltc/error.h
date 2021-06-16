/**
 * This header file contains all error classes that can be occured in Salt
 * compilation time and error interfaces or base classes
 * such as BaseError or InSourceError.
 *
 * @author EnderASz
 */
#ifndef SALT_ERRORS_H_
#define SALT_ERRORS_H_

#include "source_file.h"
#include "utils.h"
#include "token.h"

#include <string>

namespace salt
{

class BaseError {
public:
    /** Returns an error message */
    virtual std::string getMessage() = 0;

    /** Base Error deconstructor */
    virtual ~BaseError() = default;
};


class CustomError : public BaseError {
private:
    /** Custom error message */
    std::string message;
public:
    /** Custom error constructor */
    CustomError(const std::string& message);

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class FileOpenError : public BaseError {
private:
    /** Path where the error occurred */
    std::string filepath;
public:
    FileOpenError(const std::string& filepath);

    /** Returns path where the error occurred */
    std::string getFilepath();

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class CommandLineError : public BaseError {
public:
    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;

    /** Returns std::string hint to use --help */
    std::string getHelpRecomendation();
};


class UnspecifiedMainError : public CommandLineError {
    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};

class CommandLineOptionError : public CommandLineError {
protected:
    std::string option;
public:
    /** Unrecognized Option Error constructor */
    CommandLineOptionError(const std::string& option);

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;

    /** Returns unrecognized option */
    std::string getOption();
};

class UnrecognizedOptionError : public CommandLineOptionError {
protected:
    std::string option;
public:
    /** Unrecognized Option Error constructor */
    UnrecognizedOptionError(const std::string& option);

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class MissingOptionArgumentError : public CommandLineOptionError {
public:
    MissingOptionArgumentError(const std::string& option);
    virtual std::string getMessage() override;
};


class SourceError : public BaseError {
protected:
    /** Source file where the error occurred */
    SourceFile* source_file;
public:
    /** Source Error constructor */
    SourceError(const SourceFile& source_file);

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;

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
    virtual std::string getMessage() override;

    /** Returns position where the error occured */
    InStringPosition getPosition();

    std::string getLocationString();
};


class OutOfSourceRangeError : public SourceError {
public:
    /** Out of Source Range Token Get Error constructor */
    OutOfSourceRangeError(const SourceFile& source_file);

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class UnclosedCommentError : public InSourceError {
public:
    /** Unclosed Comment Error constructor */
    UnclosedCommentError(
        const InStringPosition& position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class UnclosedStringError : public InSourceError {
public:
    /** Unclosed String Literal Error constructor */
    UnclosedStringError(
        const InStringPosition& position,
        const SourceFile& source_file
    );

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class UnexpectedTokenError : public InSourceError {
private:
    /** The token that caused the error when encountered */
    std::string token_str;
    TokenType token_type;

    /** Sets token std::string */
    void setToken(const std::string& token, TokenType type = TOK_0);
    void setToken(Token token = null_token);
public:
    /** Unexpected Token Error constructors */
    UnexpectedTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const std::string& str,
        const TokenType type
    );
    UnexpectedTokenError(
        const SourceFile& source_file,
        const Token& token
    );

    /** Returns token value std::string */
    std::string getTokenStr();

    /** Returns token type */
    TokenType getTokenType();

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class UnknownTokenError : public InSourceError {
private:
    /**
     * The unrecognized token std::string that can not be parsed to
     * any known token
     */
    std::string token_str;

public:
    UnknownTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const std::string& str
    );

    /** Returns unrecognized token std::string */
    std::string getTokenStr();

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class InvalidLiteralError : public InSourceError {
protected:
    std::string literal_name;
public:
    /** Invalid Literal Error constructors */
    InvalidLiteralError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const std::string& literal_type_name);

    /** Returns literal name */
    std::string getLiteralName();

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};


class TooLongLiteralError : public InvalidLiteralError {
private:
    int max_lenght;
public:
    /** Too Long Literal Error constructor */
    TooLongLiteralError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const std::string& literal_type_name,
        const int max_lenght);

    /** Returns max literal lenght */
    int getMaxLenght();

    /** Inherited: Returns an error message */
    virtual std::string getMessage() override;
};

};

#endif
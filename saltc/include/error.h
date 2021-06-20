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
#include <memory>

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

public:
    /** Custom error constructor */
    CustomError(const string& message);

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

private:
    /** Custom error message */
    string message;

};


class FileOpenError : public BaseError {

public:
    FileOpenError(const string& filepath);

    /** Returns path where the error occurred */
    string getFilepath();

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    /** Path where the error occurred */
    string filepath;

};


class CommandLineError : public BaseError {
public:
    /** Inherited: Returns an error message */
    virtual string getMessage() override;

    /** Returns string hint to use --help */
    string getHelpRecomendation();
};


class UnspecifiedMainError : public CommandLineError {
public:
    /** Inherited: Returns an error message */
    virtual string getMessage() override;
};

class CommandLineOptionError : public CommandLineError {

public:
    /** Unrecognized Option Error constructor */
    CommandLineOptionError(const string& option);

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

    /** Returns unrecognized option */
    string getOption();

protected:
    string option;

};

class UnrecognizedOptionError : public CommandLineOptionError {

public:
    /** Unrecognized Option Error constructor */
    UnrecognizedOptionError(const string& option);

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    string option;

};


class MissingOptionArgumentError : public CommandLineOptionError {

public:
    MissingOptionArgumentError(const string& option);
    virtual string getMessage() override;

};


class SourceError : public BaseError {

public:
    /** Source Error constructor */
    SourceError(const std::shared_ptr<SourceFile>& source_file);

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

    /** Returns pointer to source file where the error occurred */
    const std::shared_ptr<SourceFile>& getSource();

protected:
    /** Source file where the error occurred */
    const std::shared_ptr<SourceFile>& source_file;

};


class InSourceError : public SourceError {

public:
    InSourceError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file
    );

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

    /** Returns position where the error occured */
    InStringPosition getPosition();

    string getLocationString();

protected:
    /** Position where the error occurred */
    InStringPosition position;

};


class OutOfSourceRangeError : public SourceError {
public:
    /** Out of Source Range Token Get Error constructor */
    OutOfSourceRangeError(const std::shared_ptr<SourceFile>& source_file);

    /** Inherited: Returns an error message */
    virtual string getMessage() override;
};


class UnclosedCommentError : public InSourceError {
public:
    /** Unclosed Comment Error constructor */
    UnclosedCommentError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file
    );

    /** Inherited: Returns an error message */
    virtual string getMessage() override;
};


class UnclosedStringError : public InSourceError {
public:
    /** Unclosed String Literal Error constructor */
    UnclosedStringError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file
    );

    /** Inherited: Returns an error message */
    virtual string getMessage() override;
};


class UnexpectedTokenError : public InSourceError {

public:
    /** Unexpected Token Error constructors */
    UnexpectedTokenError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file,
        const string& str,
        const TokenType type
    );
    UnexpectedTokenError(
        const std::shared_ptr<SourceFile>& source_file,
        const Token& token
    );

    /** Returns token value string */
    string getTokenStr();

    /** Returns token type */
    TokenType getTokenType();

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    /** The token that caused the error when encountered */
    string token_str;
    TokenType token_type;

    /** Sets token string */
    void setToken(const string& token, TokenType type = TOK_0);
    void setToken(Token token = null_token);

};


class UnknownTokenError : public InSourceError {
public:
    UnknownTokenError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file,
        const string& str
    );

    /** Returns unrecognized token string */
    string getTokenStr();

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    /**
     * The unrecognized token string that can not be parsed to
     * any known token
     */
    string token_str;

};


class InvalidLiteralError : public InSourceError {

public:
    /** Invalid Literal Error constructors */
    InvalidLiteralError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file,
        const string& literal_type_name);

    /** Returns literal name */
    string getLiteralName();
    
    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    string literal_name;

};


class TooLongLiteralError : public InvalidLiteralError {

public:
    /** Too Long Literal Error constructor */
    TooLongLiteralError(
        const InStringPosition& position,
        const std::shared_ptr<SourceFile>& source_file,
        const string& literal_type_name,
        const int max_lenght);

    /** Returns max literal lenght */
    int getMaxLenght();

    /** Inherited: Returns an error message */
    virtual string getMessage() override;

protected:
    int max_lenght;

};

};

#endif
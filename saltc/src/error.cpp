/**
 * error.h implementation
 *
 * @author EnderASz
 */
#include "../include/error.h"

#include "../include/utils.h"
#include "../include/token.h"
#include <string>

using std::string;
using std::to_string;

namespace salt
{

#pragma region CustomError
CustomError::CustomError(const string& message): message(message) {}

string CustomError::getMessage() {return message;}
#pragma endregion CustomError


#pragma region FileOpenError
FileOpenError::FileOpenError(const string& filepath): filepath(filepath) {}

string FileOpenError::getFilepath() {return filepath;}

string FileOpenError::getMessage() {
    return "An error occurred when tried to open '" + getFilepath() + "' file.";
}
#pragma endregion FileOpenError


#pragma region SourceError
SourceError::SourceError(const SourceFile& source_file)
    :source_file(const_cast<SourceFile*>(&source_file)) {}

SourceFile* SourceError::getSource() {return source_file;}

string SourceError::getMessage() {
    return "An error occured in '" +
        getSource()->getFilePath() +
        "' source.";
}
#pragma endregion SourceError

#pragma region CommandLineError
string CommandLineError::getMessage() {
    return "An error occured in command line. " +
        getHelpRecomendation();
}

string CommandLineError::getHelpRecomendation() {
    return "For more information use --help.";
}
#pragma endregion CommandLineError

#pragma region UnspecifiedMainError
string UnspecifiedMainError::getMessage() {
    return "Main file to compile is unspecified. Please specify it. " +
        getHelpRecomendation();
}
#pragma endregion UnspecifiedMainError

#pragma region UnrecognizedOptionError
UnrecognizedOptionError::UnrecognizedOptionError(const string& option)
    :option(option) {}

string UnrecognizedOptionError::getMessage() {
    return "Unrecognized command line option '" +
        getOption() +
        "'. " +
        getHelpRecomendation();
}

string UnrecognizedOptionError::getOption() {return option;}
#pragma endregion UnrecognizedOptionError

#pragma region InSourceError
InSourceError::InSourceError(
    const InStringPosition& position,
    const SourceFile& source_file)
        :SourceError(source_file),
        position(position) {}

string InSourceError::getMessage() {
    return "An error occured in '" +
        getSource()->getFilePath() +
        "' at line " +
        to_string(getPosition().line_idx + 1) +
        " at position " +
        to_string(getPosition().inline_idx + 1) +
        ".";
}

InStringPosition InSourceError::getPosition() {return position;}

string InSourceError::getLocationString() {
    return "in '" +
        getSource()->getFilePath() +
        "' at line " +
        to_string(getPosition().line_idx + 1) +
        " at position " +
        to_string(getPosition().inline_idx + 1);

}
#pragma endregion InSourceError

#pragma region OutOfSourceRangeError
OutOfSourceRangeError::OutOfSourceRangeError(const SourceFile& source_file)
    :SourceError(source_file) {}

string OutOfSourceRangeError::getMessage() {
    return "Tried to get token out of range '" +
        getSource()->getFilePath() +
        "' source.";
}
#pragma endregion OutOfSourceRangeError

#pragma region UnclosedCommentError
UnclosedCommentError::UnclosedCommentError(
    const InStringPosition& position,
    const SourceFile& source_file)
        :InSourceError(position, source_file) {}

string UnclosedCommentError::getMessage() {
    return "Unclosed comment " +
        getLocationString() +
        ".";
}
#pragma endregion UnclosedCommentError

#pragma region UnclosedStringError
UnclosedStringError::UnclosedStringError(
    const InStringPosition& position,
    const SourceFile& source_file)
        :InSourceError(position, source_file) {}

string UnclosedStringError::getMessage() {
    return "Unclosed string literal " +
        getLocationString() +
        ".";
}
#pragma endregion UnclosedStringError

#pragma region UnexpectedTokenError
UnexpectedTokenError::UnexpectedTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const string& str,
        const TokenType type)
            :InSourceError(position, source_file),
            token_str(str),
            token_type(type) {}

UnexpectedTokenError::UnexpectedTokenError(
        const SourceFile& source_file,
        const Token& token)
            :InSourceError(token.position, source_file),
            token_str(token.value),
            token_type(token.type) {}

string UnexpectedTokenError::getTokenStr() {return token_str;}

TokenType UnexpectedTokenError::getTokenType() {return token_type;}

string UnexpectedTokenError::getMessage() {
    return "Unexpected token " +
        getTokenStr() +
        " of type " +
        token_names[getTokenType()] +
        " " +
        getLocationString() +
        ".";
}
#pragma endregion UnexpectedTokenError

#pragma region UnknownTokenError
UnknownTokenError::UnknownTokenError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const string& str)
        :InSourceError(position, source_file),
        token_str(str) {}

string UnknownTokenError::getTokenStr() {return token_str;}

string UnknownTokenError::getMessage() {
    return "Unknown token " +
        getTokenStr() +
        " " +
        getLocationString() +
        ".";
}
#pragma endregion UnknownTokenError

#pragma region InvalidLiteralError
InvalidLiteralError::InvalidLiteralError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const string& literal_name)
        :InSourceError(position, source_file),
        literal_name(literal_name) {}

string InvalidLiteralError::getLiteralName() {return literal_name;}

string InvalidLiteralError::getMessage() {
    return "Invalid " +
        getLiteralName() +
        " literal " +
        getLocationString() +
        ".";
}
#pragma endregion InvalidLiteralError

#pragma region TooLongLiteralError
TooLongLiteralError::TooLongLiteralError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const string& literal_type_name,
    const int max_lenght)
        :InvalidLiteralError(position, source_file, literal_type_name),
        max_lenght(max_lenght) {}

int TooLongLiteralError::getMaxLenght() {return max_lenght;}

string TooLongLiteralError::getMessage() {
    return "Max lenght of " +
        getLiteralName() +
        " literal (" +
        to_string(getMaxLenght()) +
        ") was exceeded " +
        getLocationString() +
        ".";
}
#pragma endregion TooLongLiteralError
};
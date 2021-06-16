/**
 * error.h implementation
 *
 * @author EnderASz
 */
#include <saltc/error.h>
#include <saltc/utils.h>
#include <saltc/token.h>

namespace salt
{

// Custom error

CustomError::CustomError(const std::string& message): message(message) {}

std::string CustomError::getMessage() {
    return message;
}

// File open error

FileOpenError::FileOpenError(const std::string& filepath): filepath(filepath) {}

std::string FileOpenError::getFilepath() {
    return filepath;
}

std::string FileOpenError::getMessage() {
    return "An error occurred when tried to open '"
        + getFilepath() + "' file.";
}

// Command line error

std::string CommandLineError::getMessage() {
    return "An error occured in command line. "
        + getHelpRecomendation();
}

std::string CommandLineError::getHelpRecomendation() {
    return "For more information use --help.";
}

// Unspecified main error

std::string UnspecifiedMainError::getMessage() {
    return "Main file to compile is unspecified. Please specify it. "
        + getHelpRecomendation();
}

// Command line option error

CommandLineOptionError::CommandLineOptionError(const std::string& option)
    : option(option) {}

std::string CommandLineOptionError::getMessage() {
    return "A command line option '"
        + getOption()
        + "' cannot be interpreted correctly. "
        + getHelpRecomendation();
}

std::string CommandLineOptionError::getOption() {
    return option;
}

// Unrecognized option error

UnrecognizedOptionError::UnrecognizedOptionError(const std::string& option)
    : CommandLineOptionError(option) {}

std::string UnrecognizedOptionError::getMessage() {
    return "Unrecognized command line option '"
        + getOption()
        + "'. "
        + getHelpRecomendation();
}

// Missing option argument error

MissingOptionArgumentError::MissingOptionArgumentError(
    const std::string& option)
        : CommandLineOptionError(option) {}

std::string MissingOptionArgumentError::getMessage() {
    return "Missing argument of an option '"
        + getOption()
        + "'. "
        + getHelpRecomendation();
}

// Source error

SourceError::SourceError(const SourceFile& source_file)
    :source_file(const_cast<SourceFile*>(&source_file)) {}

SourceFile* SourceError::getSource() {
    return source_file;
}

std::string SourceError::getMessage() {
    return "An error occured in '" + getSource()->getFilePath() + "' source.";
}

// In source error

InSourceError::InSourceError(const InStringPosition& position,
    const SourceFile& source_file)
        : SourceError(source_file), position(position) {}

std::string InSourceError::getMessage() {
    return "An error occured in '"
        + getSource()->getFilePath()
        + "' at line "
        + std::to_string(getPosition().line_idx + 1)
        + " at position "
        + std::to_string(getPosition().inline_idx + 1)
        + ".";
}

InStringPosition InSourceError::getPosition() {
    return position;
}

std::string InSourceError::getLocationString() {
    return "in '"
        + getSource()->getFilePath()
        + "' at line "
        + std::to_string(getPosition().line_idx + 1)
        + " at position "
        + std::to_string(getPosition().inline_idx + 1);
}

// Out of source range error

OutOfSourceRangeError::OutOfSourceRangeError(const SourceFile& source_file)
    :SourceError(source_file) {}

std::string OutOfSourceRangeError::getMessage() {
    return "Tried to get token out of range '" +
        getSource()->getFilePath() +
        "' source.";
}

// Unclosed comment error

UnclosedCommentError::UnclosedCommentError(const InStringPosition& position,
    const SourceFile& source_file)
        : InSourceError(position, source_file) {}

std::string UnclosedCommentError::getMessage() {
    return "Unclosed comment "
        + getLocationString()
        + ".";
}

// Unclosed string error

UnclosedStringError::UnclosedStringError(const InStringPosition& position,
    const SourceFile& source_file)
        : InSourceError(position, source_file) {}

std::string UnclosedStringError::getMessage() {
    return "Unclosed std::string literal "
        + getLocationString()
        + ".";
}

// Unexpected token error

UnexpectedTokenError::UnexpectedTokenError(
        const InStringPosition& position,
        const SourceFile& source_file,
        const std::string& str,
        const TokenType type)
            :InSourceError(position, source_file),
            token_str(str),
            token_type(type) {}

UnexpectedTokenError::UnexpectedTokenError(const SourceFile& source_file,
    const Token& token)
        : InSourceError(token.position, source_file), token_str(token.value),
          token_type(token.type) {}

std::string UnexpectedTokenError::getTokenStr() {
    return token_str;
}

TokenType UnexpectedTokenError::getTokenType() {
    return token_type;
}

std::string UnexpectedTokenError::getMessage() {
    return "Unexpected token "
        + getTokenStr()
        + " of type "
        + token_names.at(getTokenType())
        + " "
        + getLocationString()
        + ".";
}

// Unknown token error

UnknownTokenError::UnknownTokenError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const std::string& str)
        : InSourceError(position, source_file),
          token_str(str) {}

std::string UnknownTokenError::getTokenStr() {
    return token_str;
}

std::string UnknownTokenError::getMessage() {
    return "Unknown token "
        + getTokenStr()
        + " "
        + getLocationString()
        + ".";
}

// Invalid literal error

InvalidLiteralError::InvalidLiteralError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const std::string& literal_name)
        : InSourceError(position, source_file),
          literal_name(literal_name) {}

std::string InvalidLiteralError::getLiteralName() {
    return literal_name;
}

std::string InvalidLiteralError::getMessage() {
    return "Invalid "
        + getLiteralName()
        + " literal "
        + getLocationString()
        + ".";
}

// Too long literal error

TooLongLiteralError::TooLongLiteralError(
    const InStringPosition& position,
    const SourceFile& source_file,
    const std::string& literal_type_name,
    const int max_lenght)
        : InvalidLiteralError(position, source_file, literal_type_name),
          max_lenght(max_lenght) {}

int TooLongLiteralError::getMaxLenght() {
    return max_lenght;
}

std::string TooLongLiteralError::getMessage() {
    return "Max lenght of "
        + getLiteralName()
        + " literal ("
        + std::to_string(getMaxLenght())
        + ") was exceeded "
        + getLocationString()
        + ".";
}

} // namespace salt

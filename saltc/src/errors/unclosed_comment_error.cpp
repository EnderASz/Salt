/**
 * errors/unclosed_comment_error.h implementation
 * 
 * @author EnderASz
 */
#include "../../include/errors/unclosed_comment_error.h"

#include <string>

using std::to_string;

namespace salt::errors
{

UnclosedCommentError::UnclosedCommentError(
    const uint line,
    const uint inline_position,
    const SourceFile& source_file)
        :CompilationError(line, inline_position, source_file) {}

UnclosedCommentError::UnclosedCommentError(
    const InStringPosition position,
    const SourceFile& source_file)
        :CompilationError(position, source_file) {}

const char* UnclosedCommentError::what() const throw() {
    string error_message = 
        "Unclosed comment in '" +
        this->source_file.getFilePath() +
        "' at line " +
        to_string(this->line) +
        " at position " +
        to_string(this->inline_position) +
        ".";

    return error_message.c_str();
}

}; // salt::errors
/**
 * UnclosedCommentError is an error throwing on detect unclosed comment.
 * 
 * @author EnderASz
 */
#ifndef UNCLOSED_COMMENT_ERROR_H_
#define UNCLOSED_COMMENT_ERROR_H_

#include "compilation_error.h"
#include "../source_file.h"
#include "../utils.h"
#include <string>

using std::string;

namespace salt::errors
{

class UnclosedCommentError: public CompilationError
{

public:
    UnclosedCommentError(
        const uint line,
        const uint inline_position,
        const SourceFile& source_file);

    UnclosedCommentError(
        const InStringPosition position,
        const SourceFile& source_file);

    const char* what() const throw ();
}; // salt::UnclosedCommentException

}; // salt::errors

#endif
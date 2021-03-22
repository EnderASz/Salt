/**
 * compilation_error.h implementation
 * 
 * @author EnderASz
 */
#include "../../include/errors/compilation_error.h"

#include "../../include/utils.h"
#include <exception>
#include <string>

using std::string;
using std::to_string;

namespace salt::errors
{

CompilationError::CompilationError(
    const uint line,
    const uint inline_position,
    const SourceFile& source_file)
        :line(line), inline_position(inline_position), source_file(source_file) {}

CompilationError::CompilationError(
    const InStringPosition position,
    const SourceFile& source_file)
        :line(position.line_idx+1),
        inline_position(position.inline_idx+1),
        source_file(source_file) {}

uint CompilationError::getLine() const {return this->line;}

uint CompilationError::getInlinePosition() const {return this->inline_position;}

const char* CompilationError::what() const throw() {
string error_message = 
    "Compilation error in '" +
    this->source_file.getFilePath() +
    "' at line " +
    to_string(this->line) +
    " at position " +
    to_string(this->inline_position) +
    ".";

return error_message.c_str();
}

}; // salt::errors
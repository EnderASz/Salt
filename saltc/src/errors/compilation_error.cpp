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
                                    const uint position,
                                    const SourceFile& source_file)
    :line(line), position(position), source_file(source_file) {}

uint CompilationError::getLine() const {return this->line;}

uint CompilationError::getPosition() const {return this->position;}

const char* CompilationError::what() const throw() {
string error_message = 
    "Compilation error in '" +
    this->source_file.getFilePath() +
    "' at line " +
    to_string(this->line) +
    " at position " +
    to_string(this->position) +
    ".";

return error_message.c_str();
}

}; // salt::errors
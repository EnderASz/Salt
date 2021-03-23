/**
 * CompilationError is an general compilation error class mainly
 * for inheritance purposes.
 * 
 * @author EnderASz
 */
#ifndef COMPILATION_ERROR_H_
#define COMPILATION_ERROR_H_

#include "../utils.h"
#include "../source_file.h"
#include <string>
#include <exception>

using std::string;

namespace salt::errors
{

class CompilationError: public std::exception
{
protected:

    /* Line number in code where error occured. */
    const uint line;

    /* In line index where error occured. */
    const uint inline_position;

    /* Source file where error occured. */
    const SourceFile& source_file;

public:

    /**
     * Explicit CompilationError constructor, initializes an compilation
     * error occured in source file at given line and position.
     */ 
    explicit CompilationError(
        const uint line,
        const uint inline_position,
        const SourceFile& source_file);

    explicit CompilationError(
        const InStringPosition position,
        const SourceFile& source_file);

    /* Getter for protected 'line' member. */
    uint getLine() const;

    /* Getter for protected 'inline_position' member. */
    uint getInlinePosition() const;

    /* Virtual method inherited from std::exception. */
    virtual const char* what() const throw() = 0;

}; // salt::CompilationError

}; // salt

#endif // COMPILATION_ERROR_H_
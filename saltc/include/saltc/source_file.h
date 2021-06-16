#ifndef SOURCE_FILE_H_
#define SOURCE_FILE_H_

#include "utils.h"

#include <stdint.h>
#include <string>
#include <vector>
#include <array>

namespace salt
{

class SourceFile
{
private:

    const std::string filename;
    const std::string filepath;
    struct {
        bool include_builtins = false;
    } meta;

public:

    SourceFile(std::string filepath);

    std::string code;

    /**
     * Returns source file name
     */
    std::string getFilename() const;

    /**
     * Gets source file path
     */
    std::string getFilePath() const;

    /**
     * Sets the use builtins flag to true.
     */
    void includeBuiltins();

};

}; // namespace salt

#endif
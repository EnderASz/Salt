#ifndef SOURCE_FILE_H_
#define SOURCE_FILE_H_

#include <string>
#include <vector>
#include <array>
#include <stdint.h>
#include "utils.h"

using std::string;

namespace salt
{

class SourceFile
{

public:
    SourceFile(string filepath);

    string code;

    /* Gets source file name */
    string getFilename() const;

    /* Gets source file path */
    string getFilePath() const;

    /* Toogle global import 'init' standard library on */
    void includeBuiltins();

private:
    const string filename;
    const string filepath;
    struct {
        bool include_builtins = false;
    } meta;

}; // salt::SourceFile

}; // salt

#endif // SOURCE_FILE_H_
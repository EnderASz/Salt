#ifndef SOURCE_FILE_H_
#define SOURCE_FILE_H_

#include <string>
#include <vector>
#include <array>
#include "utils.h"

using std::string;

namespace salt
{

class SourceFile
{
private:

    const string filename;
    const string filepath;
    struct {
        bool include_builtins = false;
    } meta;

public:


    SourceFile(string filepath);

    string code;

    /* Gets source file name */
    string getFilename() const;

    /* Gets source file path */
    string getFilePath() const;

    /* Toogle global import 'init' standard library on */
    void includeBuiltins();

    /* Returns SCC file header for this source file */
    std::array<byte, 64> makeSCCHeader();

    /* Return SCC file body for this source file */
    std::vector<byte> makeSCCBody();

}; // salt::SourceFile

}; // salt

#endif // SOURCE_FILE_H_
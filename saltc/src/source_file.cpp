/**
 * source_file.h implementation
 * 
 * @author EnderASz
 */
#include "../include/source_file.h"

#include "../include/utils.h"
#include "../include/logging.h"
#include <filesystem>
#include <string>
#include <cstring>
#include <array>

using std::string;
using std::memcpy;
using std::filesystem::path;

namespace salt
{

    SourceFile::SourceFile(string filepath)
        :filename(path(filepath).filename().string()), filepath(filepath) {
            dprint("Initializing '%s' source file object", filepath.c_str());
            this->code = load_file(filepath);
            dprint("Source code loaded");
    }

    string SourceFile::getFilename() const {return filename;}
    
    string SourceFile::getFilePath() const {return filepath;}
    
    void SourceFile::includeBuiltins() {meta.include_builtins = true;}

} // salt
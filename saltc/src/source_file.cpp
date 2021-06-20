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
using std::filesystem::path;

namespace salt
{

SourceFile::SourceFile(const string& filepath)
    : filename(path(filepath).filename().string()),
      filepath(filepath)
    {
        dprint("Initializing '%s' source file object", filepath.c_str());
        this->code = load_file(filepath);
        dprint("Source code loaded");
    }

const string& SourceFile::getFilename() const {return filename;}

const string& SourceFile::getFilePath() const {return filepath;}

void SourceFile::includeBuiltins() {meta.include_builtins = true;}

} // salt
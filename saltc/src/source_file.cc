/**
 * source_file.h implementation
 *
 * @author EnderASz
 */
#include <saltc/source_file.h>
#include <saltc/logging.h>
#include <saltc/utils.h>

#include <filesystem>
#include <cstring>
#include <array>

namespace salt
{

    SourceFile::SourceFile(std::string filepath)
        : filename(std::filesystem::path(filepath).filename().std::string()),
          filepath(filepath) {

            dprint("Initializing '%s' source file object", filepath.c_str());
            this->code = load_file(filepath);
            dprint("Source code loaded");
    }

    std::string SourceFile::getFilename() const {
        return filename;
   }

    std::string SourceFile::getFilePath() const {
        return filepath;
    }

    void SourceFile::includeBuiltins() {
        meta.include_builtins = true;
    }

} // namespace salt
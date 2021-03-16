/**
 * source_file.h implementation
 * 
 * @author EnderASz
 */
#include "../include/source_file.h"

#include "../include/utils.h"
#include "../include/compiler_metadata.h"
#include "../include/scc/synthesizer.h"
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
        :filepath(filepath), filename(path(filepath).filename().string()) {
            this->code = load_file(filepath);
    }

    string SourceFile::getFilename() const {return this->filename;}
    
    string SourceFile::getFilePath() const {return this->filepath;}
    
    void SourceFile::includeBuiltins() {this->meta.include_builtins = true;}
    
    std::array<byte, 64> SourceFile::makeSCCHeader() {
        std::array<byte, 64> header;
        header.fill('\00');
        header.data();
        memcpy(header.data(), CompilerMetadata::SCC_HEADER.data(), 6);
        memcpy(header.data()+8, CompilerMetadata::SCC_VERSION.data(), 2);
        memcpy(
            header.data()+16,
            Synthesizer::makeNum(this->meta.instructions).data(),
            4);
        //memcpy(header.data()+24, 'this->string_literals_amount', 4); ??
        memcpy(
            header.data()+32,
            Synthesizer::makeNum(this->meta.max_instruction_width).data(),
            4);
        memcpy(
            header.data()+56,
            CompilerMetadata::COMPILER_SIGNATURE.data(),
            8);
        return header;
    }
} // salt
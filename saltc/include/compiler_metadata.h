/**
 * The CompilerMetadata static class contains metadata
 * about compiler and language.
 */
#ifndef COMPILER_META_H_
#define COMPILER_META_H_

#include "utils.h"

#include <string>
#include <array>
#include <vector>
using std::string;

namespace salt
{
    class CompilerMetadata
    {
    public:
        /* Magic string, indicates that is indeed a SCC file. */
        static const std::array<byte, 6> SCC_HEADER;

        /* Format version identificator */
        static const std::array<byte, 2> SCC_VERSION;

        /* Compiler signature */
        static const std::array<byte, 8> COMPILER_SIGNATURE;
    };

};

#endif // COMPILER_META_H_
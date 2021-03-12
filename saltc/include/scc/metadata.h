/**
 * The Metadata static class contains metadata
 * about compiler, language and program.
 */
#ifndef META_H_
#define META_H_

#include "../utils.h"

#include <string>
using std::string;

namespace salt::scc
{
    class Metadata
    {
    private:
        /* Magic string, indicates that is indeed a SCC file. */
        static const std::array<byte, 6> SCC_HEADER;

        /* Format version identificator */
        static const std::array<byte, 2> SCC_VERSION;

        /* Compiler signature */
        static const std::array<byte, 8> COMPILER_SIGNATURE;

        int instructions_amount;

        int string_literals_amount;

    public:
        static std::array<byte, 6> getSCCHeader();
        static std::array<byte, 2> getSCCVersion();
        static std::array<byte, 8> getCompilerSignature();

        byte* getSCCFileHeader();
    };

};

#endif // META_H_
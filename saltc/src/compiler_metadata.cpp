/**
 * meta.h implementation
 * 
 */
#include "../include/compiler_metadata.h"

#include "../include/utils.h"
#include "../include/scc/synthesizer.h"
#include <stdint.h>
#include <array>

namespace salt
{

    /* Magic string, indicates that is indeed a SCC file. "\x7fSCC\xFF\xEE"*/
    const std::array<byte, 6> CompilerMetadata::SCC_HEADER = {
        '\x7F', 'S', 'C', 'C', '\xFF', '\xEE'};

    /* Format version identificator */
    const std::array<byte, 2> CompilerMetadata::SCC_VERSION =
        ptr_to_array<byte, 2>(Synthesizer::makeNum((uint16_t) 1).data());
        /*                     format version number (uint16) ^         */

    /* Compiler signature */
    const std::array<byte, 8> CompilerMetadata::COMPILER_SIGNATURE = {
        '\x7f','\x7a','\x7b','\x7c','\x00','\x00','\x00','\x0a'};
        /*                             xx     xx     xx         */

}
/**
 * meta.h implementation
 * 
 */
#include "../../include/scc/metadata.h"

#include "../../include/utils.h"
#include <array>

namespace salt::scc
{

    /* Magic string, indicates that is indeed a SCC file. "\x7fSCC\xFF\xEE"*/
    const std::array<byte, 6> Metadata::SCC_HEADER = {
        '\x7F', 'S', 'C', 'C', '\xFF', '\xEE'};

    /* Format version identificator */
    const std::array<byte, 2> Metadata::SCC_VERSION = {'\x01', '\x00'};

    /* Compiler signature */
    const std::array<byte, 8> Metadata::COMPILER_SIGNATURE = {
        '\x7f','\x7a','\x7b','\x7c','\x00','\x00','\x00','\x0a'};
        /*                             xx     xx     xx         */

}
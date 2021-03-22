/**
 * Enumerator and map contains special characters processable by saltc.
 * 
 * @author EnderASz
 */
#ifndef SPECIAL_CHAR_H_
#define SPECIAL_CHAR_H_

#include <map>

namespace salt::tokenizer {

    enum SpecialChar {
        BACKSPACE = 'b',
        TAB = 't',
        NEW_LINE = 'n',
        VERTICAL_TAB = 'v',
        CARRIAGE_RETURN = 'r',

        QUOTATION_MARK = '"',
        APOSTROPHE = '\'',
        BACKSLASH = '\\',
        
        BELL = 'a',
        PAGE_BREAK = 'f',

        ESCAPE_CHARACTER = 'e'
    }; // salt::tokenizer::SpecialChar

    std::map<char, SpecialChar> special_chars {
        {'b', BACKSLASH},
        {'t', TAB},
        {'n', NEW_LINE},
        {'v', VERTICAL_TAB},
        {'r', CARRIAGE_RETURN},
        
        {'"', QUOTATION_MARK},
        {'\'', APOSTROPHE},
        {'\\', BACKSLASH},
        
        {'a', BELL},
        {'f', PAGE_BREAK},

        {'e', ESCAPE_CHARACTER},
    }; // salt::tokenizer::special_chars

}; // salt::tokenizer

#endif
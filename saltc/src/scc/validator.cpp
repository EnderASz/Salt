/**
 * validator.h implementation
 *
 * @author bellrise
 */
#include "../../include/scc/validator.h"
#include <stdio.h>
#include <string.h>
#include <iostream>


namespace salt
{

    // PUBLIC

    Validator::Validator(std::string& bytecode)
    {
        this->bytecode = bytecode;
    }

    Validator::Validator(std::vector<byte>& bytecode)
    {
        std::string code;
        for (byte& chr : bytecode)
            code += chr;
        Validator((std::string&) code);
    }

    void Validator::setErrorCallback(void (*__callback)(int))
    {
        is_callback_set = true;
        f_callback = __callback;
    }

    void Validator::validate()
    {
        try {
            runValidation();
        } catch (ValidatorError e) {
            if (is_callback_set)
                f_callback(e);
            else
                throw e;
        }
    }

    // PRIVATE

    void Validator::runValidation()
    {
        loadHeader();
        checkMagic();

        if (max_instruction_width % 16 != 0)
            throw ValidatorError::instruction_width_violation;

        uint pos = checkConstStrings();
        checkInstructions(pos);

        printf("Amount of instructions: %d\n", instruction_amount);
        printf("Amount of const strings: %d\n", cstring_amount);
    }

    uint Validator::checkConstStrings()
    {
        uint cursor = 64;
        uint len;

        std::string buf;

        for (uint i = 0; i < cstring_amount; i++) {
            len = getUint(cursor) + 4;
        
            buf = bytecode.substr(cursor, len);
            if (buf.find('\n') != std::string::npos)
                throw ValidatorError::newline_in_string;

            if (bytecode[cursor + len] != '\n')
                throw ValidatorError::const_string_size;
            
            cursor += len + 1; 
        }

        return cursor;
    }

    void Validator::checkMagic()
    {
        std::string top = bytecode.substr(0, 8);

        // wtf.. I can't use std::string operator== here
        if (strncmp(MAGIC.c_str(), top.c_str(), 8) != 0)
            throw ValidatorError::invalid_header;
    }

    void Validator::checkInstructions(uint __n)
    {
        std::string ins;

        for (uint i = 0; i < instruction_amount; i++) {
            ins = getInstruction(__n);
            __n += ins.size() + 1;

            // Just check if the compiler didn't do anything stupid...
            if (ins.size() + 1 >= max_instruction_width)
                throw ValidatorError::instruction_width_violation;
            
            // If this checks, the compiler is very drunk so im just
            // crashing at this point.
            if (ins.size() < 5) {
                std::cout << "what just happened\n";
                exit(1);
            }
        }
    }

    void Validator::loadHeader()
    {
        if (bytecode.size() <= 64)
            throw ValidatorError::invalid_header;

        instruction_amount = getUint(16);
        cstring_amount = getUint(24);
        max_instruction_width = getUint(32);
    }

    void Validator::printBytes(char *__b, uint __n)
    {
        for (uint i = 0; i < __n; i++) {
            printf("%02hhx", __b[i]);
            if (i % 2 != 0)
                printf(" ");
        }
        printf("\n");
    }

    std::string Validator::getHex(char __c)
    {
        char buf[5];
        snprintf(buf, 5, "0x%02hhx", __c);
        return std::string(buf);
    }

    uint Validator::getUint(uint __n)
    {
        if (__n + 4 >= bytecode.size())
            throw ValidatorError::invalid_data_width;
            
        return * (uint *) &bytecode.c_str()[__n];
    }

    std::string Validator::getInstruction(uint __n)
    {
        int endl = bytecode.find('\n', __n);
        if (endl == std::string::npos)
            throw ValidatorError::nonterminated_instruction;
        
        return bytecode.substr(__n, endl - __n); 
    }

} // salt
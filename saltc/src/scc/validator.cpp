/**
 * validator.h implementation
 *
 * @author bellrise
 */
#include "../../include/scc/validator.h"
#include <algorithm>
#include <vector>
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

        checkConstStrings();

        printf("Amount of instructions: %d\n", instruction_amount);
        printf("Amount of const strings: %d\n", cstring_amount);
    }

    void Validator::checkConstStrings()
    {
        uint cursor = 64;
        uint len;

        for (uint i = 0; i < cstring_amount; i++) {
            len = getUint(cursor) + 5;

            if (bytecode[cursor + len] != '\n')
                throw ValidatorError::const_string_size;
            cursor += len + 1; 
        }
    }

    void Validator::checkMagic()
    {
        std::string top = bytecode.substr(0, 8);

        // wtf.. I can't use std::string operator== here
        if (strncmp(MAGIC.c_str(), top.c_str(), 8) != 0)
            throw ValidatorError::invalid_header;
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

} // salt
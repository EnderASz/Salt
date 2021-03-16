/**
 * This is the synthesizer module.
 *
 * 
 * @author bellrise
 */
#ifndef SYNTHESIZER_H_
#define SYNTHESIZER_H_

#include <string>
#include <vector>
#include <array>

#include "../utils.h"

namespace salt
{

    enum SaltType
    {
        Null   = 0x00,
        Int    = 0x01,
        Float  = 0x02,
        String = 0x03,
        Bool   = 0x04,
        Array  = 0x05
    };

    /**
     * The synthesizer class is responsible for generating SCC bytecode.  See
     * Synthesizer.FORMAT for the current used SCC format. This class should be
     * initialzed before use. Every method that generates a SVM call returns a
     * full instruction, with newline formatting so no external work has to be
     * done.
     *
     * Each SVM call method has it's desription above the definition, please
     * your time to read that before using it.
     */
    class Synthesizer
    {
    public:

        /* Current version of the SCC format. */
        static const int FORMAT = 1;

        /** 
         * Print the ID of the object (without a newline). 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        std::vector<byte> dumpi(uint id);

        /**
         * Print the value of the object to standard out  (without a newline). 
         * Note that this is based on a switch case statement reading the type 
         * of the object to call the correct function,  so it's a lot slower 
         * than just calling PRINT. 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        std::vector<byte> dumpv(uint id);

        /**
         * Load external dynamic library.
         *
         * @param   name  name of the external library 
         * @return  svm call instruction
         */
        std::vector<byte> extld(std::string name);

        /**
         * Brutally kill the program and all its threads stopping the execution
         * and immediately calling core_clean. Note that this is not the pre-
         * -ffered way of stopping code execution. 
         *
         * @return  svm call instruction
         */
        std::vector<byte> killx();

        /**
         * Print a constant string to standard out. This is the preffered way
         * of printing constant strings, because it is much faster compared to
         * calling DUMPV. 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        std::vector<byte> print(uint id);

        /** 
         * Sleep the given amount of miliseconds. 
         *
         * @param   ms  miliseconds to sleep
         * @return  svm call instruction
         */
        std::vector<byte> sleep(uint ms);

        /**
         * Create a new object with the specified destination id and copy the 
         * object from the source id to it. 
         *
         * @param   dest  destination object ID
         * @param   src   source object ID
         * @return  svm call instruction
         */
        std::vector<byte> rxcpy(uint dest, uint src);

        /**
         * Delete the object by freeing the allocated memory for the value and 
         * removing the ID from the register. 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        std::vector<byte> rxdel(uint id);

        /**
         * Create a new object and allocate a sufficent amount of memory. 
         *
         * @param   id     object ID
         * @param   val    value
         * @param   const_ (optional) if the created object should be marked
         *                 as constant
         * @return  svm call instruction
         */
        std::vector<byte> rxnew(uint id, bool const_);
        std::vector<byte> rxnew(uint id, int val, bool const_);
        std::vector<byte> rxnew(uint id, float val, bool const_);
        std::vector<byte> rxnew(uint id, std::string val, bool const_);    
        std::vector<byte> rxnew(uint id, bool val, bool const_);
        
        /**
         * Assign a new value to the object. This replaces the previous value
         * with the new one. Note that the SVM checks if the type is still the
         * same for the value and object.
         *
         * @param   id  object ID
         * @param   val value to s
         * @return  svm call instruction
         */
        std::vector<byte> rxset(uint id);
        std::vector<byte> rxset(uint id, int val);
        std::vector<byte> rxset(uint id, float val);
        std::vector<byte> rxset(uint id, std::string val);
        std::vector<byte> rxset(uint id, bool val);

        // Synthesizing
        
        /**
         * Convert the given int, uint, double or float type into the SVM byte 
         * representation of such type. 
         *
         * @param   T   can be int, uint, double or float
         * @param   val value 
         * @return  array of bytes representing the value
         */
        template<typename T>
        std::vector<byte> makeNum(T val);
        
        /**
         * Convert the string or boolean value of the type into the SVM byte
         * representation
         *
         * @param   val value
         * @return  array of bytes representing the value
         */
        std::vector<byte> makeString(std::string val);
        std::vector<byte> makeBool(bool val);

    private:

        /* Create an empty collector with the given label */
        std::vector<byte> makeLabel(const char label[6]);

        /* Construct a full instruction from the passed bytes */
        std::vector<byte> make(const char label[6], std::vector<byte> bytes);
        std::vector<byte> make(const char label[6]);

        /* Push the raw bytes into the vector */
        void pushBytes(std::vector<byte>& collector, std::string bytes);
        void pushBytes(std::vector<byte>& collector, std::vector<byte> bytes);

        /* Make a RXNEW svm call */
        std::vector<byte> makeRxnew(uint id, SaltType type, 
                          std::vector<byte> value, bool const_);
        std::vector<byte> makeRxset(uint id, SaltType type, 
                          std::vector<byte> value);

    };

} // salt
#endif // SYNTHESIZER_H_
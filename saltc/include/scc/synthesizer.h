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
     * initialized before use. Every method that generates a SVM call returns a
     * full instruction, with newline formatting so no external work has to be
     * done.
     *
     * Each SVM call method has it's description above the definition, please
     * your time to read that before using it.
     */
    class Synthesizer
    {
    public:

        /* Current version of the SCC format. */
        static const int FORMAT = 1;

        /* if the object should be read-only */
        constexpr static byte CONSTANT_FALSE = 0x00;
        constexpr static byte CONSTANT_TRUE  = 0x01;

        /* access level (the scope is the module) */
        constexpr static byte ACCESS_PUBLIC  = 0x00;
        constexpr static byte ACCESS_PRIVATE = 0x01;

        /* type of the object  */
        constexpr static byte TYPE_NULL      = 0x00;
        constexpr static byte TYPE_INT       = 0x01;
        constexpr static byte TYPE_FLOAT     = 0x02;
        constexpr static byte TYPE_BOOL      = 0x03;
        constexpr static byte TYPE_STRING    = 0x04;

        /* prepare the object for threading support */
        constexpr static byte THREADED_FALSE = 0x00;
        constexpr static byte THREADED_TRUE  = 0x01;

        /** 
         * Print the ID of the object (without a newline). 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        static std::vector<byte> dumpi(uint id);

        /**
         * Print the value of the object to standard out  (without a newline). 
         * Note that this is based on a switch case statement reading the type 
         * of the object to call the correct function,  so it's a lot slower 
         * than just calling PRINT. 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        static std::vector<byte> dumpv(uint id);

        /**
         * Load external dynamic library.
         *
         * @param   name  name of the external library 
         * @return  svm call instruction
         */
        static std::vector<byte> extld(std::string name);

        /**
         * Brutally kill the program and all its threads stopping the execution
         * and immediately calling core_clean. Note that this is not the pre-
         * -ffered way of stopping code execution. 
         *
         * @return  svm call instruction
         */
        static std::vector<byte> killx();

        /**
         * Print a constant string to standard out. This is the preferred way
         * of printing constant strings, because it is much faster compared to
         * calling `DUMPV`.
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        static std::vector<byte> print(uint id);

        /** 
         * Sleep the given amount of milliseconds.
         *
         * @param   ms  milliseconds to sleep
         * @return  svm call instruction
         */
        static std::vector<byte> sleep(uint ms);

        /**
         * Create a new object with the specified destination id and copy the 
         * object from the source id to it. 
         *
         * @param   dest  destination object ID
         * @param   src   source object ID
         * @return  svm call instruction
         */
        static std::vector<byte> rxcpy(uint dest, uint src);

        /**
         * Delete the object by freeing the allocated memory for the value and 
         * removing the ID from the register. 
         *
         * @param   id  object ID
         * @return  svm call instruction
         */
        static std::vector<byte> rxdel(uint id);

        /**
         * Create a new object and allocate a sufficent amount of memory. 
         *
         * @param   id     object ID
         * @param   val    value
         * @param   const_ (optional) if the created object should be marked
         *                 as constant
         * @return  svm call instruction
         */
        static std::vector<byte> rxnew(uint id, bool const_);
        static std::vector<byte> rxnew(uint id, int val, bool const_);
        static std::vector<byte> rxnew(uint id, float val, bool const_);
        static std::vector<byte> rxnew(uint id, std::string val, bool const_);    
        static std::vector<byte> rxnew(uint id, bool val, bool const_);
        
        /**
         * Assign a new value to the object. This replaces the previous value
         * with the new one. Note that the SVM checks if the type is still the
         * same for the value and object.
         *
         * @param   id  object ID
         * @param   val value to s
         * @return  svm call instruction
         */
        static std::vector<byte> rxset(uint id);
        static std::vector<byte> rxset(uint id, int val);
        static std::vector<byte> rxset(uint id, float val);
        static std::vector<byte> rxset(uint id, std::string val);
        static std::vector<byte> rxset(uint id, bool val);

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
        static std::vector<byte> makeNum(T val)
        {
            std::vector<byte> collector;
            for (short i = 0; i < (short) sizeof(T); i++) {
                collector.push_back(((byte *) &val)[i]);
            }
                
            return collector;
        }
        
        /**
         * Convert the string or boolean value of the type into the SVM byte
         * representation
         *
         * @param   val value
         * @return  array of bytes representing the value
         */
        static std::vector<byte> makeString(std::string val);
        static std::vector<byte> makeBool(bool val);

    private:

        /* Create an empty collector with the given label */
        static std::vector<byte> makeLabel(const char label[6]);

        /* Construct a full instruction from the passed bytes */
        static std::vector<byte> make(const char label[6], std::vector<byte> bytes);
        static std::vector<byte> make(const char label[6]);

        /* Push the raw bytes into the vector */
        static void pushBytes(std::vector<byte>& collector, std::string bytes);
        static void pushBytes(std::vector<byte>& collector, std::vector<byte> bytes);

        /* Make a RXNEW svm call */
        static std::vector<byte> makeRxnew(uint id, SaltType type, 
                          std::vector<byte> value, bool const_);
        static std::vector<byte> makeRxset(uint id, SaltType type, 
                          std::vector<byte> value);

    };

} // salt

#endif // SYNTHESIZER_H_

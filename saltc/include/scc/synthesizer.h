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
        static const int FORMAT = 3;

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
         * Call a function and push the current position and call onto the
         * stack pointer. Note that this only works for local functions in the
         * same module, and should be used when possible because it's a lot
         * faster than the alternative, CALLX. The only argument is the label
         * name, so if you want to call a function rendered as  @function_name,
         * you call: CALLFxxxxxxxxfunction_name\0.
         *
         * @param   function  name of function
         * @return  synthesized bytes
         */
        static std::vector<byte> callLocal(std::string function);

        /**
         * Exit the current executed module.
         *
         * @return  synthesized bytes
         */
        static std::vector<byte> exit();

        /**
         * Load an external SCC file and add it to the global module register.
         *
         * @param   module  name of the module to load, without the .scc
         *                  extension.
         * @return  synthesized bytes
         */
        static std::vector<byte> externalLoad(std::string module);

        /**
         * Kill the whole program on-the-spot. This tried to free any memory it
         * can as fast as possible, and this kills the whole program. Note that
         * this almost certainly produce unwanted memory leaks, so it's
         * recommended not to use it.
         *
         * @return  synthesized bytes
         */
        static std::vector<byte> kill();

        /**
         * All these methods create a single object local to the module.
         * Depending on the access modifier.
         *
         * @param   id        ID of the object.
         * @param   readonly  true if the variable should be constant
         * @return  synthesized bytes
         */
        static std::vector<byte> objectMake(uint id, bool readonly);
        static std::vector<byte> objectMake(uint id, bool readonly,
                                            int value);
        static std::vector<byte> objectMake(uint id, bool readonly,
                                            float value);
        static std::vector<byte> objectMake(uint id, bool readonly,
                                            bool value);
        static std::vector<byte> objectMake(uint id, bool readonly,
                                            std::string value);

        /**
         * Delete the object in the current module. This doesn't actually
         * remove the object from memory, but signs it as "inactive". The
         * virtual machine decides when to actually free the memory and
         * remove the object permanently.
         *
         * @param   id  ID of the object to remove
         * @return  synthesized bytes
         */
        static std::vector<byte> objectDelete(uint id);

        /**
         * Print the value of the object (without a newline). Most prints
         * require a newline anyway, so it is a good idea for the compiler
         * to create a readonly string with only a newline character, and
         * chain the PRINT object & PRINT const_newline.
         *
         * @param   id  ID of the object
         * @return  synthesized bytes
         */
        static std::vector<byte> print(uint id);

        /**
         * Return from the current function, popping a function off the
         * callstack.
         *
         * @return  synthesized bytes
         */
        static std::vector<byte> return_();

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
        static std::vector<byte> makeNum(T value)
        {
            std::vector<byte> collector;
            for (short i = 0; i < (short) sizeof(T); i++) {
                collector.push_back(((byte *) &value)[i]);
            }

            return collector;
        }

        static std::vector<byte> makeString(std::string value);
        static std::vector<byte> makeBool(bool value);

    private:

        /* Create a full instruction adding the newline at the end */
        static std::vector<byte> make(const char instruction[6],
                                      std::vector<byte> bytes);

        /* Push raw bytes onto the collector */
        static void pushBytes(std::vector<byte>& collector,
                              std::string bytes);
        static void pushBytes(std::vector<byte>& collector,
                              std::vector<byte> bytes);

        /* I'm sorry for this formatting, but it won't fit otherwise. */
        static void pushObjectData(std::vector<byte>& collector, uint id,
                                   bool readonly, bool threading, byte type,
                                   std::vector<byte> value);

    };

} // salt

#endif // SYNTHESIZER_H_

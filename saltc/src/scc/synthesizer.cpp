/**
 * synthesizer.h implementation
 *
 * @author bellrise
 */

#include "../../include/scc/synthesizer.h"
#include <vector>
#include <iostream>

namespace salt
{

    std::vector<byte> Synthesizer::dumpi(uint id)
    { return make("DUMPI", makeNum<uint>(id)); }

    std::vector<byte> Synthesizer::dumpv(uint id)
    { return make("DUMPV", makeNum<uint>(id)); }

    std::vector<byte> Synthesizer::extld(std::string name)
    { return make("EXTLD", makeString(name)); }

    std::vector<byte> Synthesizer::killx()
    { return make("KILLX"); }

    std::vector<byte> Synthesizer::print(uint id)
    { return make("PRINT", makeNum<uint>(id)); }

    std::vector<byte> Synthesizer::sleep(uint ms)
    { return make("SLEEP", makeNum<uint>(ms)); }

    std::vector<byte> Synthesizer::rxcpy(uint dest, uint src)
    {
        std::vector<byte> collector = makeLabel("RXCPY");
        pushBytes(collector, makeNum<uint>(dest));
        pushBytes(collector, makeNum<uint>(src));
        collector.push_back('\n');
        return collector;
    }

    std::vector<byte> Synthesizer::rxdel(uint id)
    { return make("RXDEL", makeNum<uint>(id)); }

    // RXNEW

    std::vector<byte> Synthesizer::rxnew(uint id, bool const_ = false)
    {
        std::vector<byte> null_;
        return makeRxnew(id, SaltType::Null, null_, const_);
    }

    std::vector<byte> Synthesizer::rxnew(uint id, int val, 
                      bool const_ = false)
    { 
        return makeRxnew(id, SaltType::Int, makeNum<int>(val), const_); 
    }

    std::vector<byte> Synthesizer::rxnew(uint id, float val, 
                      bool const_ = false)
    { 
        return makeRxnew(id, SaltType::Float, makeNum<float>(val), const_); 
    }

    std::vector<byte> Synthesizer::rxnew(uint id, std::string val, 
                      bool const_ = false)
    { 
        return makeRxnew(id, SaltType::String, makeString(val), const_);
    }

    std::vector<byte> Synthesizer::rxnew(uint id, bool val, 
                      bool const_ = false)
    { 
        return makeRxnew(id, SaltType::Bool, makeBool(val), const_); 
    }

    // RXSET

    std::vector<byte> Synthesizer::rxset(uint id)
    { 
        std::vector<byte> null_;
        null_.push_back(SaltType::Null);
        return makeRxset(id, SaltType::Null, null_);
    }

    std::vector<byte> Synthesizer::rxset(uint id, int val)
    { 
        return makeRxset(id, SaltType::Int, makeNum<int>(val)); 
    }

    std::vector<byte> Synthesizer::rxset(uint id, float val)
    { 
        return makeRxset(id, SaltType::Float, makeNum<float>(val)); 
    }

    std::vector<byte> Synthesizer::rxset(uint id, std::string val)
    { 
        return makeRxset(id, SaltType::String, makeString(val)); 
    }

    std::vector<byte> Synthesizer::rxset(uint id, bool val)
    { 
        return makeRxset(id, SaltType::Bool, makeBool(val)); 
    }

    // make

    template<typename T>
    std::vector<byte> Synthesizer::makeNum(T val)
    {
        std::vector<byte> collector;
        for (short i = 0; i < (short) sizeof(T); i++) {
            collector.push_back(((byte *) &val)[i]);
        }
            
        return collector;
    }

    std::vector<byte> Synthesizer::makeString(std::string val)
    {
        std::vector<byte> collector;
        pushBytes(collector, makeNum<uint>(val.size() + 1));
        for (char& chr : val) {
            if (chr == '\n')
                collector.push_back('\x11');
            else
                collector.push_back(chr);
        }
        collector.push_back(0);
        return collector;
    }

    std::vector<byte> Synthesizer::makeBool(bool val)
    {
        std::vector<byte> collector;
        if (val)
            collector.push_back('\x01');
        else
            collector.push_back('\x00');
        return collector;
    }

    // private

    std::vector<byte> Synthesizer::makeLabel(const char label[6])
    {
        std::vector<byte> collector;
        for (short i = 0; i < 5; i++)
            collector.push_back(label[i]);
        return collector;
    }

    std::vector<byte> Synthesizer::make(const char label[6], 
                      std::vector<byte> bytes)
    {
        std::vector<byte> collector = makeLabel(label);
        for (byte& chr : bytes)
            collector.push_back(chr);
        collector.push_back('\n');
        return collector;
    }

    std::vector<byte> Synthesizer::make(const char label[6])
    {
        std::vector<byte> collector = makeLabel(label);
        collector.push_back('\n');
        return collector;
    }

    void Synthesizer::pushBytes(std::vector<byte>& collector, 
                                std::string bytes)
    {
        for (char& chr : bytes) {
            collector.push_back(chr);
        }
    }

    void Synthesizer::pushBytes(std::vector<byte>& collector, 
                                std::vector<byte> bytes)
    {
        for (byte& chr : bytes) {
            collector.push_back(chr);
        }
    }

    std::vector<byte> Synthesizer::makeRxnew(uint id, SaltType type, 
                      std::vector<byte> value, bool const_)
    {
        std::vector<byte> collector = makeLabel("RXNEW");
        
        pushBytes(collector, makeNum<uint>(id));
        pushBytes(collector, makeBool(const_));
        collector.push_back(type);
        
        pushBytes(collector, value);

        collector.push_back('\n');
        return collector;
    }

    std::vector<byte> Synthesizer::makeRxset(uint id, SaltType type, 
                      std::vector<byte> value)
    {
        std::vector<byte> collector = makeLabel("RXSET");

        pushBytes(collector, makeNum<uint>(id));
        collector.push_back(type);

        pushBytes(collector, value);

        return collector;
    }


} // salt

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

std::vector<byte> Synthesizer::callLocal(std::string function)
{
    return make("CALLF", makeString(function));
}

std::vector<byte> Synthesizer::exit()
{
    return make("EXITE", std::vector<byte>());
}

std::vector<byte> Synthesizer::externalLoad(std::string module)
{
    return make("EXTLD", makeString(module));
}

std::vector<byte> Synthesizer::kill()
{
    return make("KILLX", std::vector<byte>());
}

std::vector<byte> Synthesizer::objectMake(uint id, bool readonly)
{
    std::vector<byte> collector;
    pushObjectData(collector, id, readonly, false, TYPE_NULL,
                   std::vector<byte>());
    return collector;
}

std::vector<byte> Synthesizer::objectMake(uint id, bool readonly, int value)
{
    std::vector<byte> collector;
    pushObjectData(collector, id, readonly, false, TYPE_INT,
                   makeNum<int>(value));
    return collector;
}

std::vector<byte> Synthesizer::objectMake(uint id, bool readonly, float value)
{
    std::vector<byte> collector;
    pushObjectData(collector, id, readonly, false, TYPE_FLOAT,
                   makeNum<float>(value));
    return collector;
}

std::vector<byte> Synthesizer::objectMake(uint id, bool readonly, bool value)
{
    std::vector<byte> collector;
    pushObjectData(collector, id, readonly, false, TYPE_BOOL,
                   makeBool(value));
    return collector;
}

std::vector<byte> Synthesizer::objectMake(uint id, bool readonly,
                                          std::string value)
{
    std::vector<byte> collector;
    pushObjectData(collector, id, readonly, false, TYPE_STRING,
                   makeString(value));
    return collector;
}

std::vector<byte> Synthesizer::objectDelete(uint id)
{
    return make("OBJDL", makeNum<uint>(id));
}

std::vector<byte> Synthesizer::return_()
{
    return make("RETRN", std::vector<byte>());
}

// private

std::vector<byte> Synthesizer::makeString(std::string value)
{
    std::vector<byte> collector;
    pushBytes(collector, makeNum<int>((int) value.size()));
    value.replace(value.begin(), value.end(), '\n', '\x11');
    pushBytes(collector, value);
    return collector;
}

std::vector<byte> Synthesizer::makeBool(bool value)
{
    std::vector<byte> collector;
    if (value)
        collector.push_back(CONSTANT_TRUE);
    else
        collector.push_back(CONSTANT_FALSE);
    return collector;
}

std::vector<byte> Synthesizer::make(const char instruction[6],
                                    std::vector<byte> bytes)
{
    std::vector<byte> collector;
    for (short i = 0; i < 5; i++)
        collector.push_back(instruction[i]);

    pushBytes(collector, bytes);

    collector.push_back('\n');
    return collector;
}

void Synthesizer::pushBytes(std::vector<byte>& collector,
                            std::string bytes)
{
    for (size_t i = 0; i < bytes.size(); i++)
        collector.push_back(bytes[i]);
}

void Synthesizer::pushBytes(std::vector<byte>& collector,
                            std::vector<byte> bytes)
{
    for (size_t i = 0; i < bytes.size(); i++)
        collector.push_back(bytes[i]);
}

void Synthesizer::pushObjectData(std::vector<byte>& collector, uint id,
                                 bool readonly, bool threading, byte type,
                                 std::vector<byte> value)
{
    // Don't look at that argument list please, look into my eyes and show
    // me a better way of doing this.
    pushBytes(collector, makeNum<uint>(id));
    pushBytes(collector, makeBool(readonly));
    collector.push_back(type);
    pushBytes(collector, value);
}


} // salt

#pragma once

#include <cstdint>


namespace elf::deserialization
{

struct TargetMachineInfo
{
    std::uint8_t magic[4];
    std::uint8_t bitVersion;
    std::uint8_t endianness;
    std::uint8_t elfVersion;
    std::uint8_t abi;
    std::uint8_t abiVersion;
    std::uint8_t padding[7];
};

}

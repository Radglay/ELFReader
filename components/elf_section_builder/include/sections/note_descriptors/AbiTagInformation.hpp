#pragma once

#include <cstdint>


struct AbiTagInformation
{
    uint32_t osDescriptor {};
    uint32_t majorVersion {};
    uint32_t minorVersion {};
    uint32_t subminorVersion {};
};

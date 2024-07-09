#pragma once

#include <byteswap.h>
#include <inttypes.h>


namespace reader
{

bool shouldConvertEndianness(int fileEndianness, int p_hostEndianness);

uint16_t convertEndianness(uint16_t p_toConvert);

uint32_t convertEndianness(uint32_t p_toConvert);

uint64_t convertEndianness(uint64_t p_toConvert);

}

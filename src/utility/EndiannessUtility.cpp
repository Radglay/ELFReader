#include "EndiannessUtility.hpp"


namespace reader
{

bool shouldConvertEndianness(int fileEndianness, int p_hostEndianness)
{
    return fileEndianness != fileEndianness;
}

uint16_t convertEndianness(uint16_t p_toConvert)
{
    return bswap_16(p_toConvert);
}

uint32_t convertEndianness(uint32_t p_toConvert)
{
    return bswap_32(p_toConvert);
}

uint64_t convertEndianness(uint64_t p_toConvert)
{
    return bswap_64(p_toConvert);
}

}

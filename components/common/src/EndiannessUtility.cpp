#include "EndiannessUtility.hpp"
#include <byteswap.h>
#include <elf.h>


int getHostEndianness()
{
    int l_hostEndianness { ELFDATA2LSB };

    if (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
        l_hostEndianness = ELFDATA2MSB;

    return l_hostEndianness;
}

bool isEndiannessCorrect(int p_endianness)
{
    return p_endianness == ELFDATA2LSB or p_endianness == ELFDATA2MSB;
}

bool shouldConvertEndianness(int p_fileEndianness, int p_hostEndianness)
{
    return p_fileEndianness != p_hostEndianness;
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

int64_t convertEndianness(int64_t p_toConvert)
{
    return bswap_64(p_toConvert);
}

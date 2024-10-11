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

bool shouldConvertEndianness(int p_fileEndianness)
{
    int l_hostEndianness = getHostEndianness();
    return p_fileEndianness != l_hostEndianness;
}

void convertEndianness(uint16_t& p_toConvert)
{
    p_toConvert = bswap_16(p_toConvert);
}

void convertEndianness(uint32_t& p_toConvert)
{
    p_toConvert = bswap_32(p_toConvert);
}

void convertEndianness(uint64_t& p_toConvert)
{
    p_toConvert = bswap_64(p_toConvert);
}

void convertEndianness(int64_t& p_toConvert)
{
    p_toConvert = bswap_64(p_toConvert);
}

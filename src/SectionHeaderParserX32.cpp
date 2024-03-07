#include "SectionHeaderParserX32.hpp"
#include <elf.h>
#include <cstring>
#include "SectionHeader.hpp"


namespace parser
{

SectionHeader SectionHeaderParserX32::parse(const char* p_headerBytes)
{
    SectionHeader l_shdr;
    std::memcpy(&l_shdr.header32, p_headerBytes, sizeof(Elf32_Shdr));
    return l_shdr;
}

}

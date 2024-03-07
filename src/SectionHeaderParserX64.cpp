#include "SectionHeaderParserX64.hpp"
#include <elf.h>
#include <cstring>
#include "SectionHeader.hpp"


namespace parser
{

SectionHeader SectionHeaderParserX64::parse(const char* p_headerBytes)
{
    SectionHeader l_shdr;
    std::memcpy(&l_shdr.header64, p_headerBytes, sizeof(Elf64_Shdr));
    return l_shdr;
}

}

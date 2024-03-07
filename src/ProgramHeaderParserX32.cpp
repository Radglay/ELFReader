#include "ProgramHeaderParserX32.hpp"
#include <elf.h>
#include <cstring>
#include "ProgramHeader.hpp"


namespace parser
{

ProgramHeader ProgramHeaderParserX32::parse(const char* p_headerBytes)
{
    ProgramHeader l_phdr;
    std::memcpy(&l_phdr.header32, p_headerBytes, sizeof(Elf32_Phdr));
    return l_phdr;
}

}

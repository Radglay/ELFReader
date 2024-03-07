#include "ProgramHeaderParserX64.hpp"
#include <elf.h>
#include <cstring>
#include "ProgramHeader.hpp"


namespace parser
{

ProgramHeader ProgramHeaderParserX64::parse(const char* p_headerBytes)
{
    ProgramHeader l_phdr;
    std::memcpy(&l_phdr.header64, p_headerBytes, sizeof(Elf64_Phdr));
    return l_phdr;
}

}

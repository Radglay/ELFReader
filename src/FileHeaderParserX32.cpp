#include "FileHeaderParserX32.hpp"
#include <elf.h>
#include <cstring>
#include "FileHeader.hpp"


namespace parser
{

FileHeader FileHeaderParserX32::parse(const char* p_headerBytes)
{
    FileHeader l_ehdr;
    std::memcpy(&l_ehdr.header32, p_headerBytes, sizeof(Elf32_Ehdr));
    return l_ehdr;
}

}

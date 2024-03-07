#include "FileHeaderParserX64.hpp"
#include <elf.h>
#include <cstring>
#include "FileHeader.hpp"


namespace parser
{

FileHeader FileHeaderParserX64::parse(const char* p_headerBytes)
{
    FileHeader l_ehdr;
    std::memcpy(&l_ehdr.header64, p_headerBytes, sizeof(Elf64_Ehdr));
    return l_ehdr;
}

}

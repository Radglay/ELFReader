#include "ElfFileParserX64.hpp"
// #include <elf.h>
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"


namespace parser
{

ElfFileParserX64::ElfFileParserX64(std::istream& p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX64::parseFileHeader()
{
    char l_buffer[52]; // MAGIC
    m_fileStream.read(l_buffer, 64);

    FileHeader l_fileHeader {};
    std::memcpy(&l_fileHeader.header64, l_buffer, sizeof(Elf64_Ehdr));

    return l_fileHeader;
}

std::vector<ProgramHeader> ElfFileParserX64::parseProgramHeaders(int p_programHeadersCount)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);
    auto l_programHeaderSize { sizeof(Elf64_Phdr) };

    for (auto& l_programHeader : l_programHeaders)
    {
        char* l_buffer { new char[l_programHeaderSize] };
        m_fileStream.get(l_buffer, l_programHeaderSize);
        std::memcpy(&l_programHeader.header64, l_buffer, l_programHeaderSize);
        delete[] l_buffer;
    }

   return l_programHeaders;
}

std::vector<SectionHeader> ElfFileParserX64::parseSectionHeaders(int p_sectionHeadersCount)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);
    auto l_sectionHeaderSize { sizeof(Elf64_Shdr) };

    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char* l_buffer { new char[l_sectionHeaderSize] };
        m_fileStream.get(l_buffer, l_sectionHeaderSize);
        std::memcpy(&l_sectionHeader.header64, l_buffer, l_sectionHeaderSize);
        delete[] l_buffer;
    }

   return l_sectionHeaders;
}

}


#include "ElfFileParserX32.hpp"
// #include <elf.h>
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"
#include <plog/Log.h>


namespace parser
{

ElfFileParserX32::ElfFileParserX32(std::istream* p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX32::parseFileHeader()
{
    char l_buffer[52]; // MAGIC
    m_fileStream->read(l_buffer, 52);

    FileHeader l_fileHeader {};
    std::memcpy(&l_fileHeader.header32, l_buffer, sizeof(Elf32_Ehdr));

    return l_fileHeader;
}

std::vector<ProgramHeader> ElfFileParserX32::parseProgramHeaders(int p_programHeadersCount)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);
    auto l_programHeaderSize { sizeof(Elf32_Phdr) };

    for (auto& l_programHeader : l_programHeaders)
    {
        char* l_buffer { new char[l_programHeaderSize] };
        m_fileStream->read(l_buffer, l_programHeaderSize);
        std::memcpy(&l_programHeader.header32, l_buffer, l_programHeaderSize);
        delete[] l_buffer;
    }

   return l_programHeaders;
}

std::vector<SectionHeader> ElfFileParserX32::parseSectionHeaders(int p_sectionHeadersCount)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);
    auto l_sectionHeaderSize { sizeof(Elf32_Shdr) };

    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char* l_buffer { new char[l_sectionHeaderSize] };
        m_fileStream->read(l_buffer, l_sectionHeaderSize);
        std::memcpy(&l_sectionHeader.header32, l_buffer, l_sectionHeaderSize);
        delete[] l_buffer;
    }

   return l_sectionHeaders;
}

}


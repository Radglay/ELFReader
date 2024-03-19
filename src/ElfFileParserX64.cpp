#include "ElfFileParserX64.hpp"
// #include <elf.h>
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include "FileHeaderParserX64.hpp"
#include "FileHeaderParserX32.hpp"
#include "ProgramHeaderParserX32.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"
#include "SectionHeaderParserX64.hpp"


namespace parser
{

ElfFileParserX64::ElfFileParserX64(std::istream& p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX64::parseFileHeader()
{
    auto* l_fileHeaderParser { createFileHeaderParser() };

    char* l_buffer { new char[64] }; // MAGIC
    m_fileStream.get(l_buffer, 64);
    return l_fileHeaderParser->parse(std::move(l_buffer));
}


FileHeaderParserX64* ElfFileParserX64::createFileHeaderParser()
{
    return new FileHeaderParserX64{};
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
    }

   return l_programHeaders;
}

ProgramHeaderParserX64* ElfFileParserX64::createProgramHeaderParser()
{
    return new ProgramHeaderParserX64{};
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
    }

   return l_sectionHeaders;
}

SectionHeaderParserX64* ElfFileParserX64::createSectionHeaderParser()
{
    return new SectionHeaderParserX64{};
}

}


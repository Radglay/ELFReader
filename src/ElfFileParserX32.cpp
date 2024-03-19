#include "ElfFileParserX32.hpp"
// #include <elf.h>
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include "FileHeaderParserX32.hpp"
#include "ProgramHeaderParserX32.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"
#include "SectionHeaderParserX32.hpp"


namespace parser
{

ElfFileParserX32::ElfFileParserX32(std::istream& p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX32::parseFileHeader()
{
    auto* l_fileHeaderParser { createFileHeaderParser() };

    char* l_buffer { new char[52] }; // MAGIC
    m_fileStream.get(l_buffer, 52);
    auto l_fileHeader { l_fileHeaderParser->parse(std::move(l_buffer)) };

    delete l_fileHeaderParser;
    delete[] l_buffer;

    return l_fileHeader;
}

FileHeaderParserX32* ElfFileParserX32::createFileHeaderParser()
{
    return new FileHeaderParserX32{};
}

std::vector<ProgramHeader> ElfFileParserX32::parseProgramHeaders(int p_programHeadersCount)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);
    auto l_programHeaderSize { sizeof(Elf32_Phdr) };

    for (auto& l_programHeader : l_programHeaders)
    {
        char* l_buffer { new char[l_programHeaderSize] };
        m_fileStream.get(l_buffer, l_programHeaderSize);
        std::memcpy(&l_programHeader.header32, l_buffer, l_programHeaderSize);
        delete[] l_buffer;
    }

   return l_programHeaders;
}

ProgramHeaderParserX32* ElfFileParserX32::createProgramHeaderParser()
{
    return new ProgramHeaderParserX32{};
}

std::vector<SectionHeader> ElfFileParserX32::parseSectionHeaders(int p_sectionHeadersCount)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);
    auto l_sectionHeaderSize { sizeof(Elf32_Shdr) };

    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char* l_buffer { new char[l_sectionHeaderSize] };
        m_fileStream.get(l_buffer, l_sectionHeaderSize);
        std::memcpy(&l_sectionHeader.header32, l_buffer, l_sectionHeaderSize);
        delete[] l_buffer;
    }

   return l_sectionHeaders;
}

SectionHeaderParserX32* ElfFileParserX32::createSectionHeaderParser()
{
    return new SectionHeaderParserX32{};
}

}


#include "ElfFileParserX32.hpp"
// #include <elf.h>
#include <fstream>
#include <cstring>
#include "FileHeader.hpp"
#include "FileHeaderParserX32.hpp"
#include "ProgramHeaderParserX32.hpp"
#include <elf.h>
#include <vector>


namespace parser
{

ElfFileParserX32::ElfFileParserX32(std::ifstream&& p_fileStream)
    : m_fileStream{ std::move(p_fileStream) }
{}

ElfFileParserX32::~ElfFileParserX32()
{
    m_fileStream.close();
}

FileHeader ElfFileParserX32::parseFileHeader()
{
    auto* l_fileHeaderParser { createFileHeaderParser() };

    char* l_buffer { new char[52] }; // MAGIC
    m_fileStream.get(l_buffer, 52);
    return l_fileHeaderParser->parse(std::move(l_buffer));
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
    }

   return l_programHeaders;
}

ProgramHeaderParserX32* ElfFileParserX32::createProgramHeaderParser()
{
    return new ProgramHeaderParserX32{};
}

}


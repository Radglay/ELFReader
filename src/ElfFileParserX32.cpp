#include "ElfFileParserX32.hpp"
// #include <elf.h>
#include <fstream>
#include <cstring>
#include "FileHeader.hpp"
#include "FileHeaderParserX32.hpp"


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

}


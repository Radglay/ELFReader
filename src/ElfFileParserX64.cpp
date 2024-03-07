#include "ElfFileParserX64.hpp"
// #include <elf.h>
#include <fstream>
#include <cstring>
#include "FileHeader.hpp"
#include "FileHeaderParserX64.hpp"


namespace parser
{

ElfFileParserX64::ElfFileParserX64(std::ifstream&& p_fileStream)
    : m_fileStream{ std::move(p_fileStream) }
{}

ElfFileParserX64::~ElfFileParserX64()
{
    m_fileStream.close();
}

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

}


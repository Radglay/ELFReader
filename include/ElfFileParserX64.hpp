#pragma once

#include "IElfFileParser.hpp"
#include <fstream>
#include "FileHeaderParserX64.hpp"
#include "FileHeader.hpp"


namespace parser
{

class ElfFileParserX64 : public IElfFileParser
{
public:
    ElfFileParserX64(std::ifstream&&);
    ~ElfFileParserX64();

    FileHeader parseFileHeader() override;
    FileHeaderParserX64* createFileHeaderParser() override;

private:
    std::ifstream m_fileStream;
};

}


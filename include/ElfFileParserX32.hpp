#pragma once

#include "IElfFileParser.hpp"
#include <fstream>
#include "FileHeaderParserX32.hpp"
#include "FileHeader.hpp"


namespace parser
{

class ElfFileParserX32 : public IElfFileParser
{
public:
    ElfFileParserX32(std::ifstream&&);
    ~ElfFileParserX32();

    FileHeader parseFileHeader() override;
    FileHeaderParserX32* createFileHeaderParser() override;

private:
    std::ifstream m_fileStream;
};

}


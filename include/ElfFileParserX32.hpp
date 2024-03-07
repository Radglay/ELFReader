#pragma once

#include "IElfFileParser.hpp"
#include <fstream>
#include "FileHeaderParserX32.hpp"
#include "FileHeader.hpp"
#include "ProgramHeaderParserX32.hpp"
#include "ProgramHeader.hpp"
#include <vector>


namespace parser
{

class ElfFileParserX32 : public IElfFileParser
{
public:
    ElfFileParserX32(std::ifstream&&);
    ~ElfFileParserX32();

    FileHeader parseFileHeader() override;
    FileHeaderParserX32* createFileHeaderParser() override;

    std::vector<ProgramHeader> parseProgramHeaders(int) override;
    ProgramHeaderParserX32* createProgramHeaderParser() override;

private:
    std::ifstream m_fileStream;
};

}


#pragma once

#include "IElfFileParser.hpp"
#include <fstream>
#include "FileHeaderParserX64.hpp"
#include "FileHeader.hpp"
#include "ProgramHeaderParserX64.hpp"
#include "ProgramHeader.hpp"
#include <vector>
#include "SectionHeader.hpp"
#include "SectionHeaderParserX64.hpp"


namespace parser
{

class ElfFileParserX64 : public IElfFileParser
{
public:
    ElfFileParserX64(std::ifstream&&);
    ~ElfFileParserX64();

    FileHeader parseFileHeader() override;
    FileHeaderParserX64* createFileHeaderParser() override;

    std::vector<ProgramHeader> parseProgramHeaders(int) override;
    ProgramHeaderParserX64* createProgramHeaderParser() override;

    std::vector<SectionHeader> parseSectionHeaders(int) override;
    SectionHeaderParserX64* createSectionHeaderParser() override;

private:
    std::ifstream m_fileStream;
};

}


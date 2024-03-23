#pragma once

#include "IElfFileParser.hpp"
#include <istream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include <vector>
#include "SectionHeader.hpp"


namespace parser
{

class ElfFileParserX64 : public IElfFileParser
{
public:
    ElfFileParserX64(std::istream&);
    ~ElfFileParserX64() = default;

    FileHeader parseFileHeader() override;
    std::vector<ProgramHeader> parseProgramHeaders(int) override;
    std::vector<SectionHeader> parseSectionHeaders(int) override;

private:
    std::istream& m_fileStream;
};

}


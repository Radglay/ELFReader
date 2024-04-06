#pragma once

#include "IElfFileParser.hpp"
#include <istream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include <vector>
#include "SectionHeader.hpp"


namespace parser
{

class ElfFileParserX32 : public IElfFileParser
{
public:
    ElfFileParserX32(std::istream*);
    ~ElfFileParserX32() = default;

    FileHeader parseFileHeader() override;
    std::vector<ProgramHeader> parseProgramHeaders(int, int, int) override;
    std::vector<SectionHeader> parseSectionHeaders(int, int, int) override;

private:
    std::istream* m_fileStream;
};

}


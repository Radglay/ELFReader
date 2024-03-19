#pragma once

#include "IElfFileParser.hpp"
#include <istream>
#include "FileHeaderParserX32.hpp"
#include "FileHeader.hpp"
#include "ProgramHeaderParserX32.hpp"
#include "ProgramHeader.hpp"
#include <vector>
#include "SectionHeader.hpp"
#include "SectionHeaderParserX32.hpp"


namespace parser
{

class ElfFileParserX32 : public IElfFileParser
{
public:
    ElfFileParserX32(std::istream&);
    ~ElfFileParserX32() = default;

    FileHeader parseFileHeader() override;
    FileHeaderParserX32* createFileHeaderParser() override;

    std::vector<ProgramHeader> parseProgramHeaders(int) override;
    ProgramHeaderParserX32* createProgramHeaderParser() override;

    std::vector<SectionHeader> parseSectionHeaders(int) override;
    SectionHeaderParserX32* createSectionHeaderParser() override;

private:
    std::istream& m_fileStream;
};

}


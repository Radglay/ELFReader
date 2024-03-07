#pragma once

#include <fstream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include "SectionHeader.hpp"
#include <vector>


namespace parser
{
class IFileHeaderParser;
class IProgramHeaderParser;
class ISectionHeaderParser;


class IElfFileParser
{
public:
    virtual ~IElfFileParser() = default;
    virtual FileHeader parseFileHeader() = 0;
    virtual IFileHeaderParser* createFileHeaderParser() = 0;

    virtual std::vector<ProgramHeader> parseProgramHeaders(int) = 0;
    virtual IProgramHeaderParser* createProgramHeaderParser() = 0;

    virtual std::vector<SectionHeader> parseSectionHeaders(int) = 0;
    virtual ISectionHeaderParser* createSectionHeaderParser() = 0;
};

}


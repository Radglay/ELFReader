#pragma once

#include <fstream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include "SectionHeader.hpp"
#include <vector>


namespace parser
{

class IElfFileParser
{
public:
    virtual ~IElfFileParser() = default;
    virtual FileHeader parseFileHeader() = 0;
    virtual std::vector<ProgramHeader> parseProgramHeaders(int, int) = 0;
    virtual std::vector<SectionHeader> parseSectionHeaders(int) = 0;
};

}


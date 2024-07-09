#pragma once

#include <fstream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include "SectionHeader.hpp"
#include <vector>


namespace reader
{

class IElfHeaderReader
{
public:
    virtual ~IElfHeaderReader() = default;
    virtual FileHeader readFileHeader(int) = 0;
    virtual std::vector<ProgramHeader> readProgramHeaders(int, int, int, int) = 0;
    virtual std::vector<SectionHeader> readSectionHeaders(int, int, int, int) = 0;
};

}


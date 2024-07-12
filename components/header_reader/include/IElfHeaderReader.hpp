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

    virtual FileHeader readFileHeader(int p_hostEndianness) = 0;

    virtual std::vector<ProgramHeader> readProgramHeaders(int p_programHeaderTableOffset,
                                                          int p_programHeadersCount,
                                                          int p_fileEndianness,
                                                          int p_hostEndianness) = 0;

    virtual std::vector<SectionHeader> readSectionHeaders(int p_sectionHeaderTableOffset,
                                                          int p_sectionHeadersCount,
                                                          int p_fileEndianness,
                                                          int p_hostEndianness) = 0;
};

}


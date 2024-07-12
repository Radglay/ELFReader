#pragma once

#include "IElfHeaderReader.hpp"
#include <istream>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include <vector>
#include "SectionHeader.hpp"


namespace reader
{

class ElfHeaderReaderX32 : public IElfHeaderReader
{
public:
    ElfHeaderReaderX32(std::istream*);

    FileHeader readFileHeader(int p_hostEndianness) override;

    std::vector<ProgramHeader> readProgramHeaders(int p_programHeaderTableOffset,
                                                  int p_programHeadersCount,
                                                  int p_fileEndianness,
                                                  int p_hostEndianness) override;

    std::vector<SectionHeader> readSectionHeaders(int p_sectionHeaderTableOffset,
                                                  int p_sectionHeadersCount,
                                                  int p_fileEndianness,
                                                  int p_hostEndianness) override;

private:
    std::istream* m_fileStream;
};

}


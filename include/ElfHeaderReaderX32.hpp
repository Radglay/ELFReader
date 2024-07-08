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
    ~ElfHeaderReaderX32() = default;

    FileHeader readFileHeader() override;
    std::vector<ProgramHeader> readProgramHeaders(int, int, int) override;
    std::vector<SectionHeader> readSectionHeaders(int, int, int) override;

private:
    std::istream* m_fileStream;
};

}


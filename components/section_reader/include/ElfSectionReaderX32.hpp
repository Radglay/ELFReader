#pragma once

#include <fstream>
#include <vector>
#include <elf.h>


namespace reader
{

class ElfSectionReaderX32
{
public:
    ElfSectionReaderX32(std::istream* p_fileStream);
    
    std::vector<Elf32_Sym> readSymbols(const Elf32_Shdr& p_symTabSection,
                                       int p_fileEndianness,
                                       int p_hostEndianness);

private:
    std::istream* m_fileStream;
};

}

#pragma once

#include <fstream>
#include <vector>
#include <elf.h>


namespace reader
{

class ElfSectionReaderX64
{
public:
    ElfSectionReaderX64(std::istream* p_fileStream);
    
    std::vector<Elf64_Sym> readSymbols(const Elf64_Shdr& p_symTabSection,
                                       int p_fileEndianness,
                                       int p_hostEndianness);

    std::vector<Elf64_Rela> readRelaEntries(const Elf64_Shdr& p_relaSection,
                                            int p_fileEndianness,
                                            int p_hostEndianness);

private:
    std::istream* m_fileStream;
};

}

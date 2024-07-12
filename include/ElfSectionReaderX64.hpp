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
    
    std::vector<Elf64_Sym> readSymbols(const Elf64_Shdr& p_symTabSection);

private:
    std::istream* m_fileStream;
};

}

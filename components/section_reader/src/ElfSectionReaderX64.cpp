#include "ElfSectionReaderX64.hpp"
#include <cstring>


namespace reader
{

ElfSectionReaderX64::ElfSectionReaderX64(std::istream* p_fileStream)
    : m_fileStream { p_fileStream }
{}

std::vector<Elf64_Sym> ElfSectionReaderX64::readSymbols(const Elf64_Shdr& p_symTabSection)
{
    std::vector<Elf64_Sym> l_symbols (p_symTabSection.sh_size / sizeof(Elf64_Sym));

    auto l_currentOffset { p_symTabSection.sh_offset };

    char l_buffer[sizeof(Elf64_Sym)];    
    for (auto& l_symbol : l_symbols)
    {
        m_fileStream->seekg(l_currentOffset);
        m_fileStream->read(l_buffer, sizeof(Elf64_Sym));
        std::memcpy(&l_symbol, l_buffer, sizeof(Elf64_Sym));

        l_currentOffset += sizeof(Elf64_Sym);
    }

    return l_symbols;
}

}

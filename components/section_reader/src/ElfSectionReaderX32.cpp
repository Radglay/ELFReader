#include "ElfSectionReaderX32.hpp"
#include <plog/Log.h>
#include "EndiannessUtility.hpp"
#include <cstring>



namespace reader
{

namespace
{

void convert32BitSymbolEndianness(Elf32_Sym& p_symbol)
{
    LOG_INFO << "Converting Symbol Endianness";

    p_symbol.st_name = convertEndianness(p_symbol.st_name);
    p_symbol.st_shndx = convertEndianness(p_symbol.st_shndx);
    p_symbol.st_value = convertEndianness(p_symbol.st_value);
    p_symbol.st_size = convertEndianness(p_symbol.st_size);
}

}

ElfSectionReaderX32::ElfSectionReaderX32(std::istream* p_fileStream)
    : m_fileStream { p_fileStream }
{}

std::vector<Elf32_Sym> ElfSectionReaderX32::readSymbols(const Elf32_Shdr& p_symTabSection,
                                                        int p_fileEndianness,
                                                        int p_hostEndianness)
{
    LOG_INFO << "Reading Symbol Table";
    std::vector<Elf32_Sym> l_symbols (p_symTabSection.sh_size / sizeof(Elf32_Sym));

    auto l_currentOffset { p_symTabSection.sh_offset };

    char l_buffer[sizeof(Elf32_Sym)];    
    for (auto& l_symbol : l_symbols)
    {
        m_fileStream->seekg(l_currentOffset);
        m_fileStream->read(l_buffer, sizeof(Elf32_Sym));
        std::memcpy(&l_symbol, l_buffer, sizeof(Elf32_Sym));

        if (shouldConvertEndianness(p_fileEndianness, p_hostEndianness))
        {
            convert32BitSymbolEndianness(l_symbol);
        }

        l_currentOffset += sizeof(Elf32_Sym);
    }

    return l_symbols;
}

}

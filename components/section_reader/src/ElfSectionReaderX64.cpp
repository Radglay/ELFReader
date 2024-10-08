#include "ElfSectionReaderX64.hpp"
#include <plog/Log.h>
#include "EndiannessUtility.hpp"
#include <cstring>



namespace reader
{

namespace
{

void convert64BitSymbolEndianness(Elf64_Sym& p_symbol)
{
    LOG_INFO << "Converting Symbol Endianness";

    p_symbol.st_name = convertEndianness(p_symbol.st_name);
    p_symbol.st_shndx = convertEndianness(p_symbol.st_shndx);
    p_symbol.st_value = convertEndianness(p_symbol.st_value);
    p_symbol.st_size = convertEndianness(p_symbol.st_size);
}

void convert64BitRelaEndianness(Elf64_Rela& p_rela)
{
    LOG_INFO << "Converting Rela Endianness";

    p_rela.r_offset = convertEndianness(p_rela.r_offset);
    p_rela.r_info = convertEndianness(p_rela.r_info);
    p_rela.r_addend = convertEndianness(p_rela.r_addend);
}

}

ElfSectionReaderX64::ElfSectionReaderX64(std::istream* p_fileStream)
    : m_fileStream { p_fileStream }
{}

std::vector<Elf64_Sym> ElfSectionReaderX64::readSymbols(const Elf64_Shdr& p_symTabSection,
                                                        int p_fileEndianness,
                                                        int p_hostEndianness)
{
    LOG_INFO << "Reading Symbol Table";
    std::vector<Elf64_Sym> l_symbols (p_symTabSection.sh_size / sizeof(Elf64_Sym));

    auto l_currentOffset { p_symTabSection.sh_offset };

    char l_buffer[sizeof(Elf64_Sym)];    
    for (auto& l_symbol : l_symbols)
    {
        m_fileStream->seekg(l_currentOffset);
        m_fileStream->read(l_buffer, sizeof(Elf64_Sym));
        std::memcpy(&l_symbol, l_buffer, sizeof(Elf64_Sym));

        if (shouldConvertEndianness(p_fileEndianness, p_hostEndianness))
        {
            convert64BitSymbolEndianness(l_symbol);
        }

        l_currentOffset += sizeof(Elf64_Sym);
    }

    return l_symbols;
}

std::vector<Elf64_Rela> ElfSectionReaderX64::readRelaEntries(const Elf64_Shdr& p_relaSection,
                                                             int p_fileEndianness,
                                                             int p_hostEndianness)
{
    LOG_INFO << "Reading Rela Entries";
    std::vector<Elf64_Rela> l_relas (p_relaSection.sh_size / sizeof(Elf64_Rela));
    
    auto l_currentOffset { p_relaSection.sh_offset };

    char l_buffer[sizeof(Elf64_Rela)];    
    for (auto& l_rela : l_relas)
    {
        m_fileStream->seekg(l_currentOffset);
        m_fileStream->read(l_buffer, sizeof(Elf64_Rela));
        std::memcpy(&l_rela, l_buffer, sizeof(Elf64_Rela));

        if (shouldConvertEndianness(p_fileEndianness, p_hostEndianness))
        {
            convert64BitRelaEndianness(l_rela);
        }

        l_currentOffset += sizeof(Elf64_Rela);
    }
}

}

#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include <istream>
#include <algorithm>
#include <vector>
#include <elf.h>
#include "BytesReadingUtility.hpp"
#include "EndiannessUtility.hpp"
#include <plog/Log.h>


namespace
{
    template <typename T>
    T& findSectionHeaderByType(std::vector<T>& p_sectionHeaders, int p_type)
    {
        return *std::find_if(p_sectionHeaders.begin(),
                             p_sectionHeaders.end(),
                             [](auto& l_header) { return l_header.sh_type == SHT_SYMTAB; } );
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::reset()
{
    m_elfStructureInfoBuilder.reset();
    delete m_elfObject;
    m_elfObject = new T{};
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildElfStructureInfo()
{
    m_elfStructureInfoBuilder.buildFileHeader();
    m_elfStructureInfoBuilder.buildSectionHeaders();
    m_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = m_elfStructureInfoBuilder.getResult();
    m_elfObject->elfStructureInfo = *l_result;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildSymbols()
{
    auto& l_symbolSectionHeader = findSectionHeaderByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_SYMTAB);

    std::vector<typename ElfObjectTraits::symbol_type> l_symbols (l_symbolSectionHeader.sh_size / sizeof(typename ElfObjectTraits::symbol_type));

    auto l_currentOffset { l_symbolSectionHeader.sh_offset };

    for (auto& l_symbol : l_symbols)
    {
        readBytesFromFile(l_symbol, l_currentOffset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineInfo.endianness)
            and shouldConvertEndianness(m_targetMachineInfo.endianness))
        {
            LOG_INFO << "Converting Endianness";

            convertEndianness(l_symbol.st_name);
            convertEndianness(l_symbol.st_value);
            convertEndianness(l_symbol.st_size);
            convertEndianness(l_symbol.st_shndx);
        }

        l_currentOffset += sizeof(typename ElfObjectTraits::symbol_type);
    }

    m_elfObject->symbols = l_symbols;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
T* ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::getResult()
{
    auto l_result = new T{ *m_elfObject };
    reset();
    return l_result;
}


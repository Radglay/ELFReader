#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include <istream>
#include <algorithm>
#include <vector>
#include <elf.h>
#include "BytesReadingUtility.hpp"
#include "EndiannessUtility.hpp"
#include <plog/Log.h>
#include <iterator>


namespace
{
    template <typename T>
    T& findSectionHeaderByType(std::vector<T>& p_sectionHeaders, int p_type)
    {
        return *std::find_if(p_sectionHeaders.begin(),
                             p_sectionHeaders.end(),
                             [p_type](auto& l_header) { return l_header.sh_type == p_type; } );
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
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildSymbolHeaders()
{
    auto& l_symbolSectionHeader = findSectionHeaderByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_SYMTAB);

    std::vector<typename ElfObjectTraits::symbol_header_type> l_symbolHeaders (l_symbolSectionHeader.sh_size / sizeof(typename ElfObjectTraits::symbol_header_type));

    auto l_currentOffset { l_symbolSectionHeader.sh_offset };

    for (auto& l_symbol : l_symbolHeaders)
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

        l_currentOffset += sizeof(typename ElfObjectTraits::symbol_header_type);
    }

    m_elfObject->symbolHeaders = l_symbolHeaders;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildNoteHeaders()
{
    std::vector<typename ElfStructureInfoTraits::section_header_type> l_noteSectionHeaders;
    std::copy_if(m_elfObject->elfStructureInfo.sectionHeaders.begin(),
                 m_elfObject->elfStructureInfo.sectionHeaders.end(),
                 std::back_inserter(l_noteSectionHeaders),
                 [](const auto& p_sectionHeader)
                 {
                    return p_sectionHeader.sh_type == SHT_NOTE;
                 });

    std::vector<typename ElfObjectTraits::note_header_type> l_noteHeaders;
    for (auto& l_noteSectionHeader : l_noteSectionHeaders)
    {
        typename ElfObjectTraits::note_header_type l_noteHeader {};
        auto l_currentOffset  { l_noteSectionHeader.sh_offset };
        readBytesFromFile(l_noteHeader, l_currentOffset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineInfo.endianness)
            and shouldConvertEndianness(m_targetMachineInfo.endianness))
        {
            convertEndianness(l_noteHeader.n_namesz);
            convertEndianness(l_noteHeader.n_descsz);
            convertEndianness(l_noteHeader.n_type);
        }

        l_noteHeaders.push_back(l_noteHeader);
    }

    m_elfObject->noteHeaders = l_noteHeaders;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationHeaders()
{

}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationWithAddendHeaders()
{
    auto& l_relaSectionHeader = findSectionHeaderByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_RELA);

    std::vector<typename ElfObjectTraits::rel_with_addend_header_type> l_relaHeaders (
        l_relaSectionHeader.sh_size / sizeof(typename ElfObjectTraits::rel_with_addend_header_type));

    auto l_currentOffset { l_relaSectionHeader.sh_offset };

    for (auto& l_relaHeader : l_relaHeaders)
    {
        readBytesFromFile(l_relaHeader, l_currentOffset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineInfo.endianness)
            and shouldConvertEndianness(m_targetMachineInfo.endianness))
        {
            LOG_INFO << "Converting RelaHeader Endianness";

            convertEndianness(l_relaHeader.r_offset);
            convertEndianness(l_relaHeader.r_info);
            convertEndianness(l_relaHeader.r_addend);
        }

        l_currentOffset += sizeof(typename ElfObjectTraits::rel_with_addend_header_type);
    }

    m_elfObject->relocationWithAddendHeaders = l_relaHeaders;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
T* ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::getResult()
{
    auto l_result = new T{ *m_elfObject };
    reset();
    return l_result;
}


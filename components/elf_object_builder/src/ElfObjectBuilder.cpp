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
#include <utility>
#include <memory>
#include "SymbolTableSection.hpp"
#include "RelocationSection.hpp"
#include "RelocationWithAddendSection.hpp"
#include "NoteSection.hpp"
#include "StringTableSection.hpp"
#include "ProgbitsSection.hpp"
#include "NobitsSection.hpp"


namespace
{
    template <typename T>
    std::shared_ptr<T>& findSectionHeaderByType(std::vector<std::shared_ptr<T>>& p_sectionHeaders, int p_type)
    {
        return *std::find_if(p_sectionHeaders.begin(),
                             p_sectionHeaders.end(),
                             [p_type](auto& l_header) { return l_header->sh_type == p_type; } );
    }

    template <typename T>
    std::vector<std::pair<int, std::shared_ptr<T>>> findSectionHeadersWithIndicesByType(std::vector<std::shared_ptr<T>>& p_sectionHeaders, int p_type)
    {
        std::vector<std::pair<int, std::shared_ptr<T>>> l_result;

        auto l_size { p_sectionHeaders.size() };
        for (int i = 0; i < l_size; ++i)
        {
            if (p_sectionHeaders[i]->sh_type == p_type)
                l_result.emplace_back(i, p_sectionHeaders[i]);
        }

        return l_result;
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
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildSymbolSections()
{
    auto& l_symbolSectionHeader = findSectionHeaderByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_SYMTAB);

    std::vector<typename ElfObjectTraits::symbol_header_type> l_symbolHeaders (l_symbolSectionHeader->sh_size / sizeof(typename ElfObjectTraits::symbol_header_type));

    auto l_currentOffset { l_symbolSectionHeader->sh_offset };

    for (auto& l_symbol : l_symbolHeaders)
    {
        readBytesFromFile(l_symbol, l_currentOffset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineInfo.endianness)
            and shouldConvertEndianness(m_targetMachineInfo.endianness))
        {
            LOG_INFO << "Converting Section Header Endianness";

            convertEndianness(l_symbol.st_name);
            convertEndianness(l_symbol.st_value);
            convertEndianness(l_symbol.st_size);
            convertEndianness(l_symbol.st_shndx);
        }

        l_currentOffset += sizeof(typename ElfObjectTraits::symbol_header_type);
    }

    m_elfObject->sections.emplace_back(
        std::make_shared<SymbolTableSection<typename ElfStructureInfoTraits::section_header_type,
                                            typename ElfObjectTraits::symbol_header_type>>(l_symbolSectionHeader, l_symbolHeaders));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildNoteSections()
{
    std::vector<std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>> l_noteSectionHeaders;
    std::copy_if(m_elfObject->elfStructureInfo.sectionHeaders.begin(),
                 m_elfObject->elfStructureInfo.sectionHeaders.end(),
                 std::back_inserter(l_noteSectionHeaders),
                 [](auto& p_sectionHeader)
                 {
                    return p_sectionHeader->sh_type == SHT_NOTE;
                 });

    for (auto& l_noteSectionHeader : l_noteSectionHeaders)
    {
        typename ElfObjectTraits::note_header_type l_noteHeader {};
        auto l_currentOffset  { l_noteSectionHeader->sh_offset };
        readBytesFromFile(l_noteHeader, l_currentOffset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineInfo.endianness)
            and shouldConvertEndianness(m_targetMachineInfo.endianness))
        {
            convertEndianness(l_noteHeader.n_namesz);
            convertEndianness(l_noteHeader.n_descsz);
            convertEndianness(l_noteHeader.n_type);
        }

        l_currentOffset += sizeof(typename ElfObjectTraits::note_header_type);
        std::string l_namespace;
        readNullTerminatedStringFromFile(l_namespace, l_currentOffset, m_fileStream);
        l_currentOffset += l_noteHeader.n_namesz;

        std::vector<unsigned char> l_descriptionBytes;
        readBytesFromFileToVector(l_descriptionBytes,
                                  l_currentOffset,
                                  l_noteHeader.n_descsz,
                                  m_fileStream);

        m_elfObject->sections.emplace_back(
            std::make_shared<NoteSection<typename ElfStructureInfoTraits::section_header_type,
                                         typename ElfObjectTraits::note_header_type>>(l_noteSectionHeader,
                                                                                      l_noteHeader,
                                                                                      l_namespace,
                                                                                      l_descriptionBytes));
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationSections()
{
    auto l_relSectionHeadersWithIndices { findSectionHeadersWithIndicesByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_REL) };

    for (auto& l_relSectionHeaderWithIndex : l_relSectionHeadersWithIndices)
    { 
        auto& l_relSectionHeader { l_relSectionHeaderWithIndex.second };
        auto l_sectionHeaderIndex { l_relSectionHeaderWithIndex.first };

        std::vector<typename ElfObjectTraits::rel_header_type> l_relHeaders (
            l_relSectionHeader->sh_size / sizeof(typename ElfObjectTraits::rel_header_type));

        auto l_currentOffset { l_relSectionHeader->sh_offset };

        for (auto& l_relHeader : l_relHeaders)
        {
            readBytesFromFile(l_relHeader, l_currentOffset, m_fileStream);

            if (isEndiannessCorrect(m_targetMachineInfo.endianness)
                and shouldConvertEndianness(m_targetMachineInfo.endianness))
            {
                LOG_INFO << "Converting Relocation Section Header Endiannes";

                convertEndianness(l_relHeader.r_offset);
                convertEndianness(l_relHeader.r_info);
            }

            l_currentOffset += sizeof(typename ElfObjectTraits::rel_header_type);
        }


        m_elfObject->sections.emplace_back(
            std::make_shared<RelocationSection<typename ElfStructureInfoTraits::section_header_type,
                                               typename ElfObjectTraits::rel_header_type>>(l_relSectionHeader, l_relHeaders));
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationWithAddendSections()
{
    auto l_relaSectionHeadersWithIndices { findSectionHeadersWithIndicesByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_RELA) };

    for (auto& l_relaSectionHeaderWithIndex : l_relaSectionHeadersWithIndices)
    { 
        auto& l_relaSectionHeader { l_relaSectionHeaderWithIndex.second };
        auto l_sectionHeaderIndex { l_relaSectionHeaderWithIndex.first };

        std::vector<typename ElfObjectTraits::rel_with_addend_header_type> l_relaHeaders (
            l_relaSectionHeader->sh_size / sizeof(typename ElfObjectTraits::rel_with_addend_header_type));

        auto l_currentOffset { l_relaSectionHeader->sh_offset };

        for (auto& l_relaHeader : l_relaHeaders)
        {
            readBytesFromFile(l_relaHeader, l_currentOffset, m_fileStream);

            if (isEndiannessCorrect(m_targetMachineInfo.endianness)
                and shouldConvertEndianness(m_targetMachineInfo.endianness))
            {
                LOG_INFO << "Converting Relocation With Addend Section Header Endiannes";

                convertEndianness(l_relaHeader.r_offset);
                convertEndianness(l_relaHeader.r_info);
                convertEndianness(l_relaHeader.r_addend);
            }

            l_currentOffset += sizeof(typename ElfObjectTraits::rel_with_addend_header_type);
        }

        m_elfObject->sections.emplace_back(
            std::make_shared<RelocationWithAddendSection<typename ElfStructureInfoTraits::section_header_type,
                                                         typename ElfObjectTraits::rel_with_addend_header_type>>(l_relaSectionHeader, l_relaHeaders));
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildStringTableSections()
{
    auto l_stringTableSectionHeadersWithIndices { findSectionHeadersWithIndicesByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_STRTAB) };

    for (auto& l_stringTabSectionHeaderWithIndex : l_stringTableSectionHeadersWithIndices)
    {
        std::map<int, std::string> l_stringTable;

        auto& l_strTabSectionHeader { l_stringTabSectionHeaderWithIndex.second };
        auto l_sectionHeaderIndex { l_stringTabSectionHeaderWithIndex.first };

        auto l_currentOffset { l_strTabSectionHeader->sh_offset };
        auto l_size { l_strTabSectionHeader->sh_size };
        auto l_endOffset { l_currentOffset + l_size };
        auto l_relativeOffset { 0x0 };

        while (l_currentOffset < l_endOffset)
        {
            std::string l_name;
            readNullTerminatedStringFromFile(l_name, l_currentOffset, m_fileStream);
            auto l_readBytes { m_fileStream->gcount()};
            l_stringTable[l_relativeOffset] = l_name;
    
            l_relativeOffset += l_readBytes;
            l_currentOffset += l_readBytes;
        }

        m_elfObject->sections.emplace_back(
            std::make_shared<StringTableSection<typename ElfStructureInfoTraits::section_header_type>>(l_strTabSectionHeader, l_stringTable));        
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildProgbitsSections()
{
    auto l_progbitsSectionHeadersWithIndices { findSectionHeadersWithIndicesByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_PROGBITS) };

    for (auto& l_sectionHeaderWithIndex : l_progbitsSectionHeadersWithIndices)
    {
        auto& l_progbitSectionHeader { l_sectionHeaderWithIndex.second };
        auto l_sectionHeaderIndex { l_sectionHeaderWithIndex.first };

        std::vector<unsigned char> l_bytes;
        auto l_offset { l_progbitSectionHeader->sh_offset};
        auto l_size { l_progbitSectionHeader->sh_size };

        l_bytes.reserve(l_size);    
        readBytesFromFileToVector(l_bytes, l_offset, l_size, m_fileStream);

        m_elfObject->sections.emplace_back(
            std::make_shared<ProgbitsSection<typename ElfStructureInfoTraits::section_header_type>>(l_progbitSectionHeader, l_bytes)); 
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildNobitsSections()
{
    auto l_nobitsSectionHeadersWithIndices { findSectionHeadersWithIndicesByType(m_elfObject->elfStructureInfo.sectionHeaders, SHT_NOBITS) };

    for (auto& l_sectionHeaderWithIndex : l_nobitsSectionHeadersWithIndices)
    {
        auto& l_nobitSectionHeader { l_sectionHeaderWithIndex.second };
        auto l_sectionHeaderIndex { l_sectionHeaderWithIndex.first };

        m_elfObject->sections.emplace_back(
            std::make_shared<NobitsSection<typename ElfStructureInfoTraits::section_header_type>>(l_nobitSectionHeader)); 
    }  
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
T* ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::getResult()
{
    auto l_result = new T{ *m_elfObject };
    reset();
    return l_result;
}


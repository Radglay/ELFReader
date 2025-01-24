#include "ElfSectionBuilder.hpp"
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



template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::reset()
{
    delete m_elfObject;
    m_elfObject = new T{};
}


template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildSymbolSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    std::vector<typename ElfObjectTraits::symbol_header_type> l_symbolHeaders (p_sectionHeader->sh_size / sizeof(typename ElfObjectTraits::symbol_header_type));

    auto l_currentOffset { p_sectionHeader->sh_offset };

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
                                            typename ElfObjectTraits::symbol_header_type>>(p_sectionHeader, l_symbolHeaders));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildNoteSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    typename ElfObjectTraits::note_header_type l_noteHeader {};
    auto l_currentOffset  { p_sectionHeader->sh_offset };
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

    std::vector<unsigned char> l_descriptorBytes;
    readBytesFromFileToVector(l_descriptorBytes,
                                l_currentOffset,
                                l_noteHeader.n_descsz,
                                m_fileStream);

    m_elfObject->sections.emplace_back(
        std::make_shared<NoteSection<typename ElfStructureInfoTraits::section_header_type,
                                     typename ElfObjectTraits::note_header_type>>(p_sectionHeader,
                                                                                  l_noteHeader,
                                                                                  l_namespace,
                                                                                  l_descriptorBytes));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    std::vector<typename ElfObjectTraits::rel_header_type> l_relHeaders (
        p_sectionHeader->sh_size / sizeof(typename ElfObjectTraits::rel_header_type));

    auto l_currentOffset { p_sectionHeader->sh_offset };

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
                                            typename ElfObjectTraits::rel_header_type>>(p_sectionHeader, l_relHeaders));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildRelocationWithAddendSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    std::vector<typename ElfObjectTraits::rel_with_addend_header_type> l_relaHeaders (
        p_sectionHeader->sh_size / sizeof(typename ElfObjectTraits::rel_with_addend_header_type));

    auto l_currentOffset { p_sectionHeader->sh_offset };

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
                                                        typename ElfObjectTraits::rel_with_addend_header_type>>(p_sectionHeader, l_relaHeaders));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildStringTableSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    std::map<int, std::string> l_stringTable;

    auto l_currentOffset { p_sectionHeader->sh_offset };
    auto l_size { p_sectionHeader->sh_size };
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
        std::make_shared<StringTableSection<typename ElfStructureInfoTraits::section_header_type>>(p_sectionHeader, l_stringTable));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildProgbitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    std::vector<unsigned char> l_bytes;
    auto l_offset { p_sectionHeader->sh_offset};
    auto l_size { p_sectionHeader->sh_size };

    l_bytes.reserve(l_size);
    readBytesFromFileToVector(l_bytes, l_offset, l_size, m_fileStream);

    m_elfObject->sections.emplace_back(
        std::make_shared<ProgbitsSection<typename ElfStructureInfoTraits::section_header_type>>(p_sectionHeader, l_bytes));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildNobitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type> p_sectionHeader)
{
    m_elfObject->sections.emplace_back(
        std::make_shared<NobitsSection<typename ElfStructureInfoTraits::section_header_type>>(p_sectionHeader));
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
T* ElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::getResult()
{
    auto l_result = new T{ *m_elfObject };
    reset();
    return l_result;
}


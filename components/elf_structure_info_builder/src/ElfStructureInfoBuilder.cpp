#include "ElfStructureInfoBuilder.hpp"
#include <plog/Log.h>
#include "BytesReadingUtility.hpp"
#include "EndiannessUtility.hpp"


template <typename T, typename Traits>
void ElfStructureInfoBuilder<T, Traits>::reset()
{
    delete m_elfStructureInfo;
    m_elfStructureInfo = new T();
}

template <typename T, typename Traits>
void ElfStructureInfoBuilder<T, Traits>::buildFileHeader()
{
    LOG_INFO << "Building X32 File Header";

    readBytesFromFile(m_elfStructureInfo->fileHeader, 0, m_fileStream);

    if (isEndiannessCorrect(m_targetMachineEndianness)
        and shouldConvertEndianness(m_targetMachineEndianness))
    {
        LOG_INFO << "Converting Endianness";

        convertEndianness(m_elfStructureInfo->fileHeader.e_type);
        convertEndianness(m_elfStructureInfo->fileHeader.e_machine);
        convertEndianness(m_elfStructureInfo->fileHeader.e_version);
        convertEndianness(m_elfStructureInfo->fileHeader.e_entry);
        convertEndianness(m_elfStructureInfo->fileHeader.e_phoff);
        convertEndianness(m_elfStructureInfo->fileHeader.e_shoff);
        convertEndianness(m_elfStructureInfo->fileHeader.e_flags);
        convertEndianness(m_elfStructureInfo->fileHeader.e_ehsize);
        convertEndianness(m_elfStructureInfo->fileHeader.e_phentsize);
        convertEndianness(m_elfStructureInfo->fileHeader.e_phnum);
        convertEndianness(m_elfStructureInfo->fileHeader.e_shentsize);
        convertEndianness(m_elfStructureInfo->fileHeader.e_shnum);
        convertEndianness(m_elfStructureInfo->fileHeader.e_shstrndx);
    }
}

template <typename T, typename Traits>
void ElfStructureInfoBuilder<T, Traits>::buildSectionHeaders()
{
    LOG_INFO << "Building X32 Section Headers";
    std::vector<typename Traits::section_header_type> l_sectionHeaders(m_elfStructureInfo->fileHeader.e_shnum);

    int l_offset = m_elfStructureInfo->fileHeader.e_shoff;
    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        readBytesFromFile(l_sectionHeader, l_offset, m_fileStream);

        if (isEndiannessCorrect(m_targetMachineEndianness)
            and shouldConvertEndianness(m_targetMachineEndianness))
        {
            LOG_INFO << "Converting Endianness";

            convertEndianness(l_sectionHeader.sh_name);
            convertEndianness(l_sectionHeader.sh_type);
            convertEndianness(l_sectionHeader.sh_flags);
            convertEndianness(l_sectionHeader.sh_addr);
            convertEndianness(l_sectionHeader.sh_offset);
            convertEndianness(l_sectionHeader.sh_size);
            convertEndianness(l_sectionHeader.sh_link);
            convertEndianness(l_sectionHeader.sh_info);
            convertEndianness(l_sectionHeader.sh_addralign);
            convertEndianness(l_sectionHeader.sh_entsize);
        }

        l_offset += sizeof(typename Traits::section_header_type);
    }

    m_elfStructureInfo->sectionHeaders = l_sectionHeaders;
}

template <typename T, typename Traits>
void ElfStructureInfoBuilder<T, Traits>::buildProgramHeaders()
{
    LOG_INFO << "Building X32 Program Headers";

    std::vector<typename Traits::program_header_type> l_programHeaders(m_elfStructureInfo->fileHeader.e_phnum);

    int l_offset = m_elfStructureInfo->fileHeader.e_phoff;
    for (auto& l_programHeader : l_programHeaders)
    {
        readBytesFromFile(l_programHeader, l_offset, m_fileStream);
        if (isEndiannessCorrect(m_targetMachineEndianness)
            and shouldConvertEndianness(m_targetMachineEndianness))
        {
            LOG_INFO << "Converting Endianness";

            convertEndianness(l_programHeader.p_type);
            convertEndianness(l_programHeader.p_offset);
            convertEndianness(l_programHeader.p_vaddr);
            convertEndianness(l_programHeader.p_paddr);
            convertEndianness(l_programHeader.p_filesz);
            convertEndianness(l_programHeader.p_memsz);
            convertEndianness(l_programHeader.p_flags);
            convertEndianness(l_programHeader.p_align);
        }

        l_offset += sizeof(typename Traits::program_header_type);
    }

    m_elfStructureInfo->programHeaders = l_programHeaders;
}

template <typename T, typename Traits>
T* ElfStructureInfoBuilder<T, Traits>::getResult()
{
    auto l_result = new T(*m_elfStructureInfo);
    reset();
    return l_result;
}

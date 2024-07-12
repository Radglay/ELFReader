#include "ElfHeaderReaderX32.hpp"
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"
#include "EndiannessUtility.hpp"

#include <endian.h>
#include <inttypes.h>

#include <algorithm>
#include <iterator>

#include <plog/Log.h>


namespace reader
{

namespace
{

void convert32BitFileHeaderToCorrectEndianness(FileHeader& p_fileHeader)
{
    LOG_INFO << "Converting File Header Endianness";

    p_fileHeader.header32.e_type = convertEndianness(p_fileHeader.header32.e_type);
    p_fileHeader.header32.e_machine = convertEndianness(p_fileHeader.header32.e_machine);
    p_fileHeader.header32.e_version = convertEndianness(p_fileHeader.header32.e_version);
    p_fileHeader.header32.e_entry = convertEndianness(p_fileHeader.header32.e_entry);
    p_fileHeader.header32.e_phoff = convertEndianness(p_fileHeader.header32.e_phoff);
    p_fileHeader.header32.e_shoff = convertEndianness(p_fileHeader.header32.e_shoff);
    p_fileHeader.header32.e_flags = convertEndianness(p_fileHeader.header32.e_flags);
    p_fileHeader.header32.e_ehsize = convertEndianness(p_fileHeader.header32.e_ehsize);
    p_fileHeader.header32.e_phentsize = convertEndianness(p_fileHeader.header32.e_phentsize);
    p_fileHeader.header32.e_phnum = convertEndianness(p_fileHeader.header32.e_phnum);
    p_fileHeader.header32.e_shentsize = convertEndianness(p_fileHeader.header32.e_shentsize);
    p_fileHeader.header32.e_shnum = convertEndianness(p_fileHeader.header32.e_shnum);
    p_fileHeader.header32.e_shstrndx = convertEndianness(p_fileHeader.header32.e_shstrndx);
}

void convert32BitProgramHeaderToCorrectEndianness(ProgramHeader& p_programHeader)
{
    LOG_INFO << "Converting Program Header Endianness";

    p_programHeader.header32.p_type = convertEndianness(p_programHeader.header32.p_type);
    p_programHeader.header32.p_offset = convertEndianness(p_programHeader.header32.p_offset);
    p_programHeader.header32.p_vaddr = convertEndianness(p_programHeader.header32.p_vaddr);
    p_programHeader.header32.p_paddr = convertEndianness(p_programHeader.header32.p_paddr);
    p_programHeader.header32.p_filesz = convertEndianness(p_programHeader.header32.p_filesz);
    p_programHeader.header32.p_memsz = convertEndianness(p_programHeader.header32.p_memsz);
    p_programHeader.header32.p_flags = convertEndianness(p_programHeader.header32.p_flags); 
    p_programHeader.header32.p_align = convertEndianness(p_programHeader.header32.p_align); 
}

void convert32BitSectionHeaderToCorrectEndianness(SectionHeader& p_sectionHeader)
{
    LOG_INFO << "Converting Section Header Endianness";

    p_sectionHeader.header32.sh_type = convertEndianness(p_sectionHeader.header32.sh_type);
    p_sectionHeader.header32.sh_flags = convertEndianness(p_sectionHeader.header32.sh_flags);
    p_sectionHeader.header32.sh_addr = convertEndianness(p_sectionHeader.header32.sh_addr);
    p_sectionHeader.header32.sh_name = convertEndianness(p_sectionHeader.header32.sh_name);
    p_sectionHeader.header32.sh_offset = convertEndianness(p_sectionHeader.header32.sh_offset);
    p_sectionHeader.header32.sh_size = convertEndianness(p_sectionHeader.header32.sh_size);
    p_sectionHeader.header32.sh_link = convertEndianness(p_sectionHeader.header32.sh_link);
    p_sectionHeader.header32.sh_info = convertEndianness(p_sectionHeader.header32.sh_info);
    p_sectionHeader.header32.sh_addralign = convertEndianness(p_sectionHeader.header32.sh_addralign);
    p_sectionHeader.header32.sh_entsize = convertEndianness(p_sectionHeader.header32.sh_entsize);
}

}


ElfHeaderReaderX32::ElfHeaderReaderX32(std::istream* p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfHeaderReaderX32::readFileHeader(int p_hostEndianness)
{
    LOG_INFO << "Reading File Header";

    unsigned char l_buffer[sizeof(Elf32_Ehdr)];
    m_fileStream->read(reinterpret_cast<char*>(l_buffer), sizeof(Elf32_Ehdr));

    FileHeader l_fileHeader;
    std::memcpy(&l_fileHeader.header32, l_buffer, sizeof(Elf32_Ehdr));
    l_fileHeader.discriminator = FileHeaderDiscriminator::SYSTEM_VERSION_32_BIT;

    int l_fileEndianness { static_cast<int>(l_fileHeader.header32.e_ident[EI_DATA]) };

    if (shouldConvertEndianness(l_fileEndianness, p_hostEndianness))
    {
        convert32BitFileHeaderToCorrectEndianness(l_fileHeader);
    }

    return l_fileHeader;
}

std::vector<ProgramHeader> ElfHeaderReaderX32::readProgramHeaders(int p_programHeaderTableOffset,
                                                                  int p_programHeadersCount,
                                                                  int p_fileEndianness,
                                                                  int p_hostEndianness)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);

    m_fileStream->seekg(p_programHeaderTableOffset);

    LOG_INFO << "Reading Program Headers";
    for (auto& l_programHeader : l_programHeaders)
    {
        char l_buffer[sizeof(Elf32_Phdr)] {};
        m_fileStream->read(l_buffer, sizeof(Elf32_Phdr));
        std::memcpy(&l_programHeader.header32, l_buffer, sizeof(Elf32_Phdr));

        if (shouldConvertEndianness(p_fileEndianness, p_hostEndianness))
        {
            convert32BitProgramHeaderToCorrectEndianness(l_programHeader);
        }
        l_programHeader.discriminator = ProgramHeaderDiscriminator::SYSTEM_VERSION_32_BIT;
    }

   return l_programHeaders;
}

std::vector<SectionHeader> ElfHeaderReaderX32::readSectionHeaders(int p_sectionHeaderTableOffset,
                                                                  int p_sectionHeadersCount,
                                                                  int p_fileEndianness,
                                                                  int p_hostEndianness)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);

    m_fileStream->seekg(p_sectionHeaderTableOffset);

    LOG_INFO << "Reading Section Headers";
    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char l_buffer[sizeof(Elf32_Shdr)] {};
        m_fileStream->read(l_buffer, sizeof(Elf32_Shdr));
        std::memcpy(&l_sectionHeader.header32, l_buffer, sizeof(Elf32_Shdr));

        if (shouldConvertEndianness(p_fileEndianness, p_hostEndianness))
        {
            convert32BitSectionHeaderToCorrectEndianness(l_sectionHeader);
        }
        l_sectionHeader.discriminator = SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT;
    }

   return l_sectionHeaders;
}

}

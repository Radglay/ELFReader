#include "ElfFileParserX32.hpp"
#include <istream>
#include <cstring>
#include "FileHeader.hpp"
#include <elf.h>
#include <vector>
#include "SectionHeader.hpp"

#include <arpa/inet.h>
#include <endian.h>
#include <inttypes.h>

#include <algorithm>
#include <iterator>

#include "WrongTargetEndianessException.hpp"
#include <plog/Log.h>


namespace parser
{

namespace
{

template <typename T>
T convertToUnsignedLittleEndian(T p_toConvert);

template <>
uint16_t convertToUnsignedLittleEndian(uint16_t p_toConvert)
{
    return ntohs(p_toConvert);
}

uint32_t convertToUnsignedLittleEndian(uint32_t p_toConvert)
{
    return ntohl(p_toConvert);
}


template <typename T>
T convertToUnsignedBigEndian(T p_toConvert);

template <>
uint16_t convertToUnsignedBigEndian(uint16_t p_toConvert)
{
    return htons(p_toConvert);
}

template <>
uint32_t convertToUnsignedBigEndian(uint32_t p_toConvert)
{
    return htonl(p_toConvert);
}

}


void fill32BitFileHeaderWithCorrectEndianess(FileHeader& p_fileHeader, int p_hostEndianess, int p_targetEndianess)
{
    LOG_INFO << "Filling 32-bit file header";

    if ( p_targetEndianess == ELFDATA2LSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Little Endian to Host Big Endian";
            p_fileHeader.header32.e_type = convertToUnsignedBigEndian(p_fileHeader.header32.e_type);
            p_fileHeader.header32.e_machine = convertToUnsignedBigEndian(p_fileHeader.header32.e_machine);
            p_fileHeader.header32.e_version = convertToUnsignedBigEndian(p_fileHeader.header32.e_version);
            p_fileHeader.header32.e_entry = convertToUnsignedBigEndian(p_fileHeader.header32.e_entry);
            p_fileHeader.header32.e_phoff = convertToUnsignedBigEndian(p_fileHeader.header32.e_phoff);
            p_fileHeader.header32.e_shoff = convertToUnsignedBigEndian(p_fileHeader.header32.e_shoff);
            p_fileHeader.header32.e_flags = convertToUnsignedBigEndian(p_fileHeader.header32.e_flags);
            p_fileHeader.header32.e_ehsize = convertToUnsignedBigEndian(p_fileHeader.header32.e_ehsize);
            p_fileHeader.header32.e_phentsize = convertToUnsignedBigEndian(p_fileHeader.header32.e_phentsize);
            p_fileHeader.header32.e_phnum = convertToUnsignedBigEndian(p_fileHeader.header32.e_phnum);
            p_fileHeader.header32.e_shentsize = convertToUnsignedBigEndian(p_fileHeader.header32.e_shentsize);
            p_fileHeader.header32.e_shnum = convertToUnsignedBigEndian(p_fileHeader.header32.e_shnum);
            p_fileHeader.header32.e_shstrndx = convertToUnsignedBigEndian(p_fileHeader.header32.e_shstrndx);
        }
    }
    else if( p_targetEndianess == ELFDATA2MSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Big Endian to Host Little Endian";
            p_fileHeader.header32.e_type = convertToUnsignedLittleEndian(p_fileHeader.header32.e_type);
            p_fileHeader.header32.e_machine = convertToUnsignedLittleEndian(p_fileHeader.header32.e_machine);
            p_fileHeader.header32.e_version = convertToUnsignedLittleEndian(p_fileHeader.header32.e_version);
            p_fileHeader.header32.e_entry = convertToUnsignedLittleEndian(p_fileHeader.header32.e_entry);
            p_fileHeader.header32.e_phoff = convertToUnsignedLittleEndian(p_fileHeader.header32.e_phoff);
            p_fileHeader.header32.e_shoff = convertToUnsignedLittleEndian(p_fileHeader.header32.e_shoff);
            p_fileHeader.header32.e_flags = convertToUnsignedLittleEndian(p_fileHeader.header32.e_flags);
            p_fileHeader.header32.e_ehsize = convertToUnsignedLittleEndian(p_fileHeader.header32.e_ehsize);
            p_fileHeader.header32.e_phentsize = convertToUnsignedLittleEndian(p_fileHeader.header32.e_phentsize);
            p_fileHeader.header32.e_phnum = convertToUnsignedLittleEndian(p_fileHeader.header32.e_phnum);
            p_fileHeader.header32.e_shentsize = convertToUnsignedLittleEndian(p_fileHeader.header32.e_shentsize);
            p_fileHeader.header32.e_shnum = convertToUnsignedLittleEndian(p_fileHeader.header32.e_shnum);
            p_fileHeader.header32.e_shstrndx = convertToUnsignedLittleEndian(p_fileHeader.header32.e_shstrndx);
        }
    }
    else
    {
        LOG_ERROR << "Exception thrown: WrongTargetEndianessException\n";
        throw WrongTargetEndianessException();
    }
}

void fill32BitProgramHeaderWithCorrectEndianess(ProgramHeader& p_programHeader, int p_hostEndianess, int p_targetEndianess)
{
    LOG_INFO << "Filling 32-bit program header";

    if ( p_targetEndianess == ELFDATA2LSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Little Endian to Host Big Endian";
            p_programHeader.header32.p_type = convertToUnsignedBigEndian(p_programHeader.header32.p_type);
            p_programHeader.header32.p_offset = convertToUnsignedBigEndian(p_programHeader.header32.p_offset);
            p_programHeader.header32.p_vaddr = convertToUnsignedBigEndian(p_programHeader.header32.p_vaddr);
            p_programHeader.header32.p_paddr = convertToUnsignedBigEndian(p_programHeader.header32.p_paddr);
            p_programHeader.header32.p_filesz = convertToUnsignedBigEndian(p_programHeader.header32.p_filesz);
            p_programHeader.header32.p_memsz = convertToUnsignedBigEndian(p_programHeader.header32.p_memsz);
            p_programHeader.header32.p_flags = convertToUnsignedBigEndian(p_programHeader.header32.p_flags); 
            p_programHeader.header32.p_align = convertToUnsignedBigEndian(p_programHeader.header32.p_align); 
        }
    }
    else if( p_targetEndianess == ELFDATA2MSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Big Endian to Host Little Endian";
            p_programHeader.header32.p_type = convertToUnsignedLittleEndian(p_programHeader.header32.p_type);
            p_programHeader.header32.p_offset = convertToUnsignedLittleEndian(p_programHeader.header32.p_offset);
            p_programHeader.header32.p_vaddr = convertToUnsignedLittleEndian(p_programHeader.header32.p_vaddr);
            p_programHeader.header32.p_paddr = convertToUnsignedLittleEndian(p_programHeader.header32.p_paddr);
            p_programHeader.header32.p_filesz = convertToUnsignedLittleEndian(p_programHeader.header32.p_filesz);
            p_programHeader.header32.p_memsz = convertToUnsignedLittleEndian(p_programHeader.header32.p_memsz);
            p_programHeader.header32.p_flags = convertToUnsignedLittleEndian(p_programHeader.header32.p_flags); 
            p_programHeader.header32.p_align = convertToUnsignedLittleEndian(p_programHeader.header32.p_align); 
        }
    }
    else
    {
        LOG_ERROR << "Exception thrown: WrongTargetEndianessException\n";
        throw WrongTargetEndianessException();
    }
}

void fill32BitSectionHeadersWithCorrectEndianess(SectionHeader& p_sectionHeader, int p_hostEndianess, int p_targetEndianess)
{
    LOG_INFO << "Filling 32-bit section header";

    if ( p_targetEndianess == ELFDATA2LSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Little Endian to Host Big Endian";

            p_sectionHeader.header32.sh_type = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_type);
            p_sectionHeader.header32.sh_flags = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_flags);
            p_sectionHeader.header32.sh_addr = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_addr);
            p_sectionHeader.header32.sh_name = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_name);
            p_sectionHeader.header32.sh_offset = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_offset);
            p_sectionHeader.header32.sh_size = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_size);
            p_sectionHeader.header32.sh_link = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_link);
            p_sectionHeader.header32.sh_info = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_info);
            p_sectionHeader.header32.sh_addralign = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_addralign);
            p_sectionHeader.header32.sh_entsize = convertToUnsignedBigEndian(p_sectionHeader.header32.sh_entsize);
        }
    }
    else if( p_targetEndianess == ELFDATA2MSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Big Endian to Host Little Endian";

            p_sectionHeader.header32.sh_type = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_type);
            p_sectionHeader.header32.sh_flags = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_flags);
            p_sectionHeader.header32.sh_addr = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_addr);
            p_sectionHeader.header32.sh_name = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_name);
            p_sectionHeader.header32.sh_offset = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_offset);
            p_sectionHeader.header32.sh_size = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_size);
            p_sectionHeader.header32.sh_link = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_link);
            p_sectionHeader.header32.sh_info = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_info);
            p_sectionHeader.header32.sh_addralign = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_addralign);
            p_sectionHeader.header32.sh_entsize = convertToUnsignedLittleEndian(p_sectionHeader.header32.sh_entsize);
        }
    }
    else
    {
        LOG_ERROR << "Exception thrown: WrongTargetEndianessException\n";
        throw WrongTargetEndianessException();
    }
}

ElfFileParserX32::ElfFileParserX32(std::istream* p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX32::parseFileHeader()
{
    unsigned char l_buffer[sizeof(Elf32_Ehdr)];
    m_fileStream->read(reinterpret_cast<char*>(l_buffer), sizeof(Elf32_Ehdr));

    FileHeader l_fileHeader;
    std::memcpy(&l_fileHeader.header32, l_buffer, sizeof(Elf32_Ehdr));
    l_fileHeader.discriminator = FileHeaderDiscriminator::SYSTEM_VERSION_32_BIT;

    int l_hostEndianess { ELFDATA2LSB };
    if ( BYTE_ORDER == BIG_ENDIAN )
    {
        l_hostEndianess = ELFDATA2MSB;
    }

    int l_targetEndianess = static_cast<int>(l_fileHeader.header32.e_ident[EI_DATA]);

    fill32BitFileHeaderWithCorrectEndianess(l_fileHeader, l_hostEndianess, l_targetEndianess);

    return l_fileHeader;
}

std::vector<ProgramHeader> ElfFileParserX32::parseProgramHeaders(int p_programHeaderTableOffset,
                                                                 int p_programHeadersCount,
                                                                 int p_targetEndianess)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);

    int l_hostEndianess { ELFDATA2LSB };
    if ( BYTE_ORDER == BIG_ENDIAN )
    {
        l_hostEndianess = ELFDATA2MSB;
    }

    m_fileStream->seekg(p_programHeaderTableOffset);
    for (auto& l_programHeader : l_programHeaders)
    {
        char l_buffer[sizeof(Elf32_Phdr)] {};
        m_fileStream->read(l_buffer, sizeof(Elf32_Phdr));
        std::memcpy(&l_programHeader.header32, l_buffer, sizeof(Elf32_Phdr));
        fill32BitProgramHeaderWithCorrectEndianess(l_programHeader, l_hostEndianess, p_targetEndianess);
        l_programHeader.discriminator = ProgramHeaderDiscriminator::SYSTEM_VERSION_32_BIT;
    }

   return l_programHeaders;
}

std::vector<SectionHeader> ElfFileParserX32::parseSectionHeaders(int p_sectionHeaderTableOffset,
                                                                 int p_sectionHeadersCount,
                                                                 int p_targetEndianess)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);

    int l_hostEndianess { ELFDATA2LSB };
    if ( BYTE_ORDER == BIG_ENDIAN )
    {
        l_hostEndianess = ELFDATA2MSB;
    }

    m_fileStream->seekg(p_sectionHeaderTableOffset);
    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char l_buffer[sizeof(Elf32_Shdr)] {};
        m_fileStream->read(l_buffer, sizeof(Elf32_Shdr));
        std::memcpy(&l_sectionHeader.header32, l_buffer, sizeof(Elf32_Shdr));
        fill32BitSectionHeadersWithCorrectEndianess(l_sectionHeader, l_hostEndianess, p_targetEndianess);
        l_sectionHeader.discriminator = SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT;
    }

   return l_sectionHeaders;
}

}


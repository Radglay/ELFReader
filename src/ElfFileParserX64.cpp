#include "ElfFileParserX64.hpp"
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

uint64_t convertToUnsignedLittleEndian(uint64_t p_toConvert)
{
    uint32_t l_lowerPart { static_cast<uint32_t>(p_toConvert) }; 
    
    p_toConvert >>= 32;
    uint32_t l_higherPart { static_cast<uint32_t>(p_toConvert) }; 

    uint64_t l_result { ntohl(static_cast<uint64_t>(l_lowerPart)) };
    l_result <<= 32;
    l_result += ntohl(static_cast<uint64_t>(l_higherPart));

    return l_result;
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

uint64_t convertToUnsignedBigEndian(uint64_t p_toConvert)
{
    uint32_t l_lowerPart { static_cast<uint32_t>(p_toConvert) }; 
    
    p_toConvert >>= 32;
    uint32_t l_higherPart { static_cast<uint32_t>(p_toConvert) }; 

    uint64_t l_result { htonl(static_cast<uint64_t>(l_lowerPart)) };
    l_result <<= 32;
    l_result += htonl(static_cast<uint64_t>(l_higherPart));

    return l_result;
}

}


void fill64BitFileHeaderWithCorrectEndianess(FileHeader& p_fileHeader, int p_hostEndianess, int p_targetEndianess)
{
    LOG_INFO << "Filling 64-bit file header";

    if ( p_targetEndianess == ELFDATA2LSB )
    {
        if ( p_hostEndianess == p_targetEndianess )
        {
            LOG_INFO << "No endianess formatting";
        }
        else
        {
            LOG_INFO << "Converting Target Little Endian to Host Big Endian";
            p_fileHeader.header64.e_type = convertToUnsignedBigEndian(p_fileHeader.header64.e_type);
            p_fileHeader.header64.e_machine = convertToUnsignedBigEndian(p_fileHeader.header64.e_machine);
            p_fileHeader.header64.e_version = convertToUnsignedBigEndian(p_fileHeader.header64.e_version);
            p_fileHeader.header64.e_entry = convertToUnsignedBigEndian(p_fileHeader.header64.e_entry);
            p_fileHeader.header64.e_phoff = convertToUnsignedBigEndian(p_fileHeader.header64.e_phoff);
            p_fileHeader.header64.e_shoff = convertToUnsignedBigEndian(p_fileHeader.header64.e_shoff);
            p_fileHeader.header64.e_flags = convertToUnsignedBigEndian(p_fileHeader.header64.e_flags);
            p_fileHeader.header64.e_ehsize = convertToUnsignedBigEndian(p_fileHeader.header64.e_ehsize);
            p_fileHeader.header64.e_phentsize = convertToUnsignedBigEndian(p_fileHeader.header64.e_phentsize);
            p_fileHeader.header64.e_phnum = convertToUnsignedBigEndian(p_fileHeader.header64.e_phnum);
            p_fileHeader.header64.e_shentsize = convertToUnsignedBigEndian(p_fileHeader.header64.e_shentsize);
            p_fileHeader.header64.e_shnum = convertToUnsignedBigEndian(p_fileHeader.header64.e_shnum);
            p_fileHeader.header64.e_shstrndx = convertToUnsignedBigEndian(p_fileHeader.header64.e_shstrndx);
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
            p_fileHeader.header64.e_type = convertToUnsignedLittleEndian(p_fileHeader.header64.e_type);
            p_fileHeader.header64.e_machine = convertToUnsignedLittleEndian(p_fileHeader.header64.e_machine);
            p_fileHeader.header64.e_version = convertToUnsignedLittleEndian(p_fileHeader.header64.e_version);
            p_fileHeader.header64.e_entry = convertToUnsignedLittleEndian(p_fileHeader.header64.e_entry);
            p_fileHeader.header64.e_phoff = convertToUnsignedLittleEndian(p_fileHeader.header64.e_phoff);
            p_fileHeader.header64.e_shoff = convertToUnsignedLittleEndian(p_fileHeader.header64.e_shoff);
            p_fileHeader.header64.e_flags = convertToUnsignedLittleEndian(p_fileHeader.header64.e_flags);
            p_fileHeader.header64.e_ehsize = convertToUnsignedLittleEndian(p_fileHeader.header64.e_ehsize);
            p_fileHeader.header64.e_phentsize = convertToUnsignedLittleEndian(p_fileHeader.header64.e_phentsize);
            p_fileHeader.header64.e_phnum = convertToUnsignedLittleEndian(p_fileHeader.header64.e_phnum);
            p_fileHeader.header64.e_shentsize = convertToUnsignedLittleEndian(p_fileHeader.header64.e_shentsize);
            p_fileHeader.header64.e_shnum = convertToUnsignedLittleEndian(p_fileHeader.header64.e_shnum);
            p_fileHeader.header64.e_shstrndx = convertToUnsignedLittleEndian(p_fileHeader.header64.e_shstrndx);
        }
    }
    else
    {
        LOG_ERROR << "Exception thrown: WrongTargetEndianessException\n";
        throw WrongTargetEndianessException();
    }

}


ElfFileParserX64::ElfFileParserX64(std::istream* p_fileStream)
    : m_fileStream{ p_fileStream }
{}

FileHeader ElfFileParserX64::parseFileHeader()
{
    unsigned char l_buffer[sizeof(Elf64_Ehdr)];
    m_fileStream->read(reinterpret_cast<char*>(l_buffer), sizeof(Elf64_Ehdr));

    FileHeader l_fileHeader;
    std::memcpy(&l_fileHeader.header64, l_buffer, sizeof(Elf64_Ehdr));

    l_fileHeader.discriminator = FileHeaderDiscriminator::SYSTEM_VERSION_64_BIT;

    int l_hostEndianess { ELFDATA2LSB };
    if ( BYTE_ORDER == BIG_ENDIAN )
    {
        l_hostEndianess = ELFDATA2MSB;
    }

    int l_targetEndianess = static_cast<int>(l_fileHeader.header64.e_ident[EI_DATA]);

    fill64BitFileHeaderWithCorrectEndianess(l_fileHeader, l_hostEndianess, l_targetEndianess);

    return l_fileHeader;
}

std::vector<ProgramHeader> ElfFileParserX64::parseProgramHeaders(int p_programHeadersCount)
{
    std::vector<ProgramHeader> l_programHeaders(p_programHeadersCount);
    auto l_programHeaderSize { sizeof(Elf64_Phdr) };

    for (auto& l_programHeader : l_programHeaders)
    {
        char* l_buffer { new char[l_programHeaderSize] };
        m_fileStream->read(l_buffer, l_programHeaderSize);
        std::memcpy(&l_programHeader.header64, l_buffer, l_programHeaderSize);
        delete[] l_buffer;
    }

   return l_programHeaders;
}

std::vector<SectionHeader> ElfFileParserX64::parseSectionHeaders(int p_sectionHeadersCount)
{
    std::vector<SectionHeader> l_sectionHeaders(p_sectionHeadersCount);
    auto l_sectionHeaderSize { sizeof(Elf64_Shdr) };

    for (auto& l_sectionHeader : l_sectionHeaders)
    {
        char* l_buffer { new char[l_sectionHeaderSize] };
        m_fileStream->read(l_buffer, l_sectionHeaderSize);
        std::memcpy(&l_sectionHeader.header64, l_buffer, l_sectionHeaderSize);
        delete[] l_buffer;
    }

   return l_sectionHeaders;
}

}


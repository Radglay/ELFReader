#include <fstream>
#include <elf.h>
#include <vector>
#include "ElfHeaderReaderCreator.hpp"
#include "IElfHeaderReader.hpp"
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include "SectionHeader.hpp"
#include <plog/Log.h>
#include "LoggerSettingUtility.hpp"


int main(int argc, char** argv)
{
    initLogger();

    if (argc != 2) {
        LOG_ERROR << "Wrong number of arguments";
        return 1;
    }

    LOG_INFO << "Opening the file " << argv[1];
    std::ifstream* l_fileStream { new std::ifstream(argv[1], std::ios::binary) };
    auto* l_elfHeaderReader { reader::ElfHeaderReaderCreator::createElfHeaderReader(l_fileStream) };

    int l_hostEndianness { ELFDATA2LSB };
    if ( BYTE_ORDER == BIG_ENDIAN )
    {
        l_hostEndianness = ELFDATA2MSB;
    }
    LOG_INFO << "Checking host endianness: " << l_hostEndianness;

    auto l_fileHeader { l_elfHeaderReader->readFileHeader(l_hostEndianness) };

    auto l_phnum { 0 };
    auto l_shnum { 0 };
    auto l_fileEndianness { 1 };
    auto l_phoff { 0 };
    auto l_shoff { 0 };

    LOG_INFO << "Checking file system version: " << static_cast<int>(l_fileHeader.discriminator);
    if (l_fileHeader.discriminator == FileHeaderDiscriminator::SYSTEM_VERSION_32_BIT)
    {
        l_phnum = l_fileHeader.header32.e_phnum; // can be moved to ElfHeaderReader...?
        l_shnum = l_fileHeader.header32.e_shnum; // can be moved to ElfHeaderReader...?
        l_fileEndianness = l_fileHeader.header32.e_ident[EI_DATA];
        l_phoff = l_fileHeader.header32.e_phoff;
        l_shoff = l_fileHeader.header32.e_shoff;
    }
    else if (l_fileHeader.discriminator == FileHeaderDiscriminator::SYSTEM_VERSION_64_BIT)
    {
        l_phnum = l_fileHeader.header64.e_phnum; // can be moved to ElfHeaderReader...?
        l_shnum = l_fileHeader.header64.e_shnum; // can be moved to ElfHeaderReader...?
        l_fileEndianness = l_fileHeader.header64.e_ident[EI_DATA];
        l_phoff = l_fileHeader.header64.e_phoff;
        l_shoff = l_fileHeader.header64.e_shoff;
    }

    std::vector<ProgramHeader> l_programHeaders { l_elfHeaderReader->readProgramHeaders(
        l_phoff, l_phnum, l_fileEndianness, l_hostEndianness) };
    
    std::vector<SectionHeader> l_sectionHeaders { l_elfHeaderReader->readSectionHeaders(
        l_shoff, l_shnum, l_fileEndianness, l_hostEndianness) };

    LOG_INFO << "Closing program";

    delete l_elfHeaderReader;
    delete l_fileStream;

    return 0;
}
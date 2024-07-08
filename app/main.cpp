#include <iostream>
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
    if (argc != 2) {
        std::cerr << "You provided wrong number of arguments...\n";
        return 1;
    }
    
    initLogger();

    LOG_INFO << "Opening the file " << argv[1];
    std::ifstream* l_fileStream { new std::ifstream(argv[1], std::ios::binary) };
    auto* l_elfFileParser { reader::ElfHeaderReaderCreator::createElfHeaderReader(l_fileStream) };

    auto l_fileHeader { l_elfFileParser->readFileHeader() };

    auto l_phnum { 0 };
    auto l_shnum { 0 };
    auto l_targetEndianess { 1 };
    auto l_phoff { 0 };
    auto l_shoff { 0 };

    if (l_fileHeader.discriminator == FileHeaderDiscriminator::SYSTEM_VERSION_32_BIT)
    {
        l_phnum = l_fileHeader.header32.e_phnum; // can be moved to ElfHeaderReader...?
        l_shnum = l_fileHeader.header32.e_shnum; // can be moved to ElfHeaderReader...?
        l_targetEndianess = l_fileHeader.header32.e_ident[EI_DATA];
        l_phoff = l_fileHeader.header32.e_phoff;
        l_shoff = l_fileHeader.header32.e_shoff;
    }
    else if (l_fileHeader.discriminator == FileHeaderDiscriminator::SYSTEM_VERSION_64_BIT)
    {
        l_phnum = l_fileHeader.header64.e_phnum; // can be moved to ElfHeaderReader...?
        l_shnum = l_fileHeader.header64.e_shnum; // can be moved to ElfHeaderReader...?
        l_targetEndianess = l_fileHeader.header64.e_ident[EI_DATA];
        l_phoff = l_fileHeader.header64.e_phoff;
        l_shoff = l_fileHeader.header64.e_shoff;
    }

    LOG_INFO << "Start parsing program headers";
    std::vector<ProgramHeader> l_programHeaders { l_elfFileParser->readProgramHeaders(
        l_phoff, l_phnum, l_targetEndianess) };
    
    LOG_INFO << "Start parsing section headers";
    std::vector<SectionHeader> l_sectionHeaders { l_elfFileParser->readSectionHeaders(
        l_shoff, l_shnum, l_targetEndianess) };

    LOG_INFO << "Closing program";

    delete l_elfFileParser;
    delete l_fileStream;

    return 0;
}
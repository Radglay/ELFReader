#include <iostream>
#include <fstream>
#include <elf.h>
#include "ElfFileParserCreator.hpp"
#include "IElfFileParser.hpp"
#include <vector>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"
#include "SectionHeader.hpp"


int main(int argc, char** argv) //ENDIANES... 
{
    if (argc != 2) {
        std::cerr << "You provided wrong number of arguments...\n";
        return 1;
    }

    std::ifstream l_fileStream { argv[1] };
    auto* l_elfFileParser { parser::ElfFileParserCreator::createElfFileParser(l_fileStream) };

    auto l_fileHeader { l_elfFileParser->parseFileHeader() };
    auto l_phnum { l_fileHeader.header32.e_phnum }; // can be moved to ElfFileParser...?
    std::vector l_programHeaders { l_elfFileParser->parseProgramHeaders(l_phnum) };

    auto l_shnum { l_fileHeader.header32.e_shnum }; // can be moved to ElfFileParser...?
    std::vector l_sectionHeaders { l_elfFileParser->parseSectionHeaders(l_shnum) };

    delete l_elfFileParser;

    return 0;
}
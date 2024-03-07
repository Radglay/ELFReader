#include <iostream>
#include <elf.h>
#include "ElfFileParserCreator.hpp"
#include "IElfFileParser.hpp"
#include <vector>
#include "FileHeader.hpp"
#include "ProgramHeader.hpp"


int main(int argc, char** argv) //ENDIANES... 
{
    if (argc != 2) {
        std::cerr << "You provided wrong number of arguments...\n";
        return 1;
    }

    parser::IElfFileParser* l_elfFileParser { parser::ElfFileParserCreator::createElfFileParser(argv[1]) };
    auto l_fileHeader { l_elfFileParser->parseFileHeader() };
    std::cout << static_cast<int>(l_fileHeader.header32.e_ident[4]) << '\n';
    auto l_phnum { l_fileHeader.header32.e_phnum }; // can be moved to ElfFileParser...?
    std::vector l_programHeaders { l_elfFileParser->parseProgramHeaders(l_phnum) };

    for (auto& l_programHeader : l_programHeaders)
    {
        std::cout << std::hex << l_programHeader.header32.p_type << '\n';
    }

    
    return 0;
}
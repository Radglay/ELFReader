#include <iostream>
#include <elf.h>
#include "ElfFileParserCreator.hpp"
#include "IElfFileParser.hpp"


int main(int argc, char** argv) //ENDIANES... 
{
    if (argc != 2) {
        std::cerr << "You provided wrong number of arguments...\n";
        return 1;
    }

    parser::IElfFileParser* l_elfFileParser { parser::ElfFileParserCreator::createElfFileParser(argv[1]) };
    auto l_fhdr { l_elfFileParser->parseFileHeader() };
    std::cout << static_cast<int>(l_fhdr.header64.e_ident[4]) << '\n';

    return 0;
}
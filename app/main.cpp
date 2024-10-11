#include <fstream>
#include <plog/Log.h>
#include "LoggerSettingUtility.hpp"
#include <iostream>
#include <elf.h>
#include "IElfObjectBuilder.hpp"
#include "ElfObjectBuilderCreator.hpp"


int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Wrong number of arguments\n";
        return 1;
    }

    initLogger();

    std::ifstream* l_fileStream { new std::ifstream(argv[1], std::ios::binary) };
    auto l_elfObjectBuilder = createElfObjectBuilder(l_fileStream);

    l_elfObjectBuilder->buildElfStructureInfo();

    return 0;
}
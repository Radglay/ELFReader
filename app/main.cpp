#include <fstream>
#include <plog/Log.h>
#include "LoggerSettingUtility.hpp"
#include <iostream>
#include <elf.h>
#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "BytesReadingUtility.hpp"



int main(int argc, char** argv)
{
    if (argc != 2) {
        std::cerr << "Wrong number of arguments\n";
        return 1;
    }

    initLogger();

    std::ifstream* l_fileStream { new std::ifstream(argv[1], std::ios::binary) };

    TargetMachineInfo l_targetMachineInfo;
    readBytesFromFile(l_targetMachineInfo, 0, l_fileStream);


    if (l_targetMachineInfo.bitVersion == ELFCLASS32)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX32> l_elfStructureInfoBuilder(l_fileStream, l_targetMachineInfo.endianness);
        auto l_elfObjectBuilder = ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32>(l_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
        l_elfObjectBuilder.buildElfStructureInfo();
    }
    else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder(l_fileStream, l_targetMachineInfo.endianness);
        auto l_elfObjectBuilder = ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64>(l_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
        l_elfObjectBuilder.buildElfStructureInfo();
        l_elfObjectBuilder.buildSymbolSections();
        l_elfObjectBuilder.buildNoteSections();
        l_elfObjectBuilder.buildRelocationSections();
        l_elfObjectBuilder.buildRelocationWithAddendSections();
        l_elfObjectBuilder.buildStringTableSections();
        l_elfObjectBuilder.buildProgbitsSections();
        l_elfObjectBuilder.buildNobitsSections();

        auto l_result { l_elfObjectBuilder.getResult() };
        auto l_sections { l_result->sections };
    }


    return 0;
}

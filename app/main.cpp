#include "mainwindow.h"
#include <QApplication>


int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();

    // if (argc != 2) {
    //     std::cerr << "Wrong number of arguments\n";
    //     return 1;
    // }

    // initLogger();

    // std::ifstream* l_fileStream { new std::ifstream(argv[1], std::ios::binary) };

    // TargetMachineInfo l_targetMachineInfo;
    // readBytesFromFile(l_targetMachineInfo, 0, l_fileStream);


    // if (l_targetMachineInfo.bitVersion == ELFCLASS32)
    // {
    //     ElfStructureInfoBuilder<ElfStructureInfoX32> l_elfStructureInfoBuilder(l_fileStream, l_targetMachineInfo.endianness);
    //     auto l_elfObjectBuilder = ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32>(l_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
    //     l_elfObjectBuilder.buildElfStructureInfo();
    // }
    // else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
    // {
    //     ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder(l_fileStream, l_targetMachineInfo.endianness);
    //     auto l_elfObjectBuilder = ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64>(l_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
    //     l_elfObjectBuilder.buildElfStructureInfo();
    //     l_elfObjectBuilder.buildSymbolSections();
    //     l_elfObjectBuilder.buildNoteSections();
    //     l_elfObjectBuilder.buildRelocationSections();
    //     l_elfObjectBuilder.buildRelocationWithAddendSections();
    //     l_elfObjectBuilder.buildStringTableSections();
    //     l_elfObjectBuilder.buildProgbitsSections();
    //     l_elfObjectBuilder.buildNobitsSections();

    //     auto l_result { l_elfObjectBuilder.getResult() };
    //     auto l_sections { l_result->sections };
    // }


    // return 0;
}

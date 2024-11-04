#include "ElfObjectBuilderCreator.hpp"
#include "ElfObjectBuilderX64.hpp"
#include "ElfObjectBuilderX32.hpp"
#include <istream>
#include <memory>
#include "TargetMachineInfo.hpp"
#include <elf.h>
#include "BytesReadingUtility.hpp"
#include "ElfStructureInfoBuilderX32.hpp"
#include "ElfStructureInfoBuilderX64.hpp"


std::unique_ptr<IElfObjectBuilder> createElfObjectBuilder(std::istream* p_fileStream)
{
    TargetMachineInfo l_targetMachineInfo;

    readBytesFromFile(l_targetMachineInfo, 0, p_fileStream);

    if (l_targetMachineInfo.bitVersion == ELFCLASS32)
    {
        ElfStructureInfoBuilderX32 l_elfStructureInfoBuilder(p_fileStream, l_targetMachineInfo.endianness);
        return std::make_unique<ElfObjectBuilderX32>(p_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
    }
    else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
    {
        ElfStructureInfoBuilderX64 l_elfStructureInfoBuilder(p_fileStream, l_targetMachineInfo.endianness);
        return std::make_unique<ElfObjectBuilderX64>(p_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo);
    }
        

    return nullptr;
}

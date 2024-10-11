#include "ElfObjectBuilderCreator.hpp"
#include "ElfObjectBuilderX64.hpp"
#include "ElfObjectBuilderX32.hpp"
#include <istream>
#include <memory>
#include "TargetMachineInfo.hpp"
#include <elf.h>
#include "BytesReadingUtility.hpp"


std::unique_ptr<IElfObjectBuilder> createElfObjectBuilder(std::istream* p_fileStream)
{
    TargetMachineInfo l_targetMachineInfo;

    readBytesFromFile(l_targetMachineInfo, 0, p_fileStream);

    if (l_targetMachineInfo.bitVersion == ELFCLASS32)
        return std::make_unique<ElfObjectBuilderX32>(p_fileStream, l_targetMachineInfo);
    else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
        return std::make_unique<ElfObjectBuilderX64>(p_fileStream, l_targetMachineInfo);

    return nullptr;
}

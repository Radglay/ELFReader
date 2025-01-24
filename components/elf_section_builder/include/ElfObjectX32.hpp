#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX32.hpp"
#include <vector>
#include <map>
#include <elf.h>
#include "IElfSection.hpp"
#include <memory>


struct ElfObjectX32
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX32 elfStructureInfo;
    std::vector<std::shared_ptr<IElfSection<Elf32_Shdr>>> sections;
};

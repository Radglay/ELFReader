#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX32.hpp"
#include <vector>
#include <elf.h>


struct ElfObjectX32
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX32 elfStructureInfo;
    std::vector<Elf32_Sym> symbolHeaders;
    std::vector<Elf32_Nhdr> noteHeaders;
};

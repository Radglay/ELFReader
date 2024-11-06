#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX32.hpp"
#include <vector>


struct ElfObjectX32
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX32 elfStructureInfo;
    std::vector<Elf32_Sym> symbols;
};

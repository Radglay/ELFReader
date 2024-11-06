#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX64.hpp"
#include <vector>


struct ElfObjectX64
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX64 elfStructureInfo;
    std::vector<Elf64_Sym> symbols;
};

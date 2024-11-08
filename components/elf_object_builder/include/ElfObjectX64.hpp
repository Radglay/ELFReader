#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX64.hpp"
#include <vector>
#include <elf.h>


struct ElfObjectX64
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX64 elfStructureInfo;
    std::vector<Elf64_Sym> symbolHeaders;
    std::vector<Elf64_Nhdr> noteHeaders;
    std::vector<Elf64_Rel> relocationHeaders;
    std::vector<Elf64_Rela> relocationWithAddendHeaders;
};

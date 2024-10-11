#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX64.hpp"


struct ElfObjectX64
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX64 elfStructureInfo;
};

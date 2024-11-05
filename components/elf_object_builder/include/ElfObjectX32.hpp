#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX32.hpp"


struct ElfObjectX32
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX32 elfStructureInfo;
};

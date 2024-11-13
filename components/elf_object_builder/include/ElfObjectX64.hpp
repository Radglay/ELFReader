#pragma once

#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX64.hpp"
#include <vector>
#include <map>
#include <elf.h>
#include "IElfSection.hpp"
#include <memory>


struct ElfObjectX64
{
    TargetMachineInfo targetMachineInfo;
    ElfStructureInfoX64 elfStructureInfo;
    std::vector<std::shared_ptr<IElfSection<Elf64_Shdr>>> sections;
};

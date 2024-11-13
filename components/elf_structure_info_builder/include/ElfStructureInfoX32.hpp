#pragma once

#include <elf.h>
#include <vector>
#include <memory>


struct ElfStructureInfoX32
{
    Elf32_Ehdr fileHeader;
    std::vector<std::shared_ptr<Elf32_Shdr>> sectionHeaders;
    std::vector<Elf32_Phdr> programHeaders;
};

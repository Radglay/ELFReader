#pragma once

#include <elf.h>
#include <vector>


struct ElfStructureInfoX32
{
    Elf32_Ehdr fileHeader;
    std::vector<Elf32_Shdr> sectionHeaders;
    std::vector<Elf32_Phdr> programHeaders;
};

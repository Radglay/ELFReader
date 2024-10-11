#pragma once

#include <elf.h>
#include <vector>


struct ElfStructureInfoX64
{
    Elf64_Ehdr fileHeader;
    std::vector<Elf64_Shdr> sectionHeaders;
    std::vector<Elf64_Phdr> programHeaders;
};

#pragma once

#include <elf.h>
#include <vector>
#include <memory>


struct ElfStructureInfoX64
{
    Elf64_Ehdr fileHeader;
    std::vector<std::shared_ptr<Elf64_Shdr>> sectionHeaders;
    std::vector<Elf64_Phdr> programHeaders;
};

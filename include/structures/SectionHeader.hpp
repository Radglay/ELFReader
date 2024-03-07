#pragma once

#include <elf.h>

union SectionHeader
{
    Elf32_Shdr header32;
    Elf64_Shdr header64;
};

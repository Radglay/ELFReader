#pragma once

#include <elf.h>

union ProgramHeader
{
    Elf32_Phdr header32;
    Elf64_Phdr header64;
};

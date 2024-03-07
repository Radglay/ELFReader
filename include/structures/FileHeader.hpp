#pragma once

#include <elf.h>

union FileHeader
{
    Elf32_Ehdr header32;
    Elf64_Ehdr header64;
};

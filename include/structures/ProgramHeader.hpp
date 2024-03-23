#pragma once

#include <elf.h>


enum class ProgramHeaderDiscriminator
{
    SYSTEM_VERSION_32_BIT,
    SYSTEM_VERSION_64_BIT
};

struct ProgramHeader
{
    ProgramHeaderDiscriminator discriminator;
    union
    {
        Elf32_Phdr header32;
        Elf64_Phdr header64;
    };
};
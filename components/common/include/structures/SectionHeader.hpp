#pragma once

#include <elf.h>


enum class SectionHeaderDiscriminator
{
    SYSTEM_VERSION_32_BIT,
    SYSTEM_VERSION_64_BIT
};

struct SectionHeader
{
    SectionHeaderDiscriminator discriminator;
    union
    {
        Elf32_Shdr header32;
        Elf64_Shdr header64;
    };
};
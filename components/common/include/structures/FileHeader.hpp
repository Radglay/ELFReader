#pragma once

#include <elf.h>


enum class FileHeaderDiscriminator
{
    SYSTEM_VERSION_32_BIT,
    SYSTEM_VERSION_64_BIT
};

struct FileHeader
{
    FileHeaderDiscriminator discriminator;
    union
    {
        Elf32_Ehdr header32;
        Elf64_Ehdr header64;
    };
};

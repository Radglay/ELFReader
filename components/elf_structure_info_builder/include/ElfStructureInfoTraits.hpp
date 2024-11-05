#pragma once

#include <elf.h>
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"


template <typename T>
class elf_structure_info_traits;


template <>
class elf_structure_info_traits<ElfStructureInfoX32>
{
public:
    using file_header_type = Elf32_Ehdr;
    using section_header_type = Elf32_Shdr;
    using program_header_type = Elf32_Phdr;
};

template <>
class elf_structure_info_traits<ElfStructureInfoX64>
{
public:
    using file_header_type = Elf64_Ehdr;
    using section_header_type = Elf64_Shdr;
    using program_header_type = Elf64_Phdr;
};


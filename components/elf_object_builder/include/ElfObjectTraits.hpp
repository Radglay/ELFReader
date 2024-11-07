#pragma once

#include <elf.h>
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"


template <typename T>
class elf_object_traits;


template <>
class elf_object_traits<ElfObjectX32>
{
public:
    using symbol_header_type = Elf32_Sym;
    using note_header_type = Elf32_Nhdr;
};

template <>
class elf_object_traits<ElfObjectX64>
{
public:
    using symbol_header_type = Elf64_Sym;
    using note_header_type = Elf64_Nhdr;
};


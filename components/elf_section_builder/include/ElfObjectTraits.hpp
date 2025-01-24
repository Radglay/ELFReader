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
    using rel_header_type = Elf32_Rel;
    using rel_with_addend_header_type = Elf32_Rela;
};

template <>
class elf_object_traits<ElfObjectX64>
{
public:
    using symbol_header_type = Elf64_Sym;
    using note_header_type = Elf64_Nhdr;
    using rel_header_type = Elf64_Rel;
    using rel_with_addend_header_type = Elf64_Rela;
};


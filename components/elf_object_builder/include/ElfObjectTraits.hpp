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
    using symbol_type = Elf32_Sym;
};

template <>
class elf_object_traits<ElfObjectX64>
{
public:
    using symbol_type = Elf64_Sym;
};


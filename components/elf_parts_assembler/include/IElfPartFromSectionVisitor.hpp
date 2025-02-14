#pragma once

#include <elf.h>
#include <string>


template <typename T, typename U>
class INoteSection;

template <typename T>
class NobitsSection;

template <typename T>
class ProgbitsSection;

template <typename T, typename U>
class RelocationSection;

template <typename T, typename U>
class RelocationWithAddendSection;

template <typename T>
class StringTableSection;

template <typename T, typename U>
class SymbolTableSection;

template <typename T>
class NullSection;


class ElfPart;


class IElfPartFromSectionVisitor
{
public:
    virtual ElfPart assembleElfPartFromSection(INoteSection<Elf32_Shdr, Elf32_Nhdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(INoteSection<Elf64_Shdr, Elf64_Nhdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf64_Rel>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(RelocationWithAddendSection<Elf64_Shdr, Elf64_Rela>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(NullSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(NullSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ~IElfPartFromSectionVisitor() = default;
};

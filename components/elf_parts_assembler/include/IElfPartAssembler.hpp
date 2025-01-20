#pragma once

#include <elf.h>
#include <string>
#include <vector>
#include <memory>


template <typename T, typename U>
class NoteSection;

template <typename T>
class NobitsSection;

template <typename T>
class ProgbitsSection;

template <typename T, typename U>
class RelocationSection;

template <typename T>
class StringTableSection;

template <typename T, typename U>
class SymbolTableSection;


class ElfPart;


class IElfPartAssembler
{
public:
    virtual ElfPart assembleElfPartFromFileHeader(Elf32_Ehdr&) = 0;
    virtual ElfPart assembleElfPartFromFileHeader(Elf64_Ehdr&) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf32_Shdr>>&, int) = 0;
    virtual std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf64_Shdr>>&, int) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf32_Phdr>&, int) = 0;
    virtual std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf64_Phdr>&, int) = 0;

    virtual ElfPart assembleElfPartFromSection(NoteSection<Elf32_Shdr, Elf32_Nhdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(NoteSection<Elf64_Shdr, Elf64_Nhdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf32_Rel>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&) = 0;

    virtual ElfPart assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&) = 0;
    virtual ElfPart assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&) = 0;

    virtual ~IElfPartAssembler() = default;
};

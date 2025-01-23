#pragma once

#include <elf.h>
#include <vector>
#include <memory>
#include "IElfSection.hpp"


template <typename T>
class StringTableSection;

class ElfPart;

class IElfPartFromSectionVisitor;


class IElfPartAssembler
{
public:
    virtual ElfPart assembleElfPartFromFileHeader(Elf32_Ehdr&) = 0;
    virtual ElfPart assembleElfPartFromFileHeader(Elf64_Ehdr&) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf32_Shdr>>&, int) = 0;
    virtual std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf64_Shdr>>&, int) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf32_Phdr>&, int) = 0;
    virtual std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf64_Phdr>&, int) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromSections(const std::vector<std::shared_ptr<IElfSection<Elf32_Shdr>>>&,
                                                              StringTableSection<Elf32_Shdr>*,
                                                              IElfPartFromSectionVisitor&) = 0;

    virtual std::vector<ElfPart> assembleElfPartsFromSections(const std::vector<std::shared_ptr<IElfSection<Elf64_Shdr>>>&,
                                                              StringTableSection<Elf64_Shdr>*,
                                                              IElfPartFromSectionVisitor&) = 0;

    virtual ~IElfPartAssembler() = default;
};

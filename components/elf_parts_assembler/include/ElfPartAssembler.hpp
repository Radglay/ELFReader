#pragma once

#include "IElfPartAssembler.hpp"
#include "StringTableSection.hpp"


class ElfPartAssembler : public IElfPartAssembler
{
public:
    ElfPart assembleElfPartFromFileHeader(Elf32_Ehdr&) override;
    ElfPart assembleElfPartFromFileHeader(Elf64_Ehdr&) override;

    std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf32_Shdr>>&, int) override;
    std::vector<ElfPart> assembleElfPartsFromSectionHeaders(const std::vector<std::shared_ptr<Elf64_Shdr>>&, int) override;

    std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf32_Phdr>&, int) override;
    std::vector<ElfPart> assembleElfPartsFromProgramHeaders(const std::vector<Elf64_Phdr>&, int) override;

    std::vector<ElfPart> assembleElfPartsFromSections(const std::vector<std::shared_ptr<IElfSection<Elf32_Shdr>>>&,
                                                      StringTableSection<Elf32_Shdr>*,
                                                      IElfPartFromSectionVisitor&) override;

    std::vector<ElfPart> assembleElfPartsFromSections(const std::vector<std::shared_ptr<IElfSection<Elf64_Shdr>>>&,
                                                      StringTableSection<Elf64_Shdr>*,
                                                      IElfPartFromSectionVisitor&) override;
};

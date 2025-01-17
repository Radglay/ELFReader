#pragma once

#include "IElfPartAssembler.hpp"


class ElfPartAssembler : public IElfPartAssembler
{
public:
    ElfPart assembleElfPartFromFileHeader(Elf32_Ehdr&) override;
    ElfPart assembleElfPartFromFileHeader(Elf64_Ehdr&) override;

    ElfPart assembleElfPartFromSectionHeader(Elf32_Shdr&, int) override;
    ElfPart assembleElfPartFromSectionHeader(Elf64_Shdr&, int) override;

    ElfPart assembleElfPartFromProgramHeader(Elf32_Phdr&, int) override;
    ElfPart assembleElfPartFromProgramHeader(Elf64_Phdr&, int) override;

    ElfPart assembleElfPartFromSection(NoteSection<Elf32_Shdr, Elf32_Nhdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(NoteSection<Elf64_Shdr, Elf64_Nhdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf32_Rel>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&) override;
};

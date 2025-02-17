#pragma once

#include "IElfPartFromSectionVisitor.hpp"


class ElfPartFromSectionVisitor : public IElfPartFromSectionVisitor
{
public:
    ElfPart assembleElfPartFromSection(INoteSection<Elf32_Shdr, Elf32_Nhdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(INoteSection<Elf64_Shdr, Elf64_Nhdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf64_Rel>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(RelocationWithAddendSection<Elf64_Shdr, Elf64_Rela>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(NullSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(NullSection<Elf64_Shdr>&, const std::string&) override;

    ElfPart assembleElfPartFromSection(UnknownSection<Elf32_Shdr>&, const std::string&) override;
    ElfPart assembleElfPartFromSection(UnknownSection<Elf64_Shdr>&, const std::string&) override;
};

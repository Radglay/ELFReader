#include "ElfPartFromSectionVisitor.hpp"
#include <elf.h>
#include "INoteSection.hpp"
#include "NobitsSection.hpp"
#include "ProgbitsSection.hpp"
#include "RelocationSection.hpp"
#include "RelocationWithAddendSection.hpp"
#include "StringTableSection.hpp"
#include "SymbolTableSection.hpp"
#include <QString>


ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(INoteSection<Elf32_Shdr, Elf32_Nhdr>& p_noteSection, const std::string& p_sectionName)
{
    auto l_noteHeader { p_noteSection.getNoteHeader() };

    auto l_offset { static_cast<int>(p_noteSection.getSectionHeader()->sh_offset) };

    std::vector<ElfField> l_fields;

    l_fields.emplace_back("n_namesz", "Elf32_Word", QString::number(l_noteHeader.n_namesz, 16).toUpper(), "");
    l_fields.emplace_back("n_descsz", "Elf32_Word", QString::number(l_noteHeader.n_descsz, 16).toUpper(), "");
    l_fields.emplace_back("n_type", "Elf32_Word", QString::number(l_noteHeader.n_type, 16).toUpper(), "");

    return ElfPart{QString::fromStdString(p_sectionName), ElfPartType::Section, l_offset, sizeof(Elf32_Nhdr), "", l_fields};
}

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(INoteSection<Elf64_Shdr, Elf64_Nhdr>& p_noteSection, const std::string& p_sectionName)
{
    auto l_noteHeader { p_noteSection.getNoteHeader() };

    auto l_offset { static_cast<int>(p_noteSection.getSectionHeader()->sh_offset) };

    std::vector<ElfField> l_fields;

    l_fields.emplace_back("n_namesz", "Elf64_Word", QString::number(l_noteHeader.n_namesz, 16).toUpper(), "");
    l_fields.emplace_back("n_descsz", "Elf64_Word", QString::number(l_noteHeader.n_descsz, 16).toUpper(), "");
    l_fields.emplace_back("n_type", "Elf64_Word", QString::number(l_noteHeader.n_type, 16).toUpper(), "");

    return ElfPart{QString::fromStdString(p_sectionName), ElfPartType::Section, l_offset, sizeof(Elf64_Nhdr), "", l_fields};
}

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf64_Rel>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(RelocationWithAddendSection<Elf64_Shdr, Elf64_Rela>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&)
{ return {}; }

ElfPart ElfPartFromSectionVisitor::assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&)
{ return {}; }

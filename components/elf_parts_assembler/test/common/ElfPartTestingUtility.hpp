#pragma once

#include <QString>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <vector>


struct ElfPart;
struct ElfField;


const QString UNSIGNED_CHAR_TYPE_TEXT { "unsigned char" };
const QString UNSIGNED_CHAR_ARRAY_TYPE_TEXT { "unsigned char[]" };
const QString ELF32_HALF_TEXT { "Elf32_Half" };
const QString ELF32_WORD_TEXT { "Elf32_Word" };
const QString ELF32_ADDR_TEXT { "Elf32_Addr" };
const QString ELF32_OFF_TEXT { "Elf32_Off" };

const QString ELF64_HALF_TEXT { "Elf64_Half" };
const QString ELF64_WORD_TEXT { "Elf64_Word" };
const QString ELF64_XWORD_TEXT { "Elf64_Xword" };
const QString ELF64_ADDR_TEXT { "Elf64_Addr" };
const QString ELF64_OFF_TEXT { "Elf64_Off" };

const QString UINT32_T_TEXT { "uint32_t" };
const QString INT_TEXT { "int" };
const QString BOOL_TEXT { "bool" };


struct ElfPartAssemblingTestSuite : public ::testing::Test
{
    void expectElfPartsAreEqual(const ElfPart&, const ElfPart&);
    void expectElfFieldsAreEqual(const ElfField&, const ElfField&);
    void expectElfPartContainersAreEqual(const std::vector<ElfPart>&, const std::vector<ElfPart>&);

private:
    void expectElfFieldTextContentsAreEqual(const ElfField&, const ElfField&);
    void expectElfPartTextContentsAreEqual(const ElfPart&, const ElfPart&);
};

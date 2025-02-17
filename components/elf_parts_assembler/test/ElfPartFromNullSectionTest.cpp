#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "ElfPartFromSectionVisitor.hpp"
#include "ElfPart.hpp"
#include "NullSection.hpp"
#include <string>
#include <QString>
#include <memory>


namespace
{

const Elf32_Shdr NULL_SECTION_HEADER_32BIT
{
    .sh_type = SHT_NULL,
    .sh_offset = 0x0,
    .sh_size = 0
};

const Elf64_Shdr NULL_SECTION_HEADER_64BIT
{
    .sh_type = SHT_NULL,
    .sh_offset = 0x0,
    .sh_size = 0
};
    
const std::string NULL_SECTION_NAME_VALUE {};


const QString NULL_SECTION_ELF_PART_NAME {};
const ElfPartType NULL_SECTION_ELF_PART_TYPE { ElfPartType::Section };
constexpr int NULL_SECTION_ELF_PART_OFFSET {};
constexpr int NULL_SECTION_ELF_PART_SIZE {};
const QString NULL_SECTION_ELF_PART_DESCRIPTION { "Null Section" };

}

using namespace ::testing;


class ElfPartFromNullSectionTestSuiteX32 : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartFromNullSectionTestSuiteX32, shouldAssembleElfPartWhenNullSectionIsProvided)
{
    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor {};

    NullSection<Elf32_Shdr> l_nullSection { std::make_shared<Elf32_Shdr>(NULL_SECTION_HEADER_32BIT) };
    auto l_targetElfPart { l_elfPartFromSectionVisitor.assembleElfPartFromSection(l_nullSection, NULL_SECTION_NAME_VALUE) };
    
    ElfPart l_expectedElfPart {
        NULL_SECTION_ELF_PART_NAME,
        NULL_SECTION_ELF_PART_TYPE,
        NULL_SECTION_ELF_PART_OFFSET,
        NULL_SECTION_ELF_PART_SIZE,
        NULL_SECTION_ELF_PART_DESCRIPTION };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}


class ElfPartFromNullSectionTestSuiteX64 : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartFromNullSectionTestSuiteX64, shouldAssembleElfPartWhenNullSectionIsProvided)
{
    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor {};

    NullSection<Elf64_Shdr> l_nullSection { std::make_shared<Elf64_Shdr>(NULL_SECTION_HEADER_64BIT) };
    auto l_targetElfPart { l_elfPartFromSectionVisitor.assembleElfPartFromSection(l_nullSection, NULL_SECTION_NAME_VALUE) };
    
    ElfPart l_expectedElfPart {
        NULL_SECTION_ELF_PART_NAME,
        NULL_SECTION_ELF_PART_TYPE,
        NULL_SECTION_ELF_PART_OFFSET,
        NULL_SECTION_ELF_PART_SIZE,
        NULL_SECTION_ELF_PART_DESCRIPTION };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}
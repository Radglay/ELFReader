#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "ElfPartFromSectionVisitor.hpp"
#include "ElfPart.hpp"
#include "UnknownSection.hpp"
#include <string>
#include <QString>
#include <memory>


namespace
{

const Elf32_Shdr UNKNOWN_SECTION_HEADER_32BIT
{
    .sh_type = 12345,
    .sh_offset = 0x123,
    .sh_size = 45
};

const Elf64_Shdr UNKNOWN_SECTION_HEADER_64BIT
{
    .sh_type = 12345,
    .sh_offset = 0x1234,
    .sh_size = 56
};
    
const std::string UNKNOWN_SECTION_NAME_VALUE {};


const QString UNKNOWN_SECTION_ELF_PART_NAME {};
const ElfPartType UNKNOWN_SECTION_ELF_PART_TYPE { ElfPartType::Section };
constexpr int UNKNOWN_SECTION_ELF_PART_OFFSET_32BIT { 0x123 };
constexpr int UNKNOWN_SECTION_ELF_PART_OFFSET_64BIT { 0x1234 };
constexpr int UNKNOWN_SECTION_ELF_PART_SIZE_32BIT { 45 };
constexpr int UNKNOWN_SECTION_ELF_PART_SIZE_64BIT { 56 };
const QString UNKNOWN_SECTION_ELF_PART_DESCRIPTION { "Unknown Section" };

}

using namespace ::testing;


class ElfPartFromUnknownSectionTestSuiteX32 : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartFromUnknownSectionTestSuiteX32, shouldAssembleElfPartWhenUnknownSectionIsProvided)
{
    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor {};

    UnknownSection<Elf32_Shdr> l_unknownSection { std::make_shared<Elf32_Shdr>(UNKNOWN_SECTION_HEADER_32BIT) };
    auto l_targetElfPart { l_elfPartFromSectionVisitor.assembleElfPartFromSection(l_unknownSection, UNKNOWN_SECTION_NAME_VALUE) };
    
    ElfPart l_expectedElfPart {
        UNKNOWN_SECTION_ELF_PART_NAME,
        UNKNOWN_SECTION_ELF_PART_TYPE,
        UNKNOWN_SECTION_ELF_PART_OFFSET_32BIT,
        UNKNOWN_SECTION_ELF_PART_SIZE_32BIT,
        UNKNOWN_SECTION_ELF_PART_DESCRIPTION };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}


class ElfPartFromUnknownSectionTestSuiteX64 : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartFromUnknownSectionTestSuiteX64, shouldAssembleElfPartWhenUnknownSectionIsProvided)
{
    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor {};

    UnknownSection<Elf64_Shdr> l_unknownSection { std::make_shared<Elf64_Shdr>(UNKNOWN_SECTION_HEADER_64BIT) };
    auto l_targetElfPart { l_elfPartFromSectionVisitor.assembleElfPartFromSection(l_unknownSection, UNKNOWN_SECTION_NAME_VALUE) };
    
    ElfPart l_expectedElfPart {
        UNKNOWN_SECTION_ELF_PART_NAME,
        UNKNOWN_SECTION_ELF_PART_TYPE,
        UNKNOWN_SECTION_ELF_PART_OFFSET_64BIT,
        UNKNOWN_SECTION_ELF_PART_SIZE_64BIT,
        UNKNOWN_SECTION_ELF_PART_DESCRIPTION };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}
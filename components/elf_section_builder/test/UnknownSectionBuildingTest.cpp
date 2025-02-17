#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "UnknownSection.hpp"
#include <elf.h>
#include "ElfSectionBuilder.hpp"
#include <sstream>
#include "TargetMachineInfo.hpp"
#include "NoteDescriptorBuilderMock.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"


namespace
{

Elf32_Shdr UNKNOWN_SECTION_HEADER_32BIT
{
    .sh_type = 12345
};

Elf64_Shdr UNKNOWN_SECTION_HEADER_64BIT
{
    .sh_type = 12345
};

}

using namespace ::testing;

class UnknownSectionBuildingX32BitTestSuite : public Test
{};

TEST_F(UnknownSectionBuildingX32BitTestSuite, shouldBuildUnknownSectionWhenSectionTypeIsInvalid)
{
    std::stringstream l_stubStream;
    TargetMachineInfo l_targetMachineInfo;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf32_Shdr>(&UNKNOWN_SECTION_HEADER_32BIT) };
    l_elfObjectBuilder.buildUnknownSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetUnknownSection { dynamic_cast<UnknownSection<Elf32_Shdr>*>(l_sections[0].get())};
    EXPECT_THAT(l_targetUnknownSection, NotNull());
}

class UnknownSectionBuildingX64BitTestSuite : public Test
{};

TEST_F(UnknownSectionBuildingX64BitTestSuite, shouldBuildUnknownSectionWhenSectionTypeIsInvalid)
{
    std::stringstream l_stubStream;
    TargetMachineInfo l_targetMachineInfo;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf64_Shdr>(&UNKNOWN_SECTION_HEADER_64BIT) };
    l_elfObjectBuilder.buildUnknownSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetUnknownSection { dynamic_cast<UnknownSection<Elf64_Shdr>*>(l_sections[0].get())};
    EXPECT_THAT(l_targetUnknownSection, NotNull());
}


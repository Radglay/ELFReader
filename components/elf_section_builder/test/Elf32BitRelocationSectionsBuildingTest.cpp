#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfSectionBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX32.hpp"
#include "ElfStructureInfoX32.hpp"
#include <tuple>
#include <vector>
#include <elf.h>
#include <algorithm>
#include "RelocationSection.hpp"
#include <memory>
#include "NoteDescriptorBuilderMock.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };


Elf32_Shdr REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS
{
    .sh_type = SHT_REL,
    .sh_size = 0x0
};

Elf32_Shdr REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS
{
    .sh_type = SHT_REL,
    .sh_size = 5 * sizeof(Elf32_Rel)
};


// (1)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1[] { 0x78, 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_1[] { 0x08, 0x0, 0x0, 0x0 };

Elf32_Rel REL_HEADER_1
{
    .r_offset = 0x3d'78,
    .r_info = 0x08
};


// (2)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2[] { static_cast<char>(0x80), 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_2[] { 0x08, 0x0, 0x0, 0x0 };

Elf32_Rel REL_HEADER_2
{
    .r_offset = 0x3d'80,
    .r_info = 0x08
};


// (3)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3[] {static_cast<char>(0x88), 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_3[] { 0x08, 0x0, 0x0, 0x0 };

Elf32_Rel REL_HEADER_3
{
    .r_offset = 0x3d'88,
    .r_info = 0x08
};


// (4)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4[] { 0x08, 0x40, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_4[] { 0x08, 0x0, 0x0, 0x0 };

Elf32_Rel REL_HEADER_4
{
    .r_offset = 0x40'08,
    .r_info = 0x08
};


// (5)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5[] { static_cast<char>(0xd0), 0x3f, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_5[] { 0x06, 0x0, 0x0, 0x0b };

Elf32_Rel REL_HEADER_5
{
    .r_offset = 0x3f'd0,
    .r_info = 0x0b'00'00'06
};


std::string generate32BitLittleEndianRelocationHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_1, sizeof(Elf32_Word));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_2, sizeof(Elf32_Word));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_3, sizeof(Elf32_Word));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_4, sizeof(Elf32_Word));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_5, sizeof(Elf32_Word));

    return l_streamContent;
}

std::string generate32BitBigEndianRelocationHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_REL_1),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_REL_1));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_REL_2),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_REL_2));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_REL_3),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_REL_3));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_REL_4),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_REL_4));


    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_REL_5),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_REL_5));

    return l_streamContent;
}

}

using namespace ::testing;


class Elf32BitRelocationSetionBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectRelocationSectionsAreEqual(const RelocationSection<Elf32_Shdr, Elf32_Rel>& p_targetRelSection,
                                          const RelocationSection<Elf32_Shdr, Elf32_Rel>& p_expectedRelSection);
};


void Elf32BitRelocationSetionBuildingTestSuite::expectRelocationSectionsAreEqual(const RelocationSection<Elf32_Shdr, Elf32_Rel>& p_targetRelSection,
                                                                                 const RelocationSection<Elf32_Shdr, Elf32_Rel>& p_expectedRelSection)
{
    auto l_targetSectionHeader { p_targetRelSection.getSectionHeader() };
    auto l_expectedSectionHeader { p_expectedRelSection.getSectionHeader() };

    EXPECT_EQ(std::addressof(*l_targetSectionHeader), std::addressof(*l_expectedSectionHeader));

    auto l_targetRelHeaders { p_targetRelSection.getRelocationHeaders() };
    auto l_expectedRelHeaders { p_expectedRelSection.getRelocationHeaders() };

    ASSERT_EQ(l_targetRelHeaders.size(), l_expectedRelHeaders.size());

    for (int i = 0; i < l_expectedRelHeaders.size(); ++i)
    {
        EXPECT_THAT(l_targetRelHeaders[i], FieldsAre(
            l_expectedRelHeaders[i].r_offset,
            l_expectedRelHeaders[i].r_info
        ));
    }
}


TEST_P(Elf32BitRelocationSetionBuildingTestSuite, shouldReadRelocationSectionWithoutAnyRelocationHeaderWhenSectionHeaderSizeIsZero)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf32_Shdr>(&REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS) };

    l_elfObjectBuilder.buildRelocationSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationSection<Elf32_Shdr, Elf32_Rel>&>(*l_sections[0])};
    
    std::vector<Elf32_Rel> l_expectedRelocationHeaders {};

    auto l_expectedRelocationSection { RelocationSection<Elf32_Shdr, Elf32_Rel>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

TEST_P(Elf32BitRelocationSetionBuildingTestSuite, shouldReadRelocationSectionWithAllFiveRelocationHeaders)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf32_Shdr>(&REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS) };

    l_elfObjectBuilder.buildRelocationSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationSection<Elf32_Shdr, Elf32_Rel>&>(*l_sections[0])};

    std::vector<Elf32_Rel> l_expectedRelocationHeaders {
        REL_HEADER_1, REL_HEADER_2, REL_HEADER_3, REL_HEADER_4, REL_HEADER_5 };

    auto l_expectedRelocationSection { RelocationSection<Elf32_Shdr, Elf32_Rel>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

INSTANTIATE_TEST_SUITE_P(RelocationSectionsBuilding,
                         Elf32BitRelocationSetionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianRelocationHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianRelocationHeaders())));

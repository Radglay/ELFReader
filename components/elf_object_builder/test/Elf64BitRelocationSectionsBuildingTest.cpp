#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilderMock.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX64.hpp"
#include <tuple>
#include <vector>
#include <elf.h>
#include <algorithm>
#include "RelocationSection.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };


Elf64_Shdr REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS
{
    .sh_type = SHT_REL,
    .sh_size = 0x0
};

Elf64_Shdr REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS
{
    .sh_type = SHT_REL,
    .sh_size = 5 * sizeof(Elf64_Rel)
};


// (1)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1[] { 0x78, 0x3d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_1[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

Elf64_Rel REL_HEADER_1
{
    .r_offset = 0x3d'78,
    .r_info = 0x08
};


// (2)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2[] { static_cast<char>(0x80), 0x3d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_2[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

Elf64_Rel REL_HEADER_2
{
    .r_offset = 0x3d'80,
    .r_info = 0x08
};


// (3)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3[] {static_cast<char>(0x88), 0x3d, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_3[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

Elf64_Rel REL_HEADER_3
{
    .r_offset = 0x3d'88,
    .r_info = 0x08
};


// (4)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4[] { 0x08, 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_4[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

Elf64_Rel REL_HEADER_4
{
    .r_offset = 0x40'08,
    .r_info = 0x08
};


// (5)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5[] { static_cast<char>(0xd0), 0x3f, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_REL_5[] { 0x06, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0b };

Elf64_Rel REL_HEADER_5
{
    .r_offset = 0x3f'd0,
    .r_info = 0x0b'00'00'00'00'00'00'06
};


std::string generate64BitLittleEndianRelocationHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_1, sizeof(Elf64_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_1, sizeof(Elf64_Xword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_2, sizeof(Elf64_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_2, sizeof(Elf64_Xword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_3, sizeof(Elf64_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_3, sizeof(Elf64_Xword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_4, sizeof(Elf64_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_4, sizeof(Elf64_Xword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_REL_5, sizeof(Elf64_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_REL_5, sizeof(Elf64_Xword));

    return l_streamContent;
}

std::string generate64BitBigEndianRelocationHeaders()
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


class Elf64BitRelocationSectionBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectRelocationSectionsAreEqual(const RelocationSection<Elf64_Shdr, Elf64_Rel>& p_targetRelSections,
                                          const RelocationSection<Elf64_Shdr, Elf64_Rel>& p_expectedRelSections);
};

void Elf64BitRelocationSectionBuildingTestSuite::expectRelocationSectionsAreEqual(const RelocationSection<Elf64_Shdr, Elf64_Rel>& p_targetRelSection,
                                                                                  const RelocationSection<Elf64_Shdr, Elf64_Rel>& p_expectedRelSection)
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


TEST_P(Elf64BitRelocationSectionBuildingTestSuite, shouldNotReadAnyRelocationHeaderWhenThereIsNoRelocationSectionHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    NiceMock<ElfStructureInfoBuilderMock<ElfStructureInfoX64>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    auto l_sectionHeader { std::shared_ptr<Elf64_Shdr>(&REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS) };

    ElfStructureInfoX64 l_stubElfStructureInfo;
    l_stubElfStructureInfo.sectionHeaders.push_back(l_sectionHeader);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationSections();

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationSection<Elf64_Shdr, Elf64_Rel>&>(*l_sections[0])};
    
    std::vector<Elf64_Rel> l_expectedRelocationHeaders {};

    auto l_expectedRelocationSection { RelocationSection<Elf64_Shdr, Elf64_Rel>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

TEST_P(Elf64BitRelocationSectionBuildingTestSuite, shouldReadAllRelocationsHeaderWhenThereIsRelocationSectionHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    NiceMock<ElfStructureInfoBuilderMock<ElfStructureInfoX64>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    auto l_sectionHeader { std::shared_ptr<Elf64_Shdr>(&REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS) };
    ElfStructureInfoX64 l_stubElfStructureInfo;
    l_stubElfStructureInfo.sectionHeaders.push_back(l_sectionHeader);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationSections();

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationSection<Elf64_Shdr, Elf64_Rel>&>(*l_sections[0])};

    std::vector<Elf64_Rel> l_expectedRelocationHeaders {
        REL_HEADER_1, REL_HEADER_2, REL_HEADER_3, REL_HEADER_4, REL_HEADER_5 };

    auto l_expectedRelocationSection { RelocationSection<Elf64_Shdr, Elf64_Rel>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

INSTANTIATE_TEST_SUITE_P(RelocationSectionsBuilding,
                         Elf64BitRelocationSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate64BitLittleEndianRelocationHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate64BitBigEndianRelocationHeaders())));

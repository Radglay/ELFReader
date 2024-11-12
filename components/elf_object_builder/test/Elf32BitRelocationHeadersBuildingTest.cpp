#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilderMock.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX32.hpp"
#include "ElfStructureInfoX32.hpp"
#include <tuple>
#include <vector>
#include <elf.h>
#include <algorithm>


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };


constexpr int FIRST_RELA_HEADER_INDEX { 0 };


constexpr Elf32_Shdr REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS
{
    .sh_type = SHT_REL,
    .sh_size = 0x0
};

constexpr Elf32_Shdr REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS
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


class Elf32BitRelocationHeaderBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectRelocationHeadersAreEqual(const std::vector<Elf32_Rel>& p_targetRelocationHeaders,
                                         const std::vector<Elf32_Rel>& p_expectedRelocationHeaders);
};

void Elf32BitRelocationHeaderBuildingTestSuite::expectRelocationHeadersAreEqual(
    const std::vector<Elf32_Rel>& p_targetRelocationHeaders,
    const std::vector<Elf32_Rel>& p_expectedRelocationHeaders)
{
    ASSERT_EQ(p_targetRelocationHeaders.size(), p_expectedRelocationHeaders.size());

    auto l_size = p_expectedRelocationHeaders.size();

    for (int i = 0; i < l_size; ++i)
    {
        const auto& l_targetRelaHeader { p_targetRelocationHeaders[i] };
        const auto& l_expectedRelaHeader { p_expectedRelocationHeaders[i] };
        EXPECT_THAT(l_targetRelaHeader, FieldsAre(
            l_expectedRelaHeader.r_offset,
            l_expectedRelaHeader.r_info       
        ));
    }
}

TEST_P(Elf32BitRelocationHeaderBuildingTestSuite, shouldNotReadAnyRelocationHeaderWhenThereIsNoRelocationSectionHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    NiceMock<ElfStructureInfoBuilderMock<ElfStructureInfoX32>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    ElfStructureInfoX32 l_stubElfStructureInfo;
    l_stubElfStructureInfo.sectionHeaders.push_back(REL_SECTION_HEADER_WITH_ZERO_REL_HEADERS);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationHeaders();

    auto l_targetRelocationHeaders { l_elfObjectBuilder.getResult()->relocationHeaders[FIRST_RELA_HEADER_INDEX] };

    std::vector<Elf32_Rel> l_expectedRelocationHeaders;

    expectRelocationHeadersAreEqual(l_targetRelocationHeaders, l_expectedRelocationHeaders);
}

TEST_P(Elf32BitRelocationHeaderBuildingTestSuite, shouldReadAllRelocationsHeaderWhenThereIsRelocationSectionHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    NiceMock<ElfStructureInfoBuilderMock<ElfStructureInfoX32>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    ElfStructureInfoX32 l_stubElfStructureInfo;
    l_stubElfStructureInfo.sectionHeaders.push_back(REL_SECTION_HEADER_WITH_FIVE_REL_HEADERS);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationHeaders();

    auto l_targetRelocationHeaders { l_elfObjectBuilder.getResult()->relocationHeaders[FIRST_RELA_HEADER_INDEX] };

    std::vector<Elf32_Rel> l_expectedRelocationHeaders {
        REL_HEADER_1, REL_HEADER_2, REL_HEADER_3, REL_HEADER_4, REL_HEADER_5 };

    expectRelocationHeadersAreEqual(l_targetRelocationHeaders, l_expectedRelocationHeaders);
}

INSTANTIATE_TEST_SUITE_P(RelocationHeadersBuilding,
                         Elf32BitRelocationHeaderBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianRelocationHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianRelocationHeaders())));

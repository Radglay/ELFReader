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


constexpr Elf32_Shdr RELA_SECTION_HEADER_WITH_ZERO_RELA_HEADERS
{
    .sh_type = SHT_RELA,
    .sh_size = 0x0
};

constexpr Elf32_Shdr RELA_SECTION_HEADER_WITH_FIVE_RELA_HEADERS
{
    .sh_type = SHT_RELA,
    .sh_size = 5 * sizeof(Elf32_Rela)
};


// (1)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_1[] { 0x78, 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_RELA_1[] { 0x08, 0x0, 0x0, 0x0 };
constexpr char R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_1[] { static_cast<char>(0xc0), 0x11, 0x0, 0x0 };

Elf32_Rela RELA_HEADER_1
{
    .r_offset = 0x3d'78,
    .r_info = 0x08,
    .r_addend = 0x11'c0
};


// (2)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_2[] { static_cast<char>(0x80), 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_RELA_2[] { 0x08, 0x0, 0x0, 0x0 };
constexpr char R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_2[] { static_cast<char>(0x98), 0x12, 0x0, 0x0 };

Elf32_Rela RELA_HEADER_2
{
    .r_offset = 0x3d'80,
    .r_info = 0x08,
    .r_addend = 0x12'98
};


// (3)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_3[] {static_cast<char>(0x88), 0x3d, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_RELA_3[] { 0x08, 0x0, 0x0, 0x0 };
constexpr char R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_3[] { static_cast<char>(0x80), 0x11, 0x0, 0x0 };

Elf32_Rela RELA_HEADER_3
{
    .r_offset = 0x3d'88,
    .r_info = 0x08,
    .r_addend = 0x11'80
};


// (4)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_4[] { 0x08, 0x40, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_RELA_4[] { 0x08, 0x0, 0x0, 0x0 };
constexpr char R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_4[] { 0x08, 0x40, 0x0, 0x0 };

Elf32_Rela RELA_HEADER_4
{
    .r_offset = 0x40'08,
    .r_info = 0x08,
    .r_addend = 0x40'08
};


// (5)
constexpr char R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_5[] { static_cast<char>(0xd0), 0x3f, 0x0, 0x0 };
constexpr char R_INFO_LITTLE_ENDIAN_VALUE_RELA_5[] { 0x06, 0x0, 0x0, 0x0b };
constexpr char R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_5[] { 0x0, 0x0, 0x0, 0x0 };

Elf32_Rela RELA_HEADER_5
{
    .r_offset = 0x3f'd0,
    .r_info = 0x0b'00'00'06,
    .r_addend = 0x0
};


std::string generate32BitLittleEndianRelocationWithAddendHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_1, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_RELA_1, sizeof(Elf32_Word));
    l_streamContent.append(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_1, sizeof(Elf32_Sword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_2, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_RELA_2, sizeof(Elf32_Word));
    l_streamContent.append(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_2, sizeof(Elf32_Sword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_3, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_RELA_3, sizeof(Elf32_Word));
    l_streamContent.append(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_3, sizeof(Elf32_Sword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_4, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_RELA_4, sizeof(Elf32_Word));
    l_streamContent.append(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_4, sizeof(Elf32_Sword));

    l_streamContent.append(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_5, sizeof(Elf32_Addr));
    l_streamContent.append(R_INFO_LITTLE_ENDIAN_VALUE_RELA_5, sizeof(Elf32_Word));
    l_streamContent.append(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_5, sizeof(Elf32_Sword));

    return l_streamContent;
}

std::string generate32BitBigEndianRelocationWithAddendHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_1),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_1));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_RELA_1),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_RELA_1));
    l_streamContent.append(std::rbegin(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_1),
                           std::rend(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_1));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_2),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_2));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_RELA_2),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_RELA_2));
    l_streamContent.append(std::rbegin(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_2),
                           std::rend(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_2));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_3),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_3));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_RELA_3),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_RELA_3));
    l_streamContent.append(std::rbegin(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_3),
                           std::rend(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_3));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_4),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_4));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_RELA_4),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_RELA_4));
    l_streamContent.append(std::rbegin(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_4),
                           std::rend(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_4));

    l_streamContent.append(std::rbegin(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_5),
                           std::rend(R_OFFSET_LITTLE_ENDIAN_VALUE_RELA_5));
    l_streamContent.append(std::rbegin(R_INFO_LITTLE_ENDIAN_VALUE_RELA_5),
                           std::rend(R_INFO_LITTLE_ENDIAN_VALUE_RELA_5));
    l_streamContent.append(std::rbegin(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_5),
                           std::rend(R_ADDEND_LITTLE_ENDIAN_VALUE_RELA_5));

    return l_streamContent;
}

}

using namespace ::testing;


class Elf32BitRelocationWithAddendHeaderBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectRelocationWithAddendHeadersAreEqual(const std::vector<Elf32_Rela>& p_targetRelocationWithAddendHeaders,
                                                   const std::vector<Elf32_Rela>& p_expectedRelocationWithAddendHeaders);
};

void Elf32BitRelocationWithAddendHeaderBuildingTestSuite::expectRelocationWithAddendHeadersAreEqual(
    const std::vector<Elf32_Rela>& p_targetRelocationWithAddendHeaders,
    const std::vector<Elf32_Rela>& p_expectedRelocationWithAddendHeaders)
{
    ASSERT_EQ(p_targetRelocationWithAddendHeaders.size(), p_expectedRelocationWithAddendHeaders.size());

    auto l_size = p_expectedRelocationWithAddendHeaders.size();

    for (int i = 0; i < l_size; ++i)
    {
        const auto& l_targetRelaHeader { p_targetRelocationWithAddendHeaders[i] };
        const auto& l_expectedRelaHeader { p_expectedRelocationWithAddendHeaders[i] };
        EXPECT_THAT(l_targetRelaHeader, FieldsAre(
            l_expectedRelaHeader.r_offset,
            l_expectedRelaHeader.r_info,
            l_expectedRelaHeader.r_addend         
        ));
    }
}

TEST_P(Elf32BitRelocationWithAddendHeaderBuildingTestSuite, shouldNotReadAnyRelocationWithAddendHeaderWhenThereIsNoRelocationSectionHeader)
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
    l_stubElfStructureInfo.sectionHeaders.push_back(RELA_SECTION_HEADER_WITH_ZERO_RELA_HEADERS);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationWithAddendHeaders();

    auto l_targetRelocationWithAddendHeaders { l_elfObjectBuilder.getResult()->relocationWithAddendHeaders };

    std::vector<Elf32_Rela> l_expectedRelocationWithAddendHeaders;

    expectRelocationWithAddendHeadersAreEqual(l_targetRelocationWithAddendHeaders, l_expectedRelocationWithAddendHeaders);
}

TEST_P(Elf32BitRelocationWithAddendHeaderBuildingTestSuite, shouldReadAllRelocationsWithAddendHeaderWhenThereIsRelocationSectionHeader)
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
    l_stubElfStructureInfo.sectionHeaders.push_back(RELA_SECTION_HEADER_WITH_FIVE_RELA_HEADERS);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildRelocationWithAddendHeaders();

    auto l_targetRelocationWithAddendHeaders { l_elfObjectBuilder.getResult()->relocationWithAddendHeaders };

    std::vector<Elf32_Rela> l_expectedRelocationWithAddendHeaders {
        RELA_HEADER_1, RELA_HEADER_2, RELA_HEADER_3, RELA_HEADER_4, RELA_HEADER_5 };

    expectRelocationWithAddendHeadersAreEqual(l_targetRelocationWithAddendHeaders, l_expectedRelocationWithAddendHeaders);
}

INSTANTIATE_TEST_SUITE_P(RelocationWithAddendHeadersBuilding,
                         Elf32BitRelocationWithAddendHeaderBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianRelocationWithAddendHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianRelocationWithAddendHeaders())));

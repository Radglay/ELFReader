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
#include "RelocationWithAddendSection.hpp"
#include <memory>
#include "NoteDescriptorBuilderMock.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };


Elf32_Shdr RELA_SECTION_HEADER_WITH_ZERO_RELA_HEADERS
{
    .sh_type = SHT_RELA,
    .sh_size = 0x0
};

Elf32_Shdr RELA_SECTION_HEADER_WITH_FIVE_RELA_HEADERS
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


class Elf32BitRelocationWithAddendSectionBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectRelocationWithAddendSectionsAreEqual(const RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>& p_targetRelaSection,
                                                    const RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>& p_expectedRelaSection);
};

void Elf32BitRelocationWithAddendSectionBuildingTestSuite::expectRelocationWithAddendSectionsAreEqual(
    const RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>& p_targetRelaSection,
    const RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>& p_expectedRelaSection)
{
    auto l_targetSectionHeader { p_targetRelaSection.getSectionHeader() };
    auto l_expectedSectionHeader { p_expectedRelaSection.getSectionHeader() };

    EXPECT_EQ(std::addressof(*l_targetSectionHeader), std::addressof(*l_expectedSectionHeader));

    auto l_targetRelaHeaders { p_targetRelaSection.getRelocationWithAddendHeaders() };
    auto l_expectedRelaHeaders { p_expectedRelaSection.getRelocationWithAddendHeaders() };

    ASSERT_EQ(l_targetRelaHeaders.size(), l_expectedRelaHeaders.size());

    for (int i = 0; i < l_expectedRelaHeaders.size(); ++i)
    {
        EXPECT_THAT(l_targetRelaHeaders[i], FieldsAre(
            l_expectedRelaHeaders[i].r_offset,
            l_expectedRelaHeaders[i].r_info,
            l_expectedRelaHeaders[i].r_addend
        ));
    }
}

TEST_P(Elf32BitRelocationWithAddendSectionBuildingTestSuite, shouldReadRelocationWithAddendSectionWithoutAnyRelocationWithAddendHeaderWhenSectionHeaderSizeIsZero)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf32_Shdr>(&RELA_SECTION_HEADER_WITH_ZERO_RELA_HEADERS) };

    l_elfObjectBuilder.buildRelocationWithAddendSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>&>(*l_sections[0])};
    
    std::vector<Elf32_Rela> l_expectedRelocationHeaders {};

    auto l_expectedRelocationSection { RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationWithAddendSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

TEST_P(Elf32BitRelocationWithAddendSectionBuildingTestSuite, shouldReadRelocationWithAddendSectionWithAllFiveRelocationWithAddendHeaders)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf32_Shdr>(&RELA_SECTION_HEADER_WITH_FIVE_RELA_HEADERS) };

    l_elfObjectBuilder.buildRelocationWithAddendSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetRelocationSection { dynamic_cast<RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>&>(*l_sections[0])};

    std::vector<Elf32_Rela> l_expectedRelocationHeaders {
        RELA_HEADER_1, RELA_HEADER_2, RELA_HEADER_3, RELA_HEADER_4, RELA_HEADER_5 };

    auto l_expectedRelocationSection { RelocationWithAddendSection<Elf32_Shdr, Elf32_Rela>(l_sectionHeader, l_expectedRelocationHeaders) };
    expectRelocationWithAddendSectionsAreEqual(l_targetRelocationSection, l_expectedRelocationSection);
}

INSTANTIATE_TEST_SUITE_P(RelocationWithAddendSectionsBuilding,
                         Elf32BitRelocationWithAddendSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianRelocationWithAddendHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianRelocationWithAddendHeaders())));

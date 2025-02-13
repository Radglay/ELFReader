#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfSectionBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX64.hpp"
#include <tuple>
#include <vector>
#include <elf.h>
#include <algorithm>
#include <memory>
#include "INoteSection.hpp"
#include "IGnuProperty.hpp"
#include "NoteDescriptorBuilderMock.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };

// (1) NOTE_GNU_PROPERTY
constexpr char N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY[] { 0x20, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY[] { 0x05, 0x0, 0x0, 0x0 };

constexpr char GNU_NAMESPACE_BOTH_ENDIANNESS[] {'G', 'N', 'U', '\0' };
std::string GNU_NAMESPACE { "GNU" };

constexpr int NOTE_GNU_PROPERTY_DESCRIPTOR_OFFSET { 16 };
constexpr int NOTE_GNU_PROPERTY_DESCRIPTOR_SIZE { 32 };

constexpr Elf64_Nhdr NOTE_GNU_PROPERTY
{
    .n_namesz = 4,
    .n_descsz = 32,
    .n_type = 5
};

Elf64_Shdr NOTE_GNU_PROPERTY_SECTION_HEADER
{
    .sh_type = SHT_NOTE,
    .sh_offset = 0x0
};


// (2) NOTE_GNU_BUILD_ID
constexpr char N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID[] { 0x14, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID[] { 0x03, 0x0, 0x0, 0x0 };

constexpr int NOTE_GNU_BUILD_ID_DESCRIPTOR_OFFSET { 16 };
constexpr int NOTE_GNU_BUILD_ID_DESCRIPTOR_SIZE { 20 };

constexpr Elf64_Nhdr NOTE_GNU_BUILD_ID
{
    .n_namesz = 4,
    .n_descsz = 20,
    .n_type = 3
};

Elf64_Shdr NOTE_GNU_BUILD_ID_SECTION_HEADER
{
    .sh_type = SHT_NOTE,
    .sh_offset = 0x0
};


// (3) NOTE_ABI_TAG
constexpr char N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x10, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x01, 0x0, 0x0, 0x0 };

constexpr int NOTE_ABI_TAG_DESCRIPTOR_OFFSET { 16 };

constexpr Elf64_Nhdr NOTE_ABI_TAG
{
    .n_namesz = 4,
    .n_descsz = 16,
    .n_type = 1
};

Elf64_Shdr NOTE_ABI_TAG_SECTION_HEADER
{
    .sh_type = SHT_NOTE,
    .sh_offset = 0x0
};


std::string generate64BitLittleEndianGnuPropertyNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf64_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf64_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf64_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_PROPERTY.n_namesz);

    return l_streamContent;
}

std::string generate64BitLittleEndianGnuBuildIdNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);

    return l_streamContent;
}

std::string generate64BitLittleEndianGnuAbiTagNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);

    return l_streamContent;
}

std::string generate64BitBigEndianGnuPropertyNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_PROPERTY.n_namesz);

    return l_streamContent;
}

std::string generate64BitBigEndianGnuBuildIdNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);

    return l_streamContent;
}

std::string generate64BitBigEndianGnuAbiTagNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);

    return l_streamContent;
}

}


using namespace ::testing;

struct Elf64BitNoteSectionBuildingTestSuite :  TestWithParam<std::tuple<int, std::string>>
{
    void expectSectionHeadersAreEqual(std::shared_ptr<Elf64_Shdr> p_targetSectionHeader, std::shared_ptr<Elf64_Shdr> p_expectedSectionHeader);
    void expectNoteHeadersAreEqual(const Elf64_Nhdr& p_targetNoteHeader, const Elf64_Nhdr& p_expectedNoteHeader);
    void expectNamespacesAreEqual(const std::string& p_targetNamespace, const std::string& p_expectedNamespace);
};

void Elf64BitNoteSectionBuildingTestSuite::expectSectionHeadersAreEqual(std::shared_ptr<Elf64_Shdr> p_targetSectionHeader,
                                                                        std::shared_ptr<Elf64_Shdr> p_expectedSectionHeader)
{
    EXPECT_EQ(std::addressof(*p_targetSectionHeader), std::addressof(*p_expectedSectionHeader));
}

void Elf64BitNoteSectionBuildingTestSuite::expectNoteHeadersAreEqual(const Elf64_Nhdr& p_targetNoteHeader,
                                                                     const Elf64_Nhdr& p_expectedNoteHeader)
{
    EXPECT_THAT(p_targetNoteHeader, FieldsAre(
                p_expectedNoteHeader.n_namesz,
                p_expectedNoteHeader.n_descsz,
                p_expectedNoteHeader.n_type
));
}

void Elf64BitNoteSectionBuildingTestSuite::expectNamespacesAreEqual(const std::string& p_targetNamespace,
                                                                    const std::string& p_expectedNamespace)
{
    EXPECT_EQ(p_targetNamespace, p_expectedNamespace);
}


struct Elf64BitGnuPropertyNoteSectionBuildingTestSuite : Elf64BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf64BitGnuPropertyNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuPropertyNoteHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;
    l_targetMachineInfo.bitVersion = ELFCLASS64;

    StrictMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    EXPECT_CALL(l_noteDescriptorBuilderMock, buildGnuPropertyInformation(NOTE_GNU_PROPERTY_DESCRIPTOR_OFFSET, NOTE_GNU_PROPERTY_DESCRIPTOR_SIZE))
        .Times(1)
        .WillOnce(Return(std::vector<std::shared_ptr<IGnuProperty>>{}));
    
        ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_GNU_PROPERTY_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf64_Shdr, Elf64_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_GNU_PROPERTY);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuPropertyNoteSectionBuilding,
                         Elf64BitGnuPropertyNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate64BitLittleEndianGnuPropertyNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate64BitBigEndianGnuPropertyNoteHeader())));


struct Elf64BitGnuBuildIdNoteSectionBuildingTestSuite : Elf64BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf64BitGnuBuildIdNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuBuildIdNodeHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    StrictMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    EXPECT_CALL(l_noteDescriptorBuilderMock, buildBuildIdInformation(NOTE_GNU_BUILD_ID_DESCRIPTOR_OFFSET, NOTE_GNU_BUILD_ID_DESCRIPTOR_SIZE))
        .Times(1)
        .WillOnce(Return(std::vector<unsigned char>{}));

    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_GNU_BUILD_ID_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf64_Shdr, Elf64_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_GNU_BUILD_ID);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuBuildIdNoteSectionBuilding,
                         Elf64BitGnuBuildIdNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate64BitLittleEndianGnuBuildIdNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate64BitBigEndianGnuBuildIdNoteHeader())));


struct Elf64BitGnuAbiTagNoteSectionBuildingTestSuite : Elf64BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf64BitGnuAbiTagNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuAbiTagNodeHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    StrictMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    EXPECT_CALL(l_noteDescriptorBuilderMock, buildAbiTagInformation(NOTE_ABI_TAG_DESCRIPTOR_OFFSET))
        .Times(1)
        .WillOnce(Return(AbiTagInformation{}));

    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_ABI_TAG_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf64_Shdr, Elf64_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_ABI_TAG);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuAbiTagNoteSectionBuilding,
                         Elf64BitGnuAbiTagNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate64BitLittleEndianGnuAbiTagNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate64BitBigEndianGnuAbiTagNoteHeader())));

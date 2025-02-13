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
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY[] { 0x18, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY[] { 0x05, 0x0, 0x0, 0x0 };

constexpr char GNU_NAMESPACE_BOTH_ENDIANNESS[] {'G', 'N', 'U', '\0' };
std::string GNU_NAMESPACE { "GNU" };

constexpr int NOTE_GNU_PROPERTY_DESCRIPTOR_OFFSET { 16 };
constexpr int NOTE_GNU_PROPERTY_DESCRIPTOR_SIZE { 24 };

constexpr Elf32_Nhdr NOTE_GNU_PROPERTY
{
    .n_namesz = 4,
    .n_descsz = 24,
    .n_type = 5
};

Elf32_Shdr NOTE_GNU_PROPERTY_SECTION_HEADER
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

Elf32_Nhdr NOTE_GNU_BUILD_ID
{
    .n_namesz = 4,
    .n_descsz = 20,
    .n_type = 3
};

Elf32_Shdr NOTE_GNU_BUILD_ID_SECTION_HEADER
{
    .sh_type = SHT_NOTE,
    .sh_offset = 0x0
};


// (3) NOTE_ABI_TAG
constexpr char N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x10, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x01, 0x0, 0x0, 0x0 };

constexpr int NOTE_ABI_TAG_DESCRIPTOR_OFFSET { 16 };

constexpr Elf32_Nhdr NOTE_ABI_TAG
{
    .n_namesz = 4,
    .n_descsz = 16,
    .n_type = 1
};

Elf32_Shdr NOTE_ABI_TAG_SECTION_HEADER
{
    .sh_type = SHT_NOTE,
    .sh_offset = 0x0
};

std::string generate32BitLittleEndianGnuPropertyNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_PROPERTY.n_namesz);

    return l_streamContent;
}

std::string generate32BitLittleEndianGnuBuildIdNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);

    return l_streamContent;
}

std::string generate32BitLittleEndianGnuAbiTagNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);

    return l_streamContent;
}

std::string generate32BitBigEndianGnuPropertyNoteHeader()
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

std::string generate32BitBigEndianGnuBuildIdNoteHeader()
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

std::string generate32BitBigEndianGnuAbiTagNoteHeader()
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

struct  Elf32BitNoteSectionBuildingTestSuite : TestWithParam<std::tuple<int, std::string>>
{
    void expectSectionHeadersAreEqual(std::shared_ptr<Elf32_Shdr> p_targetSectionHeader, std::shared_ptr<Elf32_Shdr> p_expectedSectionHeader);
    void expectNoteHeadersAreEqual(const Elf32_Nhdr& p_targetNoteHeader, const Elf32_Nhdr& p_expectedNoteHeader);
    void expectNamespacesAreEqual(const std::string& p_targetNamespace, const std::string& p_expectedNamespace);
};

void Elf32BitNoteSectionBuildingTestSuite::expectSectionHeadersAreEqual(std::shared_ptr<Elf32_Shdr> p_targetSectionHeader,
                                                                        std::shared_ptr<Elf32_Shdr> p_expectedSectionHeader)
{
    EXPECT_EQ(std::addressof(*p_targetSectionHeader), std::addressof(*p_expectedSectionHeader));

}

void Elf32BitNoteSectionBuildingTestSuite::expectNoteHeadersAreEqual(const Elf32_Nhdr& p_targetNoteHeader,
                                                                     const Elf32_Nhdr& p_expectedNoteHeader)
{
    EXPECT_THAT(p_targetNoteHeader, FieldsAre(
        p_expectedNoteHeader.n_namesz,
        p_expectedNoteHeader.n_descsz,
        p_expectedNoteHeader.n_type
    ));
}

void Elf32BitNoteSectionBuildingTestSuite::expectNamespacesAreEqual(const std::string& p_targetNamespace,
                                                                    const std::string& p_expectedNamespace)
{
    EXPECT_EQ(p_targetNamespace, p_expectedNamespace);
}


struct Elf32BitGnuPropertyNoteSectionBuildingTestSuite : Elf32BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf32BitGnuPropertyNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuPropertyNoteHeader)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;
    l_targetMachineInfo.bitVersion = ELFCLASS32;

    StrictMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    EXPECT_CALL(l_noteDescriptorBuilderMock, buildGnuPropertyInformation(NOTE_GNU_PROPERTY_DESCRIPTOR_OFFSET, NOTE_GNU_PROPERTY_DESCRIPTOR_SIZE))
        .Times(1)
        .WillOnce(Return(std::vector<std::shared_ptr<IGnuProperty>>{}));

    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_GNU_PROPERTY_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf32_Shdr, Elf32_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_GNU_PROPERTY);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuPropertyNoteSectionBuilding,
                         Elf32BitGnuPropertyNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianGnuPropertyNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianGnuPropertyNoteHeader())));


struct Elf32BitGnuBuildIdNoteSectionBuildingTestSuite : Elf32BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf32BitGnuBuildIdNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuBuildIdNodeHeader)
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
    
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_GNU_BUILD_ID_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf32_Shdr, Elf32_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_GNU_BUILD_ID);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuBuildIdNoteSectionBuilding,
                         Elf32BitGnuBuildIdNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianGnuBuildIdNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianGnuBuildIdNoteHeader())));


struct Elf32BitGnuAbiTagNoteSectionBuildingTestSuite : Elf32BitNoteSectionBuildingTestSuite
{};

TEST_P(Elf32BitGnuAbiTagNoteSectionBuildingTestSuite, shouldReadNoteSectionWithGnuAbiTagNodeHeader)
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
    
    ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_ABI_TAG_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<INoteSection<Elf32_Shdr, Elf32_Nhdr>*>(l_sections[0].get()) };

    expectSectionHeadersAreEqual(l_targetNoteSection->getSectionHeader(), l_sectionHeader);
    expectNoteHeadersAreEqual(l_targetNoteSection->getNoteHeader(), NOTE_ABI_TAG);
    expectNamespacesAreEqual(l_targetNoteSection->getNamespace(), GNU_NAMESPACE);
}

INSTANTIATE_TEST_SUITE_P(GnuAbiTagNoteSectionBuilding,
                         Elf32BitGnuAbiTagNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianGnuAbiTagNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianGnuAbiTagNoteHeader())));

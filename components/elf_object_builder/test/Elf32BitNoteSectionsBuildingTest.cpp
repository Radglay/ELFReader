#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfObjectBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX32.hpp"
#include "ElfStructureInfoX32.hpp"
#include <tuple>
#include <vector>
#include <elf.h>
#include <algorithm>
#include <memory>
#include "NoteSection.hpp"


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

std::vector<unsigned char> NOTE_GNU_PROPERTY_DESCRIPTOR_BOTH_ENDIANNESS {
    0x02, 0x00, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x80, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

constexpr Elf32_Nhdr NOTE_GNU_PROPERTY
{
    .n_namesz = 0x04,
    .n_descsz = 0x20,
    .n_type = 0x05
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

std::vector<unsigned char> NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS {
    0x87, 0xf7, 0xb0, 0x6c, 0xa1, 0xce, 0x3f, 0x0b,
    0x73, 0xc8, 0x91, 0x1d, 0x06, 0x8b, 0xcd, 0xa1,
    0xa9, 0xbc, 0x32, 0xf4
};

Elf32_Nhdr NOTE_GNU_BUILD_ID
{
    .n_namesz = 0x04,
    .n_descsz = 0x14,
    .n_type = 0x03
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

std::vector<unsigned char> NOTE_ABI_TAG_DESCRIPTOR_BOTH_ENDIANNESS {
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

constexpr Elf32_Nhdr NOTE_ABI_TAG
{
    .n_namesz = 0x04,
    .n_descsz = 0x10,
    .n_type = 0x01
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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_PROPERTY.n_descsz);

    return l_streamContent;
}

std::string generate32BitLittleEndianGnuBuildIdNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

    return l_streamContent;
}

std::string generate32BitLittleEndianGnuAbiTagNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_ABI_TAG.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_PROPERTY.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_ABI_TAG.n_descsz);

    return l_streamContent;
}

}


using namespace ::testing;

struct  Elf32BitNoteSectionBuildingTestSuite : TestWithParam<std::tuple<int, std::string>>
{
    void expectNoteSectionsAreEqual(const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_targetNoteSection,
                                    const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_expectedNoteSection);
};

void Elf32BitNoteSectionBuildingTestSuite::expectNoteSectionsAreEqual(const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_targetNoteSection,
                                                                      const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_expectedNoteSection)
{
    auto l_targetSectionHeader { p_targetNoteSection.getSectionHeader() };
    auto l_expectedSectionHeader { p_expectedNoteSection.getSectionHeader() };

    EXPECT_EQ(std::addressof(*l_targetSectionHeader), std::addressof(*l_expectedSectionHeader));

    auto l_targetNoteHeader { p_targetNoteSection.getNoteHeader() };
    auto l_expectedNoteHeader { p_expectedNoteSection.getNoteHeader() };
    
    EXPECT_THAT(l_targetNoteHeader, FieldsAre(
        l_expectedNoteHeader.n_namesz,
        l_expectedNoteHeader.n_descsz,
        l_expectedNoteHeader.n_type
    ));

    auto l_targetNamespace { p_targetNoteSection.getNamespace() };
    auto l_expectedNamespace { p_expectedNoteSection.getNamespace() };
    EXPECT_EQ(l_targetNamespace, l_expectedNamespace);
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

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_GNU_PROPERTY_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf32_Shdr, Elf32_Nhdr>&>(*l_sections[0]) };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf32_Shdr, Elf32_Nhdr>>(l_sectionHeader, NOTE_GNU_PROPERTY, GNU_NAMESPACE, NOTE_GNU_PROPERTY_DESCRIPTOR_BOTH_ENDIANNESS)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);
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

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_GNU_BUILD_ID_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf32_Shdr, Elf32_Nhdr>&>(*l_sections[0]) };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf32_Shdr, Elf32_Nhdr>>(l_sectionHeader, NOTE_GNU_BUILD_ID, GNU_NAMESPACE, NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);
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

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf32_Shdr>(NOTE_ABI_TAG_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf32_Shdr, Elf32_Nhdr>&>(*l_sections[0]) };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf32_Shdr, Elf32_Nhdr>>(l_sectionHeader, NOTE_ABI_TAG, GNU_NAMESPACE, NOTE_ABI_TAG_DESCRIPTOR_BOTH_ENDIANNESS)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);
}

INSTANTIATE_TEST_SUITE_P(GnuAbiTagNoteSectionBuilding,
                         Elf32BitGnuAbiTagNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianGnuAbiTagNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianGnuAbiTagNoteHeader())));

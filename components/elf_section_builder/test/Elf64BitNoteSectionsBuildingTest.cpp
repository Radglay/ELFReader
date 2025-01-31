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
#include "NoteSection.hpp"
#include "INoteSection.hpp"
#include <memory>
#include "AbiTagInformation.hpp"
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"
#include "IGnuProperty.hpp"
#include "GnuProperty.hpp"


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

std::vector<unsigned char> NOTE_GNU_PROPERTY_DESCRIPTOR_LITTLE_ENDIAN {
    0x02, 0x00, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x02, 0x80, 0x00, 0xc0, 0x04, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

std::vector<unsigned char> NOTE_GNU_PROPERTY_DESCRIPTOR_BIG_ENDIAN {
    0xc0, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00,
    0xc0, 0x00, 0x80, 0x02, 0x00, 0x00, 0x00, 0x04,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00
};


const GnuPropertyX86InstructionSet GNU_PROPERTY_X86_ISA
{
    .isHardwareSupportRequired = true,
    .instructionSetVersion = GNU_PROPERTY_X86_ISA_1_BASELINE
};

const GnuPropertyX86Features GNU_PROPERTY_X86_FEATURES
{
    .isCompatibleWithIBT = true,
    .isCompatibleWithSHSTK = true
};


constexpr Elf64_Nhdr NOTE_GNU_PROPERTY
{
    .n_namesz = 0x04,
    .n_descsz = 0x20,
    .n_type = 0x05
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

std::vector<unsigned char> NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS {
    0x87, 0xf7, 0xb0, 0x6c, 0xa1, 0xce, 0x3f, 0x0b,
    0x73, 0xc8, 0x91, 0x1d, 0x06, 0x8b, 0xcd, 0xa1,
    0xa9, 0xbc, 0x32, 0xf4
};

constexpr Elf64_Nhdr NOTE_GNU_BUILD_ID
{
    .n_namesz = 0x04,
    .n_descsz = 0x14,
    .n_type = 0x03
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

std::vector<unsigned char> NOTE_ABI_TAG_DESCRIPTOR_LITTLE_ENDIAN {
    0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

std::vector<unsigned char> NOTE_ABI_TAG_DESCRIPTOR_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03,
    0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00
};

const AbiTagInformation ABI_TAG_INFORMATION
{
    .osDescriptor = 0,
    .majorVersion = 3,
    .minorVersion = 2,
    .subminorVersion = 0
};
constexpr Elf64_Nhdr NOTE_ABI_TAG
{
    .n_namesz = 0x04,
    .n_descsz = 0x10,
    .n_type = 0x01
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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTOR_LITTLE_ENDIAN.data()), NOTE_GNU_PROPERTY.n_descsz);

    return l_streamContent;
}

std::string generate64BitLittleEndianGnuBuildIdNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf64_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

    return l_streamContent;
}

std::string generate64BitLittleEndianGnuAbiTagNoteHeader()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf64_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTOR_LITTLE_ENDIAN.data()), NOTE_ABI_TAG.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTOR_BIG_ENDIAN.data()), NOTE_GNU_PROPERTY.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

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
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTOR_BIG_ENDIAN.data()), NOTE_ABI_TAG.n_descsz);

    return l_streamContent;
}

}


using namespace ::testing;

class Elf64BitNoteSectionBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectNoteSectionsAreEqual(const INoteSection<Elf64_Shdr, Elf64_Nhdr>& p_targetNoteSection,
                                    const INoteSection<Elf64_Shdr, Elf64_Nhdr>& p_expectedNoteSection);
};

void Elf64BitNoteSectionBuildingTestSuite::expectNoteSectionsAreEqual(const INoteSection<Elf64_Shdr, Elf64_Nhdr>& p_targetNoteSection,
                                                                      const INoteSection<Elf64_Shdr, Elf64_Nhdr>& p_expectedNoteSection)
{
    auto l_targetSectionHeader { p_targetNoteSection.getSectionHeader() };
    auto l_expectedSectionHeader { p_expectedNoteSection.getSectionHeader() };

    EXPECT_THAT(l_targetSectionHeader, l_expectedSectionHeader);

    auto l_targetNoteHeader { p_targetNoteSection.getNoteHeader() };
    auto l_expectedNoteHeader { p_expectedNoteSection.getNoteHeader() };
    EXPECT_THAT(l_targetNoteHeader, FieldsAre(
        l_expectedNoteHeader.n_namesz,
        l_expectedNoteHeader.n_descsz,
        l_expectedNoteHeader.n_type
    ));
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

    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_GNU_PROPERTY_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf64_Shdr, Elf64_Nhdr, std::vector<std::shared_ptr<IGnuProperty>>>&>(*l_sections[0]) };

    std::vector<std::shared_ptr<IGnuProperty>> l_expectedGnuProperty {
        std::make_shared<GnuProperty<GnuPropertyX86InstructionSet>>(GNU_PROPERTY_X86_ISA),
        std::make_shared<GnuProperty<GnuPropertyX86Features>>(GNU_PROPERTY_X86_FEATURES),
    };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf64_Shdr, Elf64_Nhdr, std::vector<std::shared_ptr<IGnuProperty>>>>(l_sectionHeader,
                                                                                                          NOTE_GNU_PROPERTY,
                                                                                                          GNU_NAMESPACE,
                                                                                                          l_expectedGnuProperty)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);

    auto l_targetDescriptor { l_targetNoteSection.getDescriptor() };

    ASSERT_EQ(l_targetDescriptor.size(), 2);

    auto l_targetX86FeaturesProperty { dynamic_cast<GnuProperty<GnuPropertyX86Features>&>(*l_targetDescriptor[0]).getProperty() };
    EXPECT_EQ(l_targetX86FeaturesProperty.isCompatibleWithIBT, GNU_PROPERTY_X86_FEATURES.isCompatibleWithIBT);
    EXPECT_EQ(l_targetX86FeaturesProperty.isCompatibleWithSHSTK, GNU_PROPERTY_X86_FEATURES.isCompatibleWithSHSTK);

    auto l_targetX86InstructionSetProperty { dynamic_cast<GnuProperty<GnuPropertyX86InstructionSet>&>(*l_targetDescriptor[1]).getProperty() };
    EXPECT_EQ(l_targetX86InstructionSetProperty.isHardwareSupportRequired, GNU_PROPERTY_X86_ISA.isHardwareSupportRequired);
    EXPECT_EQ(l_targetX86InstructionSetProperty.instructionSetVersion, GNU_PROPERTY_X86_ISA.instructionSetVersion);
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

    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_GNU_BUILD_ID_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf64_Shdr, Elf64_Nhdr, std::vector<unsigned char>>&>(*l_sections[0]) };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf64_Shdr, Elf64_Nhdr, std::vector<unsigned char>>>(l_sectionHeader, NOTE_GNU_BUILD_ID, GNU_NAMESPACE, NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);

    EXPECT_THAT(l_targetNoteSection.getDescriptor(), ElementsAreArray(l_expectedNoteSection->getDescriptor()));
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

    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo);

    auto l_sectionHeader { std::make_shared<Elf64_Shdr>(NOTE_ABI_TAG_SECTION_HEADER) };

    l_elfObjectBuilder.buildNoteSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections) };
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetNoteSection { dynamic_cast<NoteSection<Elf64_Shdr, Elf64_Nhdr, AbiTagInformation>&>(*l_sections[0]) };

    auto l_expectedNoteSection { 
        std::make_shared<NoteSection<Elf64_Shdr, Elf64_Nhdr, AbiTagInformation>>(l_sectionHeader, NOTE_ABI_TAG, GNU_NAMESPACE, ABI_TAG_INFORMATION)
    };

    expectNoteSectionsAreEqual(l_targetNoteSection, *l_expectedNoteSection);

    auto l_targetDescriptor { l_targetNoteSection.getDescriptor() };
    auto l_expectedDescriptor { l_expectedNoteSection->getDescriptor() };
    EXPECT_EQ(l_targetDescriptor.osDescriptor, l_expectedDescriptor.osDescriptor);
    EXPECT_EQ(l_targetDescriptor.majorVersion, l_expectedDescriptor.majorVersion);
    EXPECT_EQ(l_targetDescriptor.minorVersion, l_expectedDescriptor.minorVersion);
    EXPECT_EQ(l_targetDescriptor.subminorVersion, l_expectedDescriptor.subminorVersion);
}

INSTANTIATE_TEST_SUITE_P(GnuAbiTagNoteSectionBuilding,
                         Elf64BitGnuAbiTagNoteSectionBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate64BitLittleEndianGnuAbiTagNoteHeader()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate64BitBigEndianGnuAbiTagNoteHeader())));

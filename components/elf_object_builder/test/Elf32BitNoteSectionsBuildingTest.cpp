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

std::vector<unsigned char> NOTE_GNU_PROPERTY_DESCRIPTION_BOTH_ENDIANNESS {
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

std::vector<unsigned char> NOTE_GNU_BUILD_ID_DESCRIPTION_BOTH_ENDIANNESS {
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
    .sh_offset = sizeof(Elf32_Nhdr) + NOTE_GNU_PROPERTY.n_namesz + NOTE_GNU_PROPERTY.n_descsz
};

// (3) NOTE_ABI_TAG
constexpr char N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x10, 0x0, 0x0, 0x0 };
constexpr char N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG[] { 0x01, 0x0, 0x0, 0x0 };

std::vector<unsigned char> NOTE_ABI_TAG_DESCRIPTION_BOTH_ENDIANNESS {
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
    .sh_offset = NOTE_GNU_BUILD_ID_SECTION_HEADER.sh_offset + sizeof(Elf32_Nhdr) + NOTE_GNU_BUILD_ID.n_namesz + NOTE_GNU_BUILD_ID.n_descsz
};

std::string generate32BitLittleEndianNoteHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_PROPERTY.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_GNU_PROPERTY.n_descsz);

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

    l_streamContent.append(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG, sizeof(Elf32_Word));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_ABI_TAG.n_descsz);

    return l_streamContent;
}

std::string generate32BitBigEndianNoteHeaders()
{
    std::string l_streamContent;

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_PROPERTY));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_PROPERTY.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_PROPERTY_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_GNU_PROPERTY.n_descsz);

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_GNU_BUILD_ID));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_GNU_BUILD_ID.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID.n_descsz);

    l_streamContent.append(std::rbegin(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_NAMESZ_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(std::rbegin(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_DESCSZ_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(std::rbegin(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG),
                           std::rend(N_TYPE_LITTLE_ENDIAN_NOTE_ABI_TAG));
    l_streamContent.append(GNU_NAMESPACE_BOTH_ENDIANNESS, NOTE_ABI_TAG.n_namesz);
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_ABI_TAG_DESCRIPTION_BOTH_ENDIANNESS.data()), NOTE_ABI_TAG.n_descsz);

    return l_streamContent;
}

}



using namespace ::testing;

class Elf32BitNoteSectionsBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectNoteSectionsAreEqual(const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_targetNoteSection,
                                    const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_expectedNoteSection);
};

void Elf32BitNoteSectionsBuildingTestSuite::expectNoteSectionsAreEqual(const NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_targetNoteSection,
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


TEST_P(Elf32BitNoteSectionsBuildingTestSuite, shouldNotReadNoteHeaderWhenThereIsNoNoteSectionHeader)
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

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildNoteSections();

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 0);
}

TEST_P(Elf32BitNoteSectionsBuildingTestSuite, shouldReadAllThreeNoteHeadersWhenThereAreThreeNoteSectionHeaders)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    NiceMock<ElfStructureInfoBuilderMock<ElfStructureInfoX32>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    auto l_sectionHeader1 { std::shared_ptr<Elf32_Shdr>(&NOTE_GNU_PROPERTY_SECTION_HEADER) };
    auto l_sectionHeader2 { std::shared_ptr<Elf32_Shdr>(&NOTE_GNU_BUILD_ID_SECTION_HEADER) };
    auto l_sectionHeader3 { std::shared_ptr<Elf32_Shdr>(&NOTE_ABI_TAG_SECTION_HEADER) };

    ElfStructureInfoX32 l_stubElfStructureInfo;
    l_stubElfStructureInfo.sectionHeaders.push_back(l_sectionHeader1);
    l_stubElfStructureInfo.sectionHeaders.push_back(l_sectionHeader2);
    l_stubElfStructureInfo.sectionHeaders.push_back(l_sectionHeader3);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_stubElfStructureInfo));

    l_elfObjectBuilder.buildElfStructureInfo();

    l_elfObjectBuilder.buildNoteSections();

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 3);

    std::vector<NoteSection<Elf32_Shdr, Elf32_Nhdr>> l_expectedNoteSections
        { 
            NoteSection<Elf32_Shdr, Elf32_Nhdr>(l_sectionHeader1, NOTE_GNU_PROPERTY, GNU_NAMESPACE, NOTE_GNU_PROPERTY_DESCRIPTION_BOTH_ENDIANNESS),
            NoteSection<Elf32_Shdr, Elf32_Nhdr>(l_sectionHeader2, NOTE_GNU_BUILD_ID, GNU_NAMESPACE, NOTE_GNU_BUILD_ID_DESCRIPTION_BOTH_ENDIANNESS),
            NoteSection<Elf32_Shdr, Elf32_Nhdr>(l_sectionHeader3, NOTE_ABI_TAG, GNU_NAMESPACE, NOTE_ABI_TAG_DESCRIPTION_BOTH_ENDIANNESS)
        };

    for (int i = 0; i < 3; ++i)
    {
        auto& l_targetNoteSection { dynamic_cast<NoteSection<Elf32_Shdr, Elf32_Nhdr>&>(*l_sections[i])};
        auto& l_expectedNoteSection { l_expectedNoteSections[i] };
        expectNoteSectionsAreEqual(l_targetNoteSection, l_expectedNoteSection);
    }
}

INSTANTIATE_TEST_SUITE_P(NoteSectionsBuilding,
                         Elf32BitNoteSectionsBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generate32BitLittleEndianNoteHeaders()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generate32BitBigEndianNoteHeaders())));

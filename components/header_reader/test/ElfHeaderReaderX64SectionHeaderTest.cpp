#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include "ElfHeaderReaderX64.hpp"
#include "SectionHeader.hpp"
#include <elf.h>


namespace
{

constexpr int ZERO_ELEMENTS_SIZE { 0 };
constexpr int THREE_ELEMENTS_SIZE { 3 };

constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };

constexpr Elf64_Off SECTION_HEADER_TABLE_OFFSET { 0x0 };


// first Section header
constexpr unsigned char SH_NAME_1_VALUE_LITTLE_ENDIAN [] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
constexpr unsigned char SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word SH_NAME_1_VALUE { 0x0 };
constexpr Elf64_Word SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE { 0x0 };
constexpr Elf64_Xword SH_FLAGS_1_VALUE { 0x0 };
constexpr Elf64_Addr SH_ADDR_1_VALUE { 0x0 };
constexpr Elf64_Off SH_OFFSET_1_VALUE { 0x0 };
constexpr Elf64_Xword SH_SIZE_1_VALUE { 0x0 };
constexpr Elf64_Word SH_LINK_1_VALUE { 0x0 };
constexpr Elf64_Word SH_INFO_1_VALUE { 0x0 };
constexpr Elf64_Xword SH_ADDRALIGN_1_VALUE { 0x0 };
constexpr Elf64_Xword SH_ENTSIZE_1_VALUE { 0x0 };

// second Section header
constexpr unsigned char SH_NAME_2_VALUE_LITTLE_ENDIAN [] { 0x1b, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_2_VALUE_LITTLE_ENDIAN[] { 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_2_VALUE_LITTLE_ENDIAN[] { 0x18, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_2_VALUE_LITTLE_ENDIAN[] { 0x18, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_2_VALUE_LITTLE_ENDIAN[] { 0x1c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word SH_NAME_2_VALUE { 0x1b };
constexpr Elf64_Word SH_TYPE_PROGBITS_VALUE { 0x01 };
constexpr Elf64_Xword SH_FLAGS_2_VALUE { 0x02 };
constexpr Elf64_Addr SH_ADDR_2_VALUE { 0x03'18 };
constexpr Elf64_Off SH_OFFSET_2_VALUE { 0x03'18 };
constexpr Elf64_Xword SH_SIZE_2_VALUE { 0x1c };
constexpr Elf64_Word SH_LINK_2_VALUE { 0x0 };
constexpr Elf64_Word SH_INFO_2_VALUE { 0x0 };
constexpr Elf64_Xword SH_ADDRALIGN_2_VALUE { 0x01 };
constexpr Elf64_Xword SH_ENTSIZE_2_VALUE { 0x0 };

// third Section header
constexpr unsigned char SH_NAME_3_VALUE_LITTLE_ENDIAN [] { 0x23, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN[] { 0x07, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_3_VALUE_LITTLE_ENDIAN[] { 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_3_VALUE_LITTLE_ENDIAN[] { 0x38, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_3_VALUE_LITTLE_ENDIAN[] { 0x38, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_3_VALUE_LITTLE_ENDIAN[] { 0x30, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word SH_NAME_3_VALUE { 0x23 };
constexpr Elf64_Word SH_TYPE_NOTES_VALUE { 0x07 };
constexpr Elf64_Xword SH_FLAGS_3_VALUE { 0x02 };
constexpr Elf64_Addr SH_ADDR_3_VALUE { 0x03'38 };
constexpr Elf64_Off SH_OFFSET_3_VALUE { 0x03'38 };
constexpr Elf64_Xword SH_SIZE_3_VALUE { 0x30 };
constexpr Elf64_Word SH_LINK_3_VALUE { 0x0 };
constexpr Elf64_Word SH_INFO_3_VALUE { 0x0 };
constexpr Elf64_Xword SH_ADDRALIGN_3_VALUE { 0x08 };
constexpr Elf64_Xword SH_ENTSIZE_3_VALUE { 0x0 };

}

namespace reader
{

using namespace ::testing;


std::string generate64BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_1_VALUE_LITTLE_ENDIAN, SH_NAME_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_1_VALUE_LITTLE_ENDIAN, SH_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_1_VALUE_LITTLE_ENDIAN, SH_ADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_1_VALUE_LITTLE_ENDIAN, SH_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_1_VALUE_LITTLE_ENDIAN, SH_SIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_1_VALUE_LITTLE_ENDIAN, SH_LINK_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_1_VALUE_LITTLE_ENDIAN, SH_INFO_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_1_VALUE_LITTLE_ENDIAN, SH_NAME_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_1_VALUE_LITTLE_ENDIAN, SH_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_1_VALUE_LITTLE_ENDIAN, SH_ADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_1_VALUE_LITTLE_ENDIAN, SH_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_1_VALUE_LITTLE_ENDIAN, SH_SIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_1_VALUE_LITTLE_ENDIAN, SH_LINK_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_1_VALUE_LITTLE_ENDIAN, SH_INFO_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitProgbitsSectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_2_VALUE_LITTLE_ENDIAN, SH_NAME_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN, SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_2_VALUE_LITTLE_ENDIAN, SH_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_2_VALUE_LITTLE_ENDIAN, SH_ADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_2_VALUE_LITTLE_ENDIAN, SH_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_2_VALUE_LITTLE_ENDIAN, SH_SIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_2_VALUE_LITTLE_ENDIAN, SH_LINK_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_2_VALUE_LITTLE_ENDIAN, SH_INFO_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitProgbitsSectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_2_VALUE_LITTLE_ENDIAN, SH_NAME_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN, SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_2_VALUE_LITTLE_ENDIAN, SH_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_2_VALUE_LITTLE_ENDIAN, SH_ADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_2_VALUE_LITTLE_ENDIAN, SH_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_2_VALUE_LITTLE_ENDIAN, SH_SIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_2_VALUE_LITTLE_ENDIAN, SH_LINK_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_2_VALUE_LITTLE_ENDIAN, SH_INFO_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitNotesSectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_3_VALUE_LITTLE_ENDIAN, SH_NAME_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN, SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_3_VALUE_LITTLE_ENDIAN, SH_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_3_VALUE_LITTLE_ENDIAN, SH_ADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_3_VALUE_LITTLE_ENDIAN, SH_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_3_VALUE_LITTLE_ENDIAN, SH_SIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_3_VALUE_LITTLE_ENDIAN, SH_LINK_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_3_VALUE_LITTLE_ENDIAN, SH_INFO_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitNotesSectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_3_VALUE_LITTLE_ENDIAN, SH_NAME_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN, SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_3_VALUE_LITTLE_ENDIAN, SH_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_3_VALUE_LITTLE_ENDIAN, SH_ADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_3_VALUE_LITTLE_ENDIAN, SH_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_3_VALUE_LITTLE_ENDIAN, SH_SIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_3_VALUE_LITTLE_ENDIAN, SH_LINK_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_3_VALUE_LITTLE_ENDIAN, SH_INFO_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}


TEST(ElfHeaderReaderX64SectionHeaderTestSuite, shouldReadAll64BitLittleEndianSectionHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate64BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent() };
    l_streamContent += generate64BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX64 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
            SECTION_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            LITTLE_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetSectionHeaders.size(), THREE_ELEMENTS_SIZE);

    EXPECT_EQ(l_targetSectionHeaders[0].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[0].header64,
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));
    
    EXPECT_EQ(l_targetSectionHeaders[1].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[1].header64,
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[2].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[2].header64,
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(ElfHeaderReaderX64SectionHeaderTestSuite, shouldReadAll64BitBigEndianSectionHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate64BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent() };
    l_streamContent += generate64BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX64 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
            SECTION_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            BIG_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetSectionHeaders.size(), THREE_ELEMENTS_SIZE);

    EXPECT_EQ(l_targetSectionHeaders[0].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[0].header64,
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[1].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[1].header64,
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[2].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_64_BIT);
    EXPECT_THAT(l_targetSectionHeaders[2].header64,
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(ElfHeaderReaderX64SectionHeaderTestSuite,
    shouldNotReadAny64BitLittleEndianSectionHeaderWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent { generate64BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent() };
    l_streamContent += generate64BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX64 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
        SECTION_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        LITTLE_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };
    
    ASSERT_EQ(l_targetSectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

TEST(ElfHeaderReaderX64SectionHeaderTestSuite,
    shouldNotReadAny64BitBigEndianSectionHeaderWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent { generate64BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent() };
    l_streamContent += generate64BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX64 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
        SECTION_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        BIG_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };
    
    ASSERT_EQ(l_targetSectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

}

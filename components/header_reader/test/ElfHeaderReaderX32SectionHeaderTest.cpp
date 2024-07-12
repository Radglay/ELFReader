#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include "ElfHeaderReaderX32.hpp"
#include "SectionHeader.hpp"
#include <elf.h>


namespace
{

constexpr int ZERO_ELEMENTS_SIZE { 0 };
constexpr int THREE_ELEMENTS_SIZE { 3 };

constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };

constexpr Elf32_Off SECTION_HEADER_TABLE_OFFSET { 0x0 };


// first Section header
constexpr unsigned char SH_NAME_1_VALUE_LITTLE_ENDIAN [] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };

constexpr Elf32_Word SH_NAME_1_VALUE { 0x0 };
constexpr Elf32_Word SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE { 0x0 };
constexpr Elf32_Word SH_FLAGS_1_VALUE { 0x0 };
constexpr Elf32_Addr SH_ADDR_1_VALUE { 0x0 };
constexpr Elf32_Off SH_OFFSET_1_VALUE { 0x0 };
constexpr Elf32_Word SH_SIZE_1_VALUE { 0x0 };
constexpr Elf32_Word SH_LINK_1_VALUE { 0x0 };
constexpr Elf32_Word SH_INFO_1_VALUE { 0x0 };
constexpr Elf32_Word SH_ADDRALIGN_1_VALUE { 0x0 };
constexpr Elf32_Word SH_ENTSIZE_1_VALUE { 0x0 };

// second Section header
constexpr unsigned char SH_NAME_2_VALUE_LITTLE_ENDIAN [] { 0x1b, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_2_VALUE_LITTLE_ENDIAN[] { 0x02, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_2_VALUE_LITTLE_ENDIAN[] { 0x94, 0x01, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_2_VALUE_LITTLE_ENDIAN[] { 0x94, 0x01, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_2_VALUE_LITTLE_ENDIAN[] { 0x13, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };

constexpr Elf32_Word SH_NAME_2_VALUE { 0x1b };
constexpr Elf32_Word SH_TYPE_PROGBITS_VALUE { 0x01 };
constexpr Elf32_Word SH_FLAGS_2_VALUE { 0x02 };
constexpr Elf32_Addr SH_ADDR_2_VALUE { 0x01'94 };
constexpr Elf32_Off SH_OFFSET_2_VALUE { 0x01'94 };
constexpr Elf32_Word SH_SIZE_2_VALUE { 0x13 };
constexpr Elf32_Word SH_LINK_2_VALUE { 0x0 };
constexpr Elf32_Word SH_INFO_2_VALUE { 0x0 };
constexpr Elf32_Word SH_ADDRALIGN_2_VALUE { 0x01 };
constexpr Elf32_Word SH_ENTSIZE_2_VALUE { 0x0 };

// third Section header
constexpr unsigned char SH_NAME_3_VALUE_LITTLE_ENDIAN [] { 0x23, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN[] { 0x07, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_FLAGS_3_VALUE_LITTLE_ENDIAN[] { 0x02, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDR_3_VALUE_LITTLE_ENDIAN[] { 0xa8, 0x01, 0x0, 0x0 };
constexpr unsigned char SH_OFFSET_3_VALUE_LITTLE_ENDIAN[] { 0xa8, 0x01, 0x0, 0x0 };
constexpr unsigned char SH_SIZE_3_VALUE_LITTLE_ENDIAN[] { 0x24, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_LINK_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_INFO_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };

constexpr Elf32_Word SH_NAME_3_VALUE { 0x23 };
constexpr Elf32_Word SH_TYPE_NOTES_VALUE { 0x07 };
constexpr Elf32_Word SH_FLAGS_3_VALUE { 0x02 };
constexpr Elf32_Addr SH_ADDR_3_VALUE { 0x01'a8 };
constexpr Elf32_Off SH_OFFSET_3_VALUE { 0x01'a8 };
constexpr Elf32_Word SH_SIZE_3_VALUE { 0x24 };
constexpr Elf32_Word SH_LINK_3_VALUE { 0x0 };
constexpr Elf32_Word SH_INFO_3_VALUE { 0x0 };
constexpr Elf32_Word SH_ADDRALIGN_3_VALUE { 0x04 };
constexpr Elf32_Word SH_ENTSIZE_3_VALUE { 0x0 };

}

namespace reader
{

using namespace ::testing;


std::string generate32BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_1_VALUE_LITTLE_ENDIAN, SH_NAME_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_1_VALUE_LITTLE_ENDIAN, SH_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_1_VALUE_LITTLE_ENDIAN, SH_ADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_1_VALUE_LITTLE_ENDIAN, SH_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_1_VALUE_LITTLE_ENDIAN, SH_SIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_1_VALUE_LITTLE_ENDIAN, SH_LINK_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_1_VALUE_LITTLE_ENDIAN, SH_INFO_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_1_VALUE_LITTLE_ENDIAN, SH_NAME_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_1_VALUE_LITTLE_ENDIAN, SH_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_1_VALUE_LITTLE_ENDIAN, SH_ADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_1_VALUE_LITTLE_ENDIAN, SH_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_1_VALUE_LITTLE_ENDIAN, SH_SIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_1_VALUE_LITTLE_ENDIAN, SH_LINK_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_1_VALUE_LITTLE_ENDIAN, SH_INFO_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitProgbitsSectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_2_VALUE_LITTLE_ENDIAN, SH_NAME_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN, SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_2_VALUE_LITTLE_ENDIAN, SH_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_2_VALUE_LITTLE_ENDIAN, SH_ADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_2_VALUE_LITTLE_ENDIAN, SH_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_2_VALUE_LITTLE_ENDIAN, SH_SIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_2_VALUE_LITTLE_ENDIAN, SH_LINK_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_2_VALUE_LITTLE_ENDIAN, SH_INFO_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitProgbitsSectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_2_VALUE_LITTLE_ENDIAN, SH_NAME_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN, SH_TYPE_PROGBITS_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_2_VALUE_LITTLE_ENDIAN, SH_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_2_VALUE_LITTLE_ENDIAN, SH_ADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_2_VALUE_LITTLE_ENDIAN, SH_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_2_VALUE_LITTLE_ENDIAN, SH_SIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_2_VALUE_LITTLE_ENDIAN, SH_LINK_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_2_VALUE_LITTLE_ENDIAN, SH_INFO_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitNotesSectionHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(SH_NAME_3_VALUE_LITTLE_ENDIAN, SH_NAME_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN, SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_FLAGS_3_VALUE_LITTLE_ENDIAN, SH_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDR_3_VALUE_LITTLE_ENDIAN, SH_ADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
              std::back_inserter(l_streamContent));
    std::copy(SH_OFFSET_3_VALUE_LITTLE_ENDIAN, SH_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
              std::back_inserter(l_streamContent));
    std::copy(SH_SIZE_3_VALUE_LITTLE_ENDIAN, SH_SIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_LINK_3_VALUE_LITTLE_ENDIAN, SH_LINK_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_INFO_3_VALUE_LITTLE_ENDIAN, SH_INFO_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitNotesSectionHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(SH_NAME_3_VALUE_LITTLE_ENDIAN, SH_NAME_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN, SH_TYPE_NOTES_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_FLAGS_3_VALUE_LITTLE_ENDIAN, SH_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDR_3_VALUE_LITTLE_ENDIAN, SH_ADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_OFFSET_3_VALUE_LITTLE_ENDIAN, SH_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_SIZE_3_VALUE_LITTLE_ENDIAN, SH_SIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_LINK_3_VALUE_LITTLE_ENDIAN, SH_LINK_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_INFO_3_VALUE_LITTLE_ENDIAN, SH_INFO_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN, SH_ADDRALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN, SH_ENTSIZE_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}


TEST(ElfHeaderReaderX32SectionHeaderTestSuite, shouldReadAll32BitLittleEndianSectionHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent() };
    l_streamContent += generate32BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate32BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
            SECTION_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            LITTLE_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetSectionHeaders.size(), THREE_ELEMENTS_SIZE);

    EXPECT_EQ(l_targetSectionHeaders[0].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[0].header32,
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));
    
    EXPECT_EQ(l_targetSectionHeaders[1].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[1].header32,
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[2].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[2].header32,
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(ElfHeaderReaderX32SectionHeaderTestSuite, shouldReadAll32BitBigEndianSectionHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent() };
    l_streamContent += generate32BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate32BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
            SECTION_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            BIG_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetSectionHeaders.size(), THREE_ELEMENTS_SIZE);

    EXPECT_EQ(l_targetSectionHeaders[0].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[0].header32,
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[1].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[1].header32,
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_EQ(l_targetSectionHeaders[2].discriminator, SectionHeaderDiscriminator::SYSTEM_VERSION_32_BIT);
    EXPECT_THAT(l_targetSectionHeaders[2].header32,
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(ElfHeaderReaderX32SectionHeaderTestSuite,
    shouldNotReadAny32BitLittleEndianSectionHeaderWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent { generate32BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent() };
    l_streamContent += generate32BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate32BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
        SECTION_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        LITTLE_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };
    
    ASSERT_EQ(l_targetSectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

TEST(ElfHeaderReaderX32SectionHeaderTestSuite,
    shouldNotReadAny32BitBigEndianSectionHeaderWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent { generate32BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent() };
    l_streamContent += generate32BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate32BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetSectionHeaders { l_headerReader.readSectionHeaders(
        SECTION_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        BIG_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };
    
    ASSERT_EQ(l_targetSectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

}

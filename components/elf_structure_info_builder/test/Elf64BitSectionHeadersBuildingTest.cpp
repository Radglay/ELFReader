#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include "ElfStructureInfoBuilder.hpp"
#include <elf.h>
#include <algorithm>
#include <cstring>


namespace
{

constexpr int ZERO_ELEMENTS_SIZE { 0 };
constexpr int THREE_ELEMENTS_SIZE { 3 };

unsigned char ZERO_ELEMENTS_SIZE_LITTLE_ENDIAN[] { 0x0, 0x0 };
unsigned char ZERO_ELEMENTS_SIZE_BIG_ENDIAN[] { 0x0, 0x0 };

unsigned char THREE_ELEMENTS_SIZE_LITTLE_ENDIAN[] { 0x3, 0x0 };
unsigned char THREE_ELEMENTS_SIZE_BIG_ENDIAN[] { 0x0, 0x3 };

unsigned char SECTION_HEADERS_OFFSET_LITTLE_ENDIAN[] { sizeof(Elf64_Ehdr), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
unsigned char SECTION_HEADERS_OFFSET_BIG_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, sizeof(Elf64_Ehdr) };

constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };


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

std::string generate64BitFileHeaderStub(unsigned char* p_sectionHeadersNum,
                                        unsigned char* p_sectionHeadersOffset)
{
    Elf64_Ehdr l_fileHeader;

    std::memcpy(&l_fileHeader.e_shnum,
                p_sectionHeadersNum,
                sizeof(Elf64_Half));

    std::memcpy(&l_fileHeader.e_shoff,
                p_sectionHeadersOffset,
                sizeof(Elf64_Off));

    char l_streamContent[sizeof(Elf64_Ehdr)];
    std::memcpy(l_streamContent, &l_fileHeader, sizeof(Elf64_Ehdr));

    return std::string{ l_streamContent, sizeof(Elf64_Ehdr) };
}

TEST(Elf64BitSectionHeadersBuildingTestSuite, shouldReadAll64BitLittleEndianSectionHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(THREE_ELEMENTS_SIZE_LITTLE_ENDIAN,
                                                   SECTION_HEADERS_OFFSET_LITTLE_ENDIAN);
    l_streamContent += generate64BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };
    ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildSectionHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_THAT(l_targetElfStructureInfo->sectionHeaders.size(), THREE_ELEMENTS_SIZE);
    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[0],
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));
    
    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[1],
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[2],
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(Elf64BitSectionHeadersBuildingTestSuite, shouldReadAll64BitBigEndianSectionHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(THREE_ELEMENTS_SIZE_BIG_ENDIAN,
                                                   SECTION_HEADERS_OFFSET_BIG_ENDIAN);
    l_streamContent += generate64BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };
    ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { l_stubStream, BIG_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildSectionHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_THAT(l_targetElfStructureInfo->sectionHeaders.size(), THREE_ELEMENTS_SIZE);
    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[0],
                FieldsAre(SH_NAME_1_VALUE, SH_TYPE_SECTION_HEADER_TABLE_ENTRY_VALUE, SH_FLAGS_1_VALUE,
                          SH_ADDR_1_VALUE, SH_OFFSET_1_VALUE, SH_SIZE_1_VALUE,
                          SH_LINK_1_VALUE, SH_INFO_1_VALUE, SH_ADDRALIGN_1_VALUE,
                          SH_ENTSIZE_1_VALUE));
    
    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[1],
                FieldsAre(SH_NAME_2_VALUE, SH_TYPE_PROGBITS_VALUE, SH_FLAGS_2_VALUE,
                          SH_ADDR_2_VALUE, SH_OFFSET_2_VALUE, SH_SIZE_2_VALUE,
                          SH_LINK_2_VALUE, SH_INFO_2_VALUE, SH_ADDRALIGN_2_VALUE,
                          SH_ENTSIZE_2_VALUE));

    EXPECT_THAT(*l_targetElfStructureInfo->sectionHeaders[2],
                FieldsAre(SH_NAME_3_VALUE, SH_TYPE_NOTES_VALUE, SH_FLAGS_3_VALUE,
                          SH_ADDR_3_VALUE, SH_OFFSET_3_VALUE, SH_SIZE_3_VALUE,
                          SH_LINK_3_VALUE, SH_INFO_3_VALUE, SH_ADDRALIGN_3_VALUE,
                          SH_ENTSIZE_3_VALUE));
}

TEST(Elf64BitSectionHeadersBuildingTestSuite,
    shouldNotReadAny64BitLittleEndianSectionHeadersWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(ZERO_ELEMENTS_SIZE_LITTLE_ENDIAN,
                                                   SECTION_HEADERS_OFFSET_LITTLE_ENDIAN);
    l_streamContent += generate64BitSectionHeaderTableEntrySectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitProgbitsSectionHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };
    ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildSectionHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };
    
    EXPECT_EQ(l_targetElfStructureInfo->sectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

TEST(Elf64BitSectionHeadersBuildingTestSuite,
    shouldNotReadAny64BitBigEndianSectionHeadersWhenSectionHeaderTableEntryCountIsEqualToZero)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(ZERO_ELEMENTS_SIZE_BIG_ENDIAN,
                                                   SECTION_HEADERS_OFFSET_BIG_ENDIAN);
    l_streamContent += generate64BitSectionHeaderTableEntrySectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitProgbitsSectionHeaderBigEndianStreamContent();
    l_streamContent += generate64BitNotesSectionHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };
    ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { l_stubStream, BIG_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildSectionHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };
    
    EXPECT_EQ(l_targetElfStructureInfo->sectionHeaders.size(), ZERO_ELEMENTS_SIZE);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include "ElfStructureInfoBuilderX64.hpp"
#include <elf.h>
#include <algorithm>
#include <cstring>
#include <arpa/inet.h>


namespace
{

constexpr int ZERO_ELEMENTS_SIZE { 0 };
constexpr int THREE_ELEMENTS_SIZE { 3 };

unsigned char ZERO_ELEMENTS_SIZE_LITTLE_ENDIAN[] { 0x0, 0x0 };
unsigned char ZERO_ELEMENTS_SIZE_BIG_ENDIAN[] { 0x0, 0x0 };

unsigned char THREE_ELEMENTS_SIZE_LITTLE_ENDIAN[] { 0x3, 0x0 };
unsigned char THREE_ELEMENTS_SIZE_BIG_ENDIAN[] { 0x0, 0x3 };

unsigned char PROGRAM_HEADERS_OFFSET_LITTLE_ENDIAN[] { sizeof(Elf64_Ehdr), 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
unsigned char PROGRAM_HEADERS_OFFSET_BIG_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, sizeof(Elf64_Ehdr) };

constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };


// first Program header
constexpr unsigned char P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN[] { 0x06, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_1_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_1_VALUE_LITTLE_ENDIAN[] { 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_VADDR_1_VALUE_LITTLE_ENDIAN[] { 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_PADDR_1_VALUE_LITTLE_ENDIAN[] { 0x40, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_1_VALUE_LITTLE_ENDIAN[] { 0xd8, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_1_VALUE_LITTLE_ENDIAN[] { 0xd8, 0x02, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_1_VALUE_LITTLE_ENDIAN[] { 0x08, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word P_TYPE_PROGRAM_HEADER_TABLE_VALUE { 0x06 };
constexpr Elf64_Word P_FLAGS_1_VALUE { 0x04 };
constexpr Elf64_Off P_OFFSET_1_VALUE { 0x40 };
constexpr Elf64_Addr P_VADDR_1_VALUE { 0x40 };
constexpr Elf64_Addr P_PADDR_1_VALUE { 0x40 };
constexpr Elf64_Xword P_FILESZ_1_VALUE { 0x02'd8 };
constexpr Elf64_Xword P_MEMSZ_1_VALUE { 0x02'd8 };
constexpr Elf64_Xword P_ALIGN_1_VALUE { 0x08 };

// second Program Header
constexpr unsigned char P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN[] { 0x03, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_2_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_2_VALUE_LITTLE_ENDIAN[] { 0x18, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_VADDR_2_VALUE_LITTLE_ENDIAN[] { 0x18, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_PADDR_2_VALUE_LITTLE_ENDIAN[] { 0x18, 0x03, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_2_VALUE_LITTLE_ENDIAN[] { 0x1c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_2_VALUE_LITTLE_ENDIAN[] { 0x1c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_2_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word P_TYPE_INTERPRETER_INFO_VALUE { 0x03 };
constexpr Elf64_Word P_FLAGS_2_VALUE { 0x04 };
constexpr Elf64_Off	P_OFFSET_2_VALUE { 0x03'18 };
constexpr Elf64_Addr P_VADDR_2_VALUE { 0x03'18 };
constexpr Elf64_Addr P_PADDR_2_VALUE { 0x03'18 };
constexpr Elf64_Xword P_FILESZ_2_VALUE { 0x1c };
constexpr Elf64_Xword P_MEMSZ_2_VALUE { 0x1c };
constexpr Elf64_Xword P_ALIGN_2_VALUE { 0x01 };

// third Program Header
constexpr unsigned char P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_3_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_VADDR_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_PADDR_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_3_VALUE_LITTLE_ENDIAN[] { 0x88, 0x0a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_3_VALUE_LITTLE_ENDIAN[] { 0x88, 0x0a, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x10, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 };

constexpr Elf64_Word P_TYPE_LOAD_SEGMENT_VALUE { 0x01 };
constexpr Elf64_Word P_FLAGS_3_VALUE { 0x04 };
constexpr Elf64_Off	P_OFFSET_3_VALUE { 0x0 };
constexpr Elf64_Addr P_VADDR_3_VALUE { 0x0 };
constexpr Elf64_Addr P_PADDR_3_VALUE { 0x0 };
constexpr Elf64_Xword P_FILESZ_3_VALUE { 0x0a'88 };
constexpr Elf64_Xword P_MEMSZ_3_VALUE { 0x0a'88 };
constexpr Elf64_Xword P_ALIGN_3_VALUE { 0x10'00 };

}


using namespace ::testing;


std::string generate64BitProgramHeaderTableProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN, P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_1_VALUE_LITTLE_ENDIAN, P_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_1_VALUE_LITTLE_ENDIAN, P_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
              std::back_inserter(l_streamContent));
    std::copy(P_VADDR_1_VALUE_LITTLE_ENDIAN, P_VADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(P_PADDR_1_VALUE_LITTLE_ENDIAN, P_PADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_1_VALUE_LITTLE_ENDIAN, P_FILESZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_1_VALUE_LITTLE_ENDIAN, P_MEMSZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_1_VALUE_LITTLE_ENDIAN, P_ALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitInterpreterInfoProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN, P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_2_VALUE_LITTLE_ENDIAN, P_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
              std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_2_VALUE_LITTLE_ENDIAN, P_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
              std::back_inserter(l_streamContent));
    std::copy(P_VADDR_2_VALUE_LITTLE_ENDIAN, P_VADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(P_PADDR_2_VALUE_LITTLE_ENDIAN, P_PADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
              std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_2_VALUE_LITTLE_ENDIAN, P_FILESZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_2_VALUE_LITTLE_ENDIAN, P_MEMSZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_2_VALUE_LITTLE_ENDIAN, P_ALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
              std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitLoadableSegmentProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN, P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_3_VALUE_LITTLE_ENDIAN, P_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_3_VALUE_LITTLE_ENDIAN, P_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::copy(P_VADDR_3_VALUE_LITTLE_ENDIAN, P_VADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_PADDR_3_VALUE_LITTLE_ENDIAN, P_PADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_3_VALUE_LITTLE_ENDIAN, P_FILESZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_3_VALUE_LITTLE_ENDIAN, P_MEMSZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_3_VALUE_LITTLE_ENDIAN, P_ALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitProgramHeaderTableProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN, P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_1_VALUE_LITTLE_ENDIAN, P_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_1_VALUE_LITTLE_ENDIAN, P_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_1_VALUE_LITTLE_ENDIAN, P_VADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_1_VALUE_LITTLE_ENDIAN, P_PADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_1_VALUE_LITTLE_ENDIAN, P_FILESZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_1_VALUE_LITTLE_ENDIAN, P_MEMSZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_1_VALUE_LITTLE_ENDIAN, P_ALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitInterpreterInfoProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN, P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_2_VALUE_LITTLE_ENDIAN, P_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_2_VALUE_LITTLE_ENDIAN, P_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_2_VALUE_LITTLE_ENDIAN, P_VADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_2_VALUE_LITTLE_ENDIAN, P_PADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_2_VALUE_LITTLE_ENDIAN, P_FILESZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_2_VALUE_LITTLE_ENDIAN, P_MEMSZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_2_VALUE_LITTLE_ENDIAN, P_ALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitLoadableSegmentProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN, P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_3_VALUE_LITTLE_ENDIAN, P_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_3_VALUE_LITTLE_ENDIAN, P_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_3_VALUE_LITTLE_ENDIAN, P_VADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_3_VALUE_LITTLE_ENDIAN, P_PADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_3_VALUE_LITTLE_ENDIAN, P_FILESZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_3_VALUE_LITTLE_ENDIAN, P_MEMSZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_3_VALUE_LITTLE_ENDIAN, P_ALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf64_Xword),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate64BitFileHeaderStub(unsigned char* p_programHeadersNum,
                                        unsigned char* p_programHeadersOffset)
{
    Elf64_Ehdr l_fileHeader;

    std::memcpy(&l_fileHeader.e_phnum,
                p_programHeadersNum,
                sizeof(Elf64_Half));

    std::memcpy(&l_fileHeader.e_phoff,
                p_programHeadersOffset,
                sizeof(Elf64_Off));

    char l_streamContent[sizeof(Elf64_Ehdr)];
    std::memcpy(l_streamContent, &l_fileHeader, sizeof(Elf64_Ehdr));

    return std::string{ l_streamContent, sizeof(Elf64_Ehdr) };
}


TEST(Elf64BitProgramHeadersBuildingTestSuite, shouldReadAll64BitLittleEndianProgramHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(THREE_ELEMENTS_SIZE_LITTLE_ENDIAN,
                                                   PROGRAM_HEADERS_OFFSET_LITTLE_ENDIAN);
    l_streamContent += generate64BitProgramHeaderTableProgramHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitInterpreterInfoProgramHeaderLittleEndianStreamContent();
    l_streamContent += generate64BitLoadableSegmentProgramHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX64 l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildProgramHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };
 
    EXPECT_EQ(l_targetElfStructureInfo->programHeaders.size(), THREE_ELEMENTS_SIZE);

    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[0],
                FieldsAre(P_TYPE_PROGRAM_HEADER_TABLE_VALUE, P_FLAGS_1_VALUE, P_OFFSET_1_VALUE,
                          P_VADDR_1_VALUE, P_PADDR_1_VALUE, P_FILESZ_1_VALUE,
                          P_MEMSZ_1_VALUE, P_ALIGN_1_VALUE));

    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[1],
            FieldsAre(P_TYPE_INTERPRETER_INFO_VALUE, P_FLAGS_2_VALUE, P_OFFSET_2_VALUE,
                      P_VADDR_2_VALUE, P_PADDR_2_VALUE, P_FILESZ_2_VALUE,
                      P_MEMSZ_2_VALUE, P_ALIGN_2_VALUE));

    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[2],
            FieldsAre(P_TYPE_LOAD_SEGMENT_VALUE, P_FLAGS_3_VALUE, P_OFFSET_3_VALUE,
                      P_VADDR_3_VALUE,P_PADDR_3_VALUE, P_FILESZ_3_VALUE,
                      P_MEMSZ_3_VALUE, P_ALIGN_3_VALUE));
}

TEST(Elf64BitProgramHeadersBuildingTestSuite, shouldReadAll64BitBigEndianProgramHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(THREE_ELEMENTS_SIZE_BIG_ENDIAN,
                                                   PROGRAM_HEADERS_OFFSET_BIG_ENDIAN);
    l_streamContent += generate64BitProgramHeaderTableProgramHeaderBigEndianStreamContent();
    l_streamContent += generate64BitInterpreterInfoProgramHeaderBigEndianStreamContent();
    l_streamContent += generate64BitLoadableSegmentProgramHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX64 l_elfStructureInfoBuilder { l_stubStream, BIG_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildProgramHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_EQ(l_targetElfStructureInfo->programHeaders.size(), THREE_ELEMENTS_SIZE);
    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[0],
                FieldsAre(P_TYPE_PROGRAM_HEADER_TABLE_VALUE, P_FLAGS_1_VALUE, P_OFFSET_1_VALUE,
                          P_VADDR_1_VALUE, P_PADDR_1_VALUE, P_FILESZ_1_VALUE,
                          P_MEMSZ_1_VALUE, P_ALIGN_1_VALUE));

    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[1],
            FieldsAre(P_TYPE_INTERPRETER_INFO_VALUE, P_FLAGS_2_VALUE, P_OFFSET_2_VALUE,
                      P_VADDR_2_VALUE, P_PADDR_2_VALUE, P_FILESZ_2_VALUE,
                      P_MEMSZ_2_VALUE, P_ALIGN_2_VALUE));

    EXPECT_THAT(l_targetElfStructureInfo->programHeaders[2],
            FieldsAre(P_TYPE_LOAD_SEGMENT_VALUE, P_FLAGS_3_VALUE, P_OFFSET_3_VALUE,
                      P_VADDR_3_VALUE,P_PADDR_3_VALUE, P_FILESZ_3_VALUE,
                      P_MEMSZ_3_VALUE, P_ALIGN_3_VALUE));
}

TEST(Elf64BitProgramHeadersBuildingTestSuite, shouldReadZero64BitLittleEndianProgramHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(ZERO_ELEMENTS_SIZE_LITTLE_ENDIAN,
                                                   PROGRAM_HEADERS_OFFSET_LITTLE_ENDIAN);
    l_streamContent += generate64BitProgramHeaderTableProgramHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX64 l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildProgramHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_EQ(l_targetElfStructureInfo->programHeaders.size(), ZERO_ELEMENTS_SIZE);
}

TEST(Elf64BitProgramHeadersBuildingTestSuite, shouldReadZero64BitBigEndianProgramHeaders)
{
    std::string l_streamContent;
    l_streamContent += generate64BitFileHeaderStub(ZERO_ELEMENTS_SIZE_BIG_ENDIAN,
                                                   PROGRAM_HEADERS_OFFSET_BIG_ENDIAN);
    l_streamContent += generate64BitProgramHeaderTableProgramHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX64 l_elfStructureInfoBuilder { l_stubStream, BIG_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    l_elfStructureInfoBuilder.buildProgramHeaders();
    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_EQ(l_targetElfStructureInfo->programHeaders.size(), ZERO_ELEMENTS_SIZE);
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include <string>
#include "ElfHeaderReaderX32.hpp"
#include "ElfHeaderReaderX64.hpp"
#include "ProgramHeader.hpp"
#include <elf.h>


namespace
{

constexpr int ZERO_ELEMENTS_SIZE { 0 };
constexpr int THREE_ELEMENTS_SIZE { 3 };

constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };

constexpr Elf32_Off PROGRAM_HEADER_TABLE_OFFSET { 0x0 };

// first Program header
constexpr unsigned char P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN[] { 0x06, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_1_VALUE_LITTLE_ENDIAN[] { 0x34, 0x0, 0x0, 0x0 };
constexpr unsigned char P_VADDR_1_VALUE_LITTLE_ENDIAN[] { 0x34, 0x0, 0x0, 0x0 };
constexpr unsigned char P_PADDR_1_VALUE_LITTLE_ENDIAN[] { 0x34, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_1_VALUE_LITTLE_ENDIAN[] { 0x60, 0x1, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_1_VALUE_LITTLE_ENDIAN[] { 0x60, 0x1, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_1_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_1_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };

constexpr Elf32_Word P_TYPE_PROGRAM_HEADER_TABLE_VALUE { 0x06 };
constexpr Elf32_Off	P_OFFSET_1_VALUE { 0x34 } ;
constexpr Elf32_Addr P_VADDR_1_VALUE { 0x34 };
constexpr Elf32_Addr P_PADDR_1_VALUE { 0x34 };
constexpr Elf32_Word P_FILESZ_1_VALUE { 0x01'60 };
constexpr Elf32_Word P_MEMSZ_1_VALUE { 0x01'60 };
constexpr Elf32_Word P_FLAGS_1_VALUE { 0x04 };
constexpr Elf32_Word P_ALIGN_1_VALUE { 0x04 };

// second Program Header
constexpr unsigned char P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN[] { 0x03, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_2_VALUE_LITTLE_ENDIAN[] { 0x94, 0x01, 0x0, 0x0 };
constexpr unsigned char P_VADDR_2_VALUE_LITTLE_ENDIAN[] { 0x94, 0x01, 0x0, 0x0 };
constexpr unsigned char P_PADDR_2_VALUE_LITTLE_ENDIAN[] { 0x94, 0x01, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_2_VALUE_LITTLE_ENDIAN[] { 0x13, 0x0, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_2_VALUE_LITTLE_ENDIAN[] { 0x13, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_2_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_2_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };

constexpr Elf32_Word P_TYPE_INTERPRETER_INFO_VALUE { 0x03 };
constexpr Elf32_Off	P_OFFSET_2_VALUE { 0x01'94 } ;
constexpr Elf32_Addr P_VADDR_2_VALUE { 0x01'94 };
constexpr Elf32_Addr P_PADDR_2_VALUE { 0x01'94 };
constexpr Elf32_Word P_FILESZ_2_VALUE { 0x13 };
constexpr Elf32_Word P_MEMSZ_2_VALUE { 0x13 };
constexpr Elf32_Word P_FLAGS_2_VALUE { 0x04 };
constexpr Elf32_Word P_ALIGN_2_VALUE { 0x01 };

// third Program Header
constexpr unsigned char P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN[] { 0x01, 0x0, 0x0, 0x0 };
constexpr unsigned char P_OFFSET_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_VADDR_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_PADDR_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char P_FILESZ_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x05, 0x0, 0x0 };
constexpr unsigned char P_MEMSZ_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x05, 0x0, 0x0 };
constexpr unsigned char P_FLAGS_3_VALUE_LITTLE_ENDIAN[] { 0x04, 0x0, 0x0, 0x0 };
constexpr unsigned char P_ALIGN_3_VALUE_LITTLE_ENDIAN[] { 0x0, 0x01, 0x0, 0x0 };

constexpr Elf32_Word P_TYPE_LOAD_SEGMENT_VALUE { 0x01 };
constexpr Elf32_Off	P_OFFSET_3_VALUE { 0x0 } ;
constexpr Elf32_Addr P_VADDR_3_VALUE { 0x0 };
constexpr Elf32_Addr P_PADDR_3_VALUE { 0x0 };
constexpr Elf32_Word P_FILESZ_3_VALUE { 0x05'00 };
constexpr Elf32_Word P_MEMSZ_3_VALUE { 0x05'00 };
constexpr Elf32_Word P_FLAGS_3_VALUE { 0x04 };
constexpr Elf32_Word P_ALIGN_3_VALUE { 0x01'00 };

}

namespace reader
{

using namespace ::testing;


std::string generate32BitProgramHeaderTableProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN, P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
              std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_1_VALUE_LITTLE_ENDIAN, P_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::copy(P_VADDR_1_VALUE_LITTLE_ENDIAN, P_VADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_PADDR_1_VALUE_LITTLE_ENDIAN, P_PADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_1_VALUE_LITTLE_ENDIAN, P_FILESZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_1_VALUE_LITTLE_ENDIAN, P_MEMSZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_1_VALUE_LITTLE_ENDIAN, P_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_1_VALUE_LITTLE_ENDIAN, P_ALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitInterpreterInfoProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN, P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_2_VALUE_LITTLE_ENDIAN, P_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::copy(P_VADDR_2_VALUE_LITTLE_ENDIAN, P_VADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_PADDR_2_VALUE_LITTLE_ENDIAN, P_PADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_2_VALUE_LITTLE_ENDIAN, P_FILESZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_2_VALUE_LITTLE_ENDIAN, P_MEMSZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_2_VALUE_LITTLE_ENDIAN, P_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_2_VALUE_LITTLE_ENDIAN, P_ALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitLoadableSegmentProgramHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;

    std::copy(P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN, P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_OFFSET_3_VALUE_LITTLE_ENDIAN, P_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::copy(P_VADDR_3_VALUE_LITTLE_ENDIAN, P_VADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_PADDR_3_VALUE_LITTLE_ENDIAN, P_PADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::copy(P_FILESZ_3_VALUE_LITTLE_ENDIAN, P_FILESZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_MEMSZ_3_VALUE_LITTLE_ENDIAN, P_MEMSZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_FLAGS_3_VALUE_LITTLE_ENDIAN, P_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::copy(P_ALIGN_3_VALUE_LITTLE_ENDIAN, P_ALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitProgramHeaderTableProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN, P_TYPE_PROGRAM_HEADER_TABLE_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_1_VALUE_LITTLE_ENDIAN, P_OFFSET_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_1_VALUE_LITTLE_ENDIAN, P_VADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_1_VALUE_LITTLE_ENDIAN, P_PADDR_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_1_VALUE_LITTLE_ENDIAN, P_FILESZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_1_VALUE_LITTLE_ENDIAN, P_MEMSZ_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_1_VALUE_LITTLE_ENDIAN, P_FLAGS_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_1_VALUE_LITTLE_ENDIAN, P_ALIGN_1_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitInterpreterInfoProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN, P_TYPE_INTERPRETER_INFO_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_2_VALUE_LITTLE_ENDIAN, P_OFFSET_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_2_VALUE_LITTLE_ENDIAN, P_VADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_2_VALUE_LITTLE_ENDIAN, P_PADDR_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_2_VALUE_LITTLE_ENDIAN, P_FILESZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_2_VALUE_LITTLE_ENDIAN, P_MEMSZ_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_2_VALUE_LITTLE_ENDIAN, P_FLAGS_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_2_VALUE_LITTLE_ENDIAN, P_ALIGN_2_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitLoadableSegmentProgramHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;

    std::reverse_copy(P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN, P_TYPE_LOADABLE_SEGMENT_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_OFFSET_3_VALUE_LITTLE_ENDIAN, P_OFFSET_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_VADDR_3_VALUE_LITTLE_ENDIAN, P_VADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_PADDR_3_VALUE_LITTLE_ENDIAN, P_PADDR_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FILESZ_3_VALUE_LITTLE_ENDIAN, P_FILESZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_MEMSZ_3_VALUE_LITTLE_ENDIAN, P_MEMSZ_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_FLAGS_3_VALUE_LITTLE_ENDIAN, P_FLAGS_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(P_ALIGN_3_VALUE_LITTLE_ENDIAN, P_ALIGN_3_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}


TEST(ElfHeaderReaderX32ProgramHeaderTestSuite, shouldReadAll32BitLittleEndianProgramHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitProgramHeaderTableProgramHeaderLittleEndianStreamContent() };
    l_streamContent += generate32BitInterpreterInfoProgramHeaderLittleEndianStreamContent();
    l_streamContent += generate32BitLoadableSegmentProgramHeaderLittleEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetProgramHeaders { l_headerReader.readProgramHeaders(
            PROGRAM_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            LITTLE_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetProgramHeaders.size(), THREE_ELEMENTS_SIZE);
    EXPECT_EQ(l_targetProgramHeaders[0].discriminator, ProgramHeaderDiscriminator::SYSTEM_VERSION_32_BIT);

    EXPECT_THAT(l_targetProgramHeaders[0].header32,
                FieldsAre(P_TYPE_PROGRAM_HEADER_TABLE_VALUE, P_OFFSET_1_VALUE, P_VADDR_1_VALUE,
                          P_PADDR_1_VALUE, P_FILESZ_1_VALUE, P_MEMSZ_1_VALUE,
                          P_FLAGS_1_VALUE, P_ALIGN_1_VALUE));

    EXPECT_THAT(l_targetProgramHeaders[1].header32,
            FieldsAre(P_TYPE_INTERPRETER_INFO_VALUE, P_OFFSET_2_VALUE, P_VADDR_2_VALUE,
                        P_PADDR_2_VALUE, P_FILESZ_2_VALUE, P_MEMSZ_2_VALUE,
                        P_FLAGS_2_VALUE, P_ALIGN_2_VALUE));

    EXPECT_THAT(l_targetProgramHeaders[2].header32,
            FieldsAre(P_TYPE_LOAD_SEGMENT_VALUE, P_OFFSET_3_VALUE, P_VADDR_3_VALUE,
                        P_PADDR_3_VALUE, P_FILESZ_3_VALUE, P_MEMSZ_3_VALUE,
                        P_FLAGS_3_VALUE, P_ALIGN_3_VALUE));
}

TEST(ElfHeaderReaderX32ProgramHeaderTestSuite, shouldReadAll32BitBigEndianProgramHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitProgramHeaderTableProgramHeaderBigEndianStreamContent() };
    l_streamContent += generate32BitInterpreterInfoProgramHeaderBigEndianStreamContent();
    l_streamContent += generate32BitLoadableSegmentProgramHeaderBigEndianStreamContent();

    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };
    auto l_targetProgramHeaders { l_headerReader.readProgramHeaders(
            PROGRAM_HEADER_TABLE_OFFSET,
            THREE_ELEMENTS_SIZE,
            BIG_ENDIAN_VALUE,
            LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetProgramHeaders.size(), THREE_ELEMENTS_SIZE);
    EXPECT_EQ(l_targetProgramHeaders[0].discriminator, ProgramHeaderDiscriminator::SYSTEM_VERSION_32_BIT);

    EXPECT_THAT(l_targetProgramHeaders[0].header32,
                FieldsAre(P_TYPE_PROGRAM_HEADER_TABLE_VALUE, P_OFFSET_1_VALUE, P_VADDR_1_VALUE,
                          P_PADDR_1_VALUE, P_FILESZ_1_VALUE, P_MEMSZ_1_VALUE,
                          P_FLAGS_1_VALUE, P_ALIGN_1_VALUE));

    EXPECT_THAT(l_targetProgramHeaders[1].header32,
            FieldsAre(P_TYPE_INTERPRETER_INFO_VALUE, P_OFFSET_2_VALUE, P_VADDR_2_VALUE,
                        P_PADDR_2_VALUE, P_FILESZ_2_VALUE, P_MEMSZ_2_VALUE,
                        P_FLAGS_2_VALUE, P_ALIGN_2_VALUE));

    EXPECT_THAT(l_targetProgramHeaders[2].header32,
            FieldsAre(P_TYPE_LOAD_SEGMENT_VALUE, P_OFFSET_3_VALUE, P_VADDR_3_VALUE,
                        P_PADDR_3_VALUE, P_FILESZ_3_VALUE, P_MEMSZ_3_VALUE,
                        P_FLAGS_3_VALUE, P_ALIGN_3_VALUE));
}

TEST(ElfHeaderReaderX32ProgramHeaderTestSuite, shouldReadZero32BitLittleEndianProgramHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitProgramHeaderTableProgramHeaderLittleEndianStreamContent() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetProgramHeaders { l_headerReader.readProgramHeaders(
        PROGRAM_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        LITTLE_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetProgramHeaders.size(), ZERO_ELEMENTS_SIZE);
}

TEST(ElfHeaderReaderX32ProgramHeaderTestSuite, shouldReadZero32BitBigEndianProgramHeadersOnHostLittleEndian)
{
    std::string l_streamContent { generate32BitProgramHeaderTableProgramHeaderBigEndianStreamContent() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfHeaderReaderX32 l_headerReader { l_stubStream };

    auto l_targetProgramHeaders { l_headerReader.readProgramHeaders(
        PROGRAM_HEADER_TABLE_OFFSET,
        ZERO_ELEMENTS_SIZE,
        BIG_ENDIAN_VALUE,
        LITTLE_ENDIAN_VALUE) };

    ASSERT_EQ(l_targetProgramHeaders.size(), ZERO_ELEMENTS_SIZE);
}

}

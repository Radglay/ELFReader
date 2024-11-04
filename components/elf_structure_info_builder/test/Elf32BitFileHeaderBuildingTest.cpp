#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <sstream>
#include "ElfStructureInfoBuilderX32.hpp"
#include <elf.h>
#include <algorithm>


namespace
{

constexpr unsigned char MAGIC_NUMBERS_VALUE[] { 0x7f, 0x45, 0x4c, 0x46 };
constexpr unsigned char BIT_VERSION_32_VALUE { 0x1 };
constexpr unsigned char LITTLE_ENDIAN_VALUE { 0x1 };
constexpr unsigned char BIG_ENDIAN_VALUE { 0x2 };
constexpr unsigned char WRONG_ENDIAN_VALUE { 0x3 };
constexpr unsigned char ORIGINAL_ELF_FILE_VERSION_VALUE { 0x1 };
constexpr unsigned char SYSTEM_V_ABI_VALUE { 0x0 };
constexpr unsigned char ABI_VERSION_VALUE { 0x0 };
constexpr unsigned char RESERVED_PADDING_BYTES_VALUE[] { 0, 0, 0, 0, 0, 0, 0 };

constexpr unsigned char E_TYPE_EXEC_VALUE_LITTLE_ENDIAN[] { 0x02, 0x0 };
constexpr unsigned char E_MACHINE_X86_VALUE_LITTLE_ENDIAN[] { 0x03, 0x0 };
constexpr unsigned char E_VERSION_ORIGINAL_VALUE_LITTLE_ENDIAN[] { 0x1, 0x0, 0x0, 0x0 };
constexpr unsigned char E_ENTRY_VALUE_LITTLE_ENDIAN[] { 0x10, 0xa0, 0x0, 0x0 };
constexpr unsigned char E_PHOFF_VALUE_LITTLE_ENDIAN[] { 0x34, 0x0, 0x0, 0x0 };
constexpr unsigned char E_SHOFF_VALUE_LITTLE_ENDIAN[] { 0x64, 0x37, 0x0, 0x0 };
constexpr unsigned char E_FLAGS_VALUE_LITTLE_ENDIAN[] { 0x0, 0x0, 0x0, 0x0 };
constexpr unsigned char E_EHSIZE_LITTLE_ENDIAN[] { 0x34, 0x0 };
constexpr unsigned char E_PHENTSIZE_VALUE_LITTLE_ENDIAN[] { 0x20, 0x0 };
constexpr unsigned char E_PHNUM_VALUE_LITTLE_ENDIAN[] { 0x0b, 0x0 };
constexpr unsigned char E_SHENTSIZE_LITTLE_ENDIAN[] { 0x28, 0x0 };
constexpr unsigned char E_SHNUM_LITTLE_ENDIAN[] { 0x1d, 0x0 };
constexpr unsigned char E_SHSTRNDX_LITTLE_ENDIAN[] { 0x1c, 0x0 };

constexpr Elf32_Half E_TYPE_EXEC_VALUE { 0x02 };
constexpr Elf32_Half E_MACHINE_X86_VALUE { 0x03 };
constexpr Elf32_Word E_VERSION_ORIGINAL_VALUE { 0x1 };
constexpr Elf32_Addr E_ENTRY_VALUE { 0xa0'10 };
constexpr Elf32_Off	E_PHOFF_VALUE { 0x34 };
constexpr Elf32_Off	E_SHOFF_VALUE { 0x37'64 };
constexpr Elf32_Word E_FLAGS_VALUE { 0x0 };
constexpr Elf32_Half E_EHSIZE { 0x34 };
constexpr Elf32_Half E_PHENTSIZE_VALUE { 0x20 };
constexpr Elf32_Half E_PHNUM_VALUE { 0x0b };
constexpr Elf32_Half E_SHENTSIZE_VALUE { 0x28 };
constexpr Elf32_Half E_SHNUM_VALUE { 0x1d };
constexpr Elf32_Half E_SHSTRNDX_VALUE { 0x1c };

}

using namespace ::testing;


std::string generateFileHeaderIdentPartWithSpecificEndianness(unsigned char p_endianness)
{
    std::string l_identPart;
    std::copy_n(MAGIC_NUMBERS_VALUE, sizeof(MAGIC_NUMBERS_VALUE), std::back_inserter(l_identPart));
    l_identPart += static_cast<char>(BIT_VERSION_32_VALUE);
    l_identPart += static_cast<char>(p_endianness);
    l_identPart += static_cast<char>(ORIGINAL_ELF_FILE_VERSION_VALUE);
    l_identPart += static_cast<char>(SYSTEM_V_ABI_VALUE);
    l_identPart += static_cast<char>(ABI_VERSION_VALUE);
    std::copy_n(RESERVED_PADDING_BYTES_VALUE, sizeof(RESERVED_PADDING_BYTES_VALUE), std::back_inserter(l_identPart));

    return l_identPart;
}

std::string generate32BitFileHeaderLittleEndianStreamContent()
{   
    std::string l_streamContent;
    l_streamContent += generateFileHeaderIdentPartWithSpecificEndianness(LITTLE_ENDIAN_VALUE);

    std::copy_n(E_TYPE_EXEC_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_MACHINE_X86_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_VERSION_ORIGINAL_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Word), std::back_inserter(l_streamContent));
    std::copy_n(E_ENTRY_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Addr), std::back_inserter(l_streamContent));
    std::copy_n(E_PHOFF_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Off), std::back_inserter(l_streamContent));
    std::copy_n(E_SHOFF_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Off), std::back_inserter(l_streamContent));
    std::copy_n(E_FLAGS_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Word), std::back_inserter(l_streamContent));
    std::copy_n(E_EHSIZE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_PHENTSIZE_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_PHNUM_VALUE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_SHENTSIZE_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_SHNUM_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));
    std::copy_n(E_SHSTRNDX_LITTLE_ENDIAN, sizeof(Elf32_Half), std::back_inserter(l_streamContent));

    return l_streamContent;
}

std::string generate32BitFileHeaderBigEndianStreamContent()
{   
    std::string l_streamContent;
    l_streamContent += generateFileHeaderIdentPartWithSpecificEndianness(BIG_ENDIAN_VALUE);

    std::reverse_copy(E_TYPE_EXEC_VALUE_LITTLE_ENDIAN, E_TYPE_EXEC_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_MACHINE_X86_VALUE_LITTLE_ENDIAN, E_MACHINE_X86_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_VERSION_ORIGINAL_VALUE_LITTLE_ENDIAN, E_VERSION_ORIGINAL_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_ENTRY_VALUE_LITTLE_ENDIAN, E_ENTRY_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Addr),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_PHOFF_VALUE_LITTLE_ENDIAN, E_PHOFF_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_SHOFF_VALUE_LITTLE_ENDIAN, E_SHOFF_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Off),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_FLAGS_VALUE_LITTLE_ENDIAN, E_FLAGS_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Word),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_EHSIZE_LITTLE_ENDIAN, E_EHSIZE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_PHENTSIZE_VALUE_LITTLE_ENDIAN, E_PHENTSIZE_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_PHNUM_VALUE_LITTLE_ENDIAN, E_PHNUM_VALUE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_SHENTSIZE_LITTLE_ENDIAN, E_SHENTSIZE_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_SHNUM_LITTLE_ENDIAN, E_SHNUM_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));
    std::reverse_copy(E_SHSTRNDX_LITTLE_ENDIAN, E_SHSTRNDX_LITTLE_ENDIAN + sizeof(Elf32_Half),
                      std::back_inserter(l_streamContent));

    return l_streamContent;
}


TEST(Elf32BitFileHeaderBuildingTestSuite, shouldBuild32BitLittleEndianFileHeader)
{
    std::string l_streamContent { generate32BitFileHeaderLittleEndianStreamContent() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX32 l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    std::string l_expectedIdentPart { generateFileHeaderIdentPartWithSpecificEndianness(LITTLE_ENDIAN_VALUE) };
    EXPECT_THAT(l_targetElfStructureInfo->fileHeader,
                FieldsAre(ElementsAreArray(l_expectedIdentPart.c_str(), l_expectedIdentPart.c_str() + 16),
                          E_TYPE_EXEC_VALUE, E_MACHINE_X86_VALUE, E_VERSION_ORIGINAL_VALUE,
                          E_ENTRY_VALUE, E_PHOFF_VALUE, E_SHOFF_VALUE,
                          E_FLAGS_VALUE, E_EHSIZE, E_PHENTSIZE_VALUE,
                          E_PHNUM_VALUE, E_SHENTSIZE_VALUE, E_SHNUM_VALUE, E_SHSTRNDX_VALUE));
}

TEST(Elf32BitFileHeaderBuildingTestSuite, shouldBuild32BitBigEndianFileHeader)
{
    std::string l_streamContent { generate32BitFileHeaderBigEndianStreamContent() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX32 l_elfStructureInfoBuilder { l_stubStream, BIG_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    auto l_targetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    std::string l_expectedIdentPart { generateFileHeaderIdentPartWithSpecificEndianness(BIG_ENDIAN_VALUE) };
    EXPECT_THAT(l_targetElfStructureInfo->fileHeader,
                FieldsAre(ElementsAreArray(l_expectedIdentPart.c_str(), l_expectedIdentPart.c_str() + 16),
                          E_TYPE_EXEC_VALUE, E_MACHINE_X86_VALUE, E_VERSION_ORIGINAL_VALUE,
                          E_ENTRY_VALUE, E_PHOFF_VALUE, E_SHOFF_VALUE,
                          E_FLAGS_VALUE, E_EHSIZE, E_PHENTSIZE_VALUE,
                          E_PHNUM_VALUE, E_SHENTSIZE_VALUE, E_SHNUM_VALUE, E_SHSTRNDX_VALUE));
}


TEST(Elf32BitFileHeaderBuildingTestSuite, shouldResetElfStructureInfoAfterGettingResult)
{
    std::string l_streamContent { generate32BitFileHeaderLittleEndianStreamContent() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfStructureInfoBuilderX32 l_elfStructureInfoBuilder { l_stubStream, LITTLE_ENDIAN_VALUE };
    l_elfStructureInfoBuilder.buildFileHeader();

    auto l_firstTargetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };
    auto l_secondTargetElfStructureInfo { l_elfStructureInfoBuilder.getResult() };

    EXPECT_THAT(l_secondTargetElfStructureInfo, NotNull());
    EXPECT_THAT(l_firstTargetElfStructureInfo, Not(Pointer(l_secondTargetElfStructureInfo)));
}

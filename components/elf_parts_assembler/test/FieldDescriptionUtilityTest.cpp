#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "FieldDescriptionUtility.hpp"
#include <QString>
#include <tuple>


namespace
{
    constexpr int BIT_VERSION_32_VALUE { 0x1 };
    const QString BIT_VERSION_32_HIGH_LEVEL_VALUE { "32 Bit Version" };

    constexpr int BIT_VERSION_64_VALUE { 0x2 };
    const QString BIT_VERSION_64_HIGH_LEVEL_VALUE { "64 Bit Version" };

    constexpr int LITTLE_ENDIAN_VALUE { 0x1 };
    const QString LITTLE_ENDIAN_HIGH_LEVEL_VALUE { "Little Endian" };

    constexpr int BIG_ENDIAN_VALUE { 0x2 };
    const QString BIG_ENDIAN_HIGH_LEVEL_VALUE { "Big Endian" };

    constexpr int ELF_VERSION_VALUE { 0x1 };
    const QString ELF_VERSION_HIGH_LEVEL_VALUE { "Original ELF Version" };

    constexpr int SYSV_ABI_VALUE {};
    const QString SYSV_ABI_HIGH_LEVEL_VALUE { "UNIX System V ABI" };

    constexpr int ABI_VERSION_VALUE { 0x1 };
    const QString ABI_VERSION_HIGH_LEVEL_VALUE { "1" };

    constexpr int RELOCATION_FILE_VALUE { 0x1 };
    const QString RELOCATION_FILE_HIGH_LEVEL_VALUE { "Relocatable file" };
    
    constexpr int EXECUTABLE_FILE_VALUE { 0x2 };
    const QString EXECUTABLE_FILE_HIGH_LEVEL_VALUE { "Executable file" };

    constexpr int SHARED_OBJECT_FILE_VALUE { 0x3 };
    const QString SHARED_OBJECT_FILE_HIGH_LEVEL_VALUE { "Shared object file" };

    constexpr int CORE_FILE_VALUE { 0x4 };
    const QString CORE_FILE_HIGH_LEVEL_VALUE { "Core file" };

    constexpr int AMD_X86_64_ARCHITECTURE_VALUE { 0x3E };
    const QString AMD_X86_64_HIGH_LEVEL_VALUE { "AMD x86-64 architecture" };


    constexpr int NUMBER_10 { 10 };
    constexpr int NUMBER_0 {};

    const QString DECIMAL_10_TEXT_VALUE { "10" };
    const QString DECIMAL_0_TEXT_VALUE { "0" };

    const QString HEX_10_TEXT_VALUE { "0xA" };
    const QString HEX_0_TEXT_VALUE { "0x0" };

    constexpr int BINARY_LENGTH_4 { 4 };
    constexpr int BINARY_LENGTH_8 { 8 };
    const QString BINARY_10_WITH_8_BITS_LENGTH_TEXT_VALUE { "0b00001010" };
    const QString BINARY_10_WITH_4_BITS_LENGTH_TEXT_VALUE { "0b1010" };
    const QString BINARY_0_WITH_8_BITS_LENGTH_TEXT_VALUE { "0b00000000" };
    const QString BINARY_0_WITH_4_BITS_LENGTH_TEXT_VALUE { "0b0000" };


    
    constexpr int UNUSED_SECTION_HEADER_TABLE_TYPE_VALUE {};
    constexpr int PROGBITS_TYPE_VALUE { 1 };
    constexpr int SYMTAB_TYPE_VALUE { 2 };
    constexpr int STRTAB_TYPE_VALUE { 3 };
    constexpr int RELA_TYPE_VALUE { 4 };
    constexpr int HASH_TYPE_VALUE { 5 };
    constexpr int DYNAMIC_TYPE_VALUE { 6 };
    constexpr int NOTE_TYPE_VALUE { 7 };
    constexpr int NOBITS_TYPE_VALUE { 8 };
    constexpr int REL_TYPE_VALUE { 9 };
    constexpr int SHLIB_TYPE_VALUE { 10 };
    constexpr int DYNSYM_TYPE_VALUE { 11 };
    constexpr int INIT_ARRAY_TYPE_VALUE { 14 };
    constexpr int FINI_ARRAY_TYPE_VALUE { 15 };
    constexpr int PREINIT_ARRAY_TYPE_VALUE { 16 };
    constexpr int GROUP_TYPE_VALUE { 17 };

    constexpr int SECTION_TYPE_WITHOUT_LINK_OR_INFO_VALUE { 99999 };


    const QString UNUSED_SECTION_HEADER_TABLE_TYPE_TEXT_VALUE { "Section header table entry unused" };
    const QString PROGBITS_TYPE_TEXT_VALUE { "Program data" };
    const QString SYMTAB_TYPE_TEXT_VALUE { "Symbol table" };
    const QString STRTAB_TYPE_TEXT_VALUE { "String table" };
    const QString RELA_TYPE_TEXT_VALUE { "Relocation entries with addends" };
    const QString HASH_TYPE_TEXT_VALUE { "Symbol hash table" };
    const QString DYNAMIC_TYPE_TEXT_VALUE { "Dynamic linking information" };
    const QString NOTE_TYPE_TEXT_VALUE { "Notes" };
    const QString NOBITS_TYPE_TEXT_VALUE { "Program space with no data (bss)" };
    const QString REL_TYPE_TEXT_VALUE { "Relocation entries, no addends" };
    const QString SHLIB_TYPE_TEXT_VALUE { "Reserved" };
    const QString DYNSYM_TYPE_TEXT_VALUE { "Dynamic linker symbol table" };
    const QString INIT_ARRAY_TYPE_TEXT_VALUE { "Array of constructors" };
    const QString FINI_ARRAY_TYPE_TEXT_VALUE { "Array of destructors" };
    const QString PREINIT_ARRAY_TYPE_TEXT_VALUE { "Array of pre-constructors" };
    const QString GROUP_TYPE_TEXT_VALUE { "Section group" };


    constexpr uint32_t DYNAMIC_SH_TYPE_VALUE { 6u };
    constexpr uint32_t HASH_SH_TYPE_VALUE { 5u };
    constexpr uint32_t REL_SH_TYPE_VALUE { 9u };
    constexpr uint32_t RELA_SH_TYPE_VALUE { 4u };
    constexpr uint32_t SYMTAB_SH_TYPE_VALUE { 2u };
    constexpr uint32_t DYNAMIC_SYMTAB_SH_TYPE_VALUE { 11u };
    constexpr uint32_t GROUP_SH_TYPE_VALUE { 17u };


    constexpr int SH_LINK_VALUE { 3 };

    const QString DYNAMIC_SH_LINK_TEXT_VALUE { "The section header index of the associated string table: 3" };
    const QString HASH_SH_LINK_TEXT_VALUE { "The section header index of the associated symbol table: 3" };
    const QString RELOCATION_SH_LINK_TEXT_VALUE { "The section header index of the associated symbol table: 3" };
    const QString SYMTAB_SH_LINK_TEXT_VALUE { "The section header index of the associated string table: 3" };
    const QString GROUP_SH_LINK_TEXT_VALUE { "The section header index of the associated symbol table: 3" };

    constexpr int SH_INFO_VALUE { 5 };

    const QString RELOCATION_SH_INFO_TEXT_VALUE { "Section header index to which the relocation applies: 5" };
    const QString SYMTAB_SH_INFO_TEXT_VALUE { "One greater than the last symbol table index of the local symbol (binding STB_LOCAL): 5" };
    const QString GROUP_SH_INFO_TEXT_VALUE { "The index in the associated symbol table. Symbol's name is the signature for the section group: 5" };

    const QString EMPTY_TEXT_VALUE {};
}


using namespace ::testing;


struct FieldDescriptionUtilityIdentPartBitVersionTestSuite: public TestWithParam<std::tuple<int, QString>>
{};

TEST_P(FieldDescriptionUtilityIdentPartBitVersionTestSuite, shouldGetProperBitVersionHighLevelValue)
{
    auto l_params { GetParam() };
    auto l_bitVersion { std::get<0>(l_params) };
    auto l_expectedBitVersionHighLevelValue { std::get<1>(l_params) };
    EXPECT_EQ(getBitVersionHighLevelValue(l_bitVersion), l_expectedBitVersionHighLevelValue);
}

INSTANTIATE_TEST_SUITE_P(IdentPartBitVersionTest,
                         FieldDescriptionUtilityIdentPartBitVersionTestSuite,
                         Values(std::tuple<int, QString>(BIT_VERSION_32_VALUE, BIT_VERSION_32_HIGH_LEVEL_VALUE),
                                std::tuple<int, QString>(BIT_VERSION_64_VALUE, BIT_VERSION_64_HIGH_LEVEL_VALUE)));


struct FieldDescriptionUtilityIdentPartEndiannessTestSuite: public TestWithParam<std::tuple<int, QString>>
{};

TEST_P(FieldDescriptionUtilityIdentPartEndiannessTestSuite, shouldGetProperEndiannessHighLevelValue)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_expectedEndiannessHighLevelValue { std::get<1>(l_params) };
    EXPECT_EQ(getEndiannessHighLevelValue(l_endianness), l_expectedEndiannessHighLevelValue);
}

INSTANTIATE_TEST_SUITE_P(IdentPartEndiannessTest,
                         FieldDescriptionUtilityIdentPartEndiannessTestSuite,
                         Values(std::tuple<int, QString>(LITTLE_ENDIAN_VALUE, LITTLE_ENDIAN_HIGH_LEVEL_VALUE),
                                std::tuple<int, QString>(BIG_ENDIAN_VALUE, BIG_ENDIAN_HIGH_LEVEL_VALUE)));


struct FieldDescriptionUtilityIdentPartElfVersionTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilityIdentPartElfVersionTestSuite, shouldGetProperElfVersionHighLevelValue)
{
    EXPECT_EQ(getELFVersionHighLevelValue(ELF_VERSION_VALUE), ELF_VERSION_HIGH_LEVEL_VALUE);
}

struct FieldDescriptionUtilityIdentPartABITestSuite : public Test
{};

TEST_F(FieldDescriptionUtilityIdentPartABITestSuite, shouldGetProperABIHighLevelValue)
{
    EXPECT_EQ(getABIHighLevelValue(SYSV_ABI_VALUE), SYSV_ABI_HIGH_LEVEL_VALUE);
}


struct FieldDescriptionUtilityIdentPartABIVersionTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilityIdentPartABIVersionTestSuite, shouldGetProperABIVersionHighLevelValue)
{
    EXPECT_EQ(getABIVersionHighLevelValue(ABI_VERSION_VALUE), ABI_VERSION_HIGH_LEVEL_VALUE);
}


struct FieldDescriptionUtilityIdentPartObjectTypeTestSuite : public TestWithParam<std::tuple<int, QString>>
{};

TEST_P(FieldDescriptionUtilityIdentPartObjectTypeTestSuite, shouldGetProperObjectTypeHighLevelValue)
{
    auto l_params { GetParam() };
    auto l_objectType { std::get<0>(l_params) };
    auto l_expectedObjectTypeHighLevelValue { std::get<1>(l_params) };
    EXPECT_EQ(getObjectTypeHighLevelValue(l_objectType), l_expectedObjectTypeHighLevelValue);
}

INSTANTIATE_TEST_SUITE_P(IdentPartObjectTypeTest,
                         FieldDescriptionUtilityIdentPartObjectTypeTestSuite,
                         Values(std::tuple<int, QString>(RELOCATION_FILE_VALUE, RELOCATION_FILE_HIGH_LEVEL_VALUE),
                                std::tuple<int, QString>(EXECUTABLE_FILE_VALUE, EXECUTABLE_FILE_HIGH_LEVEL_VALUE),
                                std::tuple<int, QString>(SHARED_OBJECT_FILE_VALUE, SHARED_OBJECT_FILE_HIGH_LEVEL_VALUE),
                                std::tuple<int, QString>(CORE_FILE_VALUE, CORE_FILE_HIGH_LEVEL_VALUE)));


struct FieldDescriptionUtilityIdentPartInstructionSetArchitectureTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilityIdentPartInstructionSetArchitectureTestSuite, shouldGetProperInstructionSetArchitectureHighLevelValue)
{
    EXPECT_EQ(getInstructionSetArchitectureHighLevelValue(AMD_X86_64_ARCHITECTURE_VALUE), AMD_X86_64_HIGH_LEVEL_VALUE);
}



struct FieldDescriptionUtilityNumberConversionsTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilityNumberConversionsTestSuite, shouldConvertNumberToDecimalNumberString)
{

    EXPECT_EQ(getDecimalNumberString(NUMBER_10), DECIMAL_10_TEXT_VALUE);
    EXPECT_EQ(getDecimalNumberString(NUMBER_0), DECIMAL_0_TEXT_VALUE);
}

TEST_F(FieldDescriptionUtilityNumberConversionsTestSuite, shouldConvertNumberToHexNumberString)
{

    EXPECT_EQ(getHexNumberString(NUMBER_10), HEX_10_TEXT_VALUE);
    EXPECT_EQ(getHexNumberString(NUMBER_0), HEX_0_TEXT_VALUE);
}

TEST_F(FieldDescriptionUtilityNumberConversionsTestSuite, shouldConvertNumberToBinaryNumberString)
{

    EXPECT_EQ(getBinaryNumberString(NUMBER_10, BINARY_LENGTH_8), BINARY_10_WITH_8_BITS_LENGTH_TEXT_VALUE);
    EXPECT_EQ(getBinaryNumberString(NUMBER_10, BINARY_LENGTH_4), BINARY_10_WITH_4_BITS_LENGTH_TEXT_VALUE);
    EXPECT_EQ(getBinaryNumberString(NUMBER_0, BINARY_LENGTH_8), BINARY_0_WITH_8_BITS_LENGTH_TEXT_VALUE);
    EXPECT_EQ(getBinaryNumberString(NUMBER_0, BINARY_LENGTH_4), BINARY_0_WITH_4_BITS_LENGTH_TEXT_VALUE);
}


struct FieldDescriptionUtilitySectionTypeTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilitySectionTypeTestSuite, shouldGetProperSectionTypeHighLevelValue)
{
    EXPECT_EQ(getSectionTypeHighLevelValue(UNUSED_SECTION_HEADER_TABLE_TYPE_VALUE), UNUSED_SECTION_HEADER_TABLE_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(PROGBITS_TYPE_VALUE), PROGBITS_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(SYMTAB_TYPE_VALUE), SYMTAB_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(STRTAB_TYPE_VALUE), STRTAB_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(RELA_TYPE_VALUE), RELA_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(HASH_TYPE_VALUE), HASH_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(DYNAMIC_TYPE_VALUE), DYNAMIC_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(NOTE_TYPE_VALUE), NOTE_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(NOBITS_TYPE_VALUE), NOBITS_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(REL_TYPE_VALUE), REL_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(SHLIB_TYPE_VALUE), SHLIB_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(DYNSYM_TYPE_VALUE), DYNSYM_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(INIT_ARRAY_TYPE_VALUE), INIT_ARRAY_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(FINI_ARRAY_TYPE_VALUE), FINI_ARRAY_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(PREINIT_ARRAY_TYPE_VALUE), PREINIT_ARRAY_TYPE_TEXT_VALUE);
    EXPECT_EQ(getSectionTypeHighLevelValue(GROUP_TYPE_VALUE), GROUP_TYPE_TEXT_VALUE);
}


struct FieldDescriptionUtilitySectionInfoTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilitySectionInfoTestSuite, shouldGetProperSectionInfoHighLevelValue)
{
    EXPECT_EQ(getSectionInfoHighLevelValue(REL_SH_TYPE_VALUE, SH_INFO_VALUE), RELOCATION_SH_INFO_TEXT_VALUE);
    EXPECT_EQ(getSectionInfoHighLevelValue(RELA_SH_TYPE_VALUE, SH_INFO_VALUE), RELOCATION_SH_INFO_TEXT_VALUE);
    EXPECT_EQ(getSectionInfoHighLevelValue(SYMTAB_SH_TYPE_VALUE, SH_INFO_VALUE), SYMTAB_SH_INFO_TEXT_VALUE);
    EXPECT_EQ(getSectionInfoHighLevelValue(DYNAMIC_SYMTAB_SH_TYPE_VALUE, SH_INFO_VALUE), SYMTAB_SH_INFO_TEXT_VALUE);
    EXPECT_EQ(getSectionInfoHighLevelValue(GROUP_SH_TYPE_VALUE, SH_INFO_VALUE), GROUP_SH_INFO_TEXT_VALUE);
}

TEST_F(FieldDescriptionUtilitySectionInfoTestSuite, shouldNotGetSectionInfoHighLevelValueWhenNotValidSectionType)
{
    EXPECT_EQ(getSectionInfoHighLevelValue(SECTION_TYPE_WITHOUT_LINK_OR_INFO_VALUE, SH_INFO_VALUE), EMPTY_TEXT_VALUE);
}


struct FieldDescriptionUtilitySectionLinkTestSuite : public Test
{};

TEST_F(FieldDescriptionUtilitySectionLinkTestSuite, shouldGetProperSectionLinkHighLevelValue)
{
    EXPECT_EQ(getSectionLinkHighLevelValue(DYNAMIC_SH_TYPE_VALUE, SH_LINK_VALUE), DYNAMIC_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(HASH_SH_TYPE_VALUE, SH_LINK_VALUE), HASH_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(REL_SH_TYPE_VALUE, SH_LINK_VALUE), RELOCATION_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(RELA_SH_TYPE_VALUE, SH_LINK_VALUE), RELOCATION_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(SYMTAB_SH_TYPE_VALUE, SH_LINK_VALUE), SYMTAB_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(DYNAMIC_SYMTAB_SH_TYPE_VALUE, SH_LINK_VALUE), SYMTAB_SH_LINK_TEXT_VALUE);
    EXPECT_EQ(getSectionLinkHighLevelValue(GROUP_SH_TYPE_VALUE, SH_LINK_VALUE), GROUP_SH_LINK_TEXT_VALUE);
}

TEST_F(FieldDescriptionUtilitySectionLinkTestSuite, shouldNotGetSectionLinkHighLevelValueWhenNotValidSectionType)
{
    EXPECT_EQ(getSectionLinkHighLevelValue(SECTION_TYPE_WITHOUT_LINK_OR_INFO_VALUE, SH_LINK_VALUE), EMPTY_TEXT_VALUE);
}

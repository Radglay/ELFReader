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

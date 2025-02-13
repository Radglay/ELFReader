#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "NoteDescriptorBuilder.hpp"
#include "GnuProperty.hpp"
#include <vector>
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"
#include <elf.h>
#include <string>
#include <sstream>
#include <tuple>
#include "AbiTagInformation.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };

constexpr int BIT_VERSION_32 { ELFCLASS32 };
constexpr int BIT_VERSION_64 { ELFCLASS64 };

constexpr int DESCRIPTOR_OFFSET_ZERO {};
constexpr int DESCRIPTOR_SIZE_ZERO {};



// (1) NOTE_GNU_PROPERTY
std::vector<unsigned char> GNU_PROPERTY_SIZE_LITTLE_ENDIAN {
    0x04, 0x00, 0x00, 0x00
};

std::vector<unsigned char> GNU_PROPERTY_SIZE_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x04
};

std::vector<unsigned char> GNU_PROPERTY_X86_FEATURES_TYPE_LITTLE_ENDIAN {
    0x02, 0x00, 0x00, 0xc0
};

std::vector<unsigned char> GNU_PROPERTY_X86_FEATURES_TYPE_BIG_ENDIAN {
    0xc0, 0x00, 0x00, 0x02
};


std::vector<unsigned char> GNU_PROPERTY_X86_NO_FEATURES {
    0x00, 0x00, 0x00, 0x00
};


std::vector<unsigned char> GNU_PROPERTY_X86_IBT_FEATURE_LITTLE_ENDIAN {
    0x01, 0x00, 0x00, 0x00
};

std::vector<unsigned char> GNU_PROPERTY_X86_IBT_FEATURE_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x01
};


std::vector<unsigned char> GNU_PROPERTY_X86_SHSTK_FEATURE_LITTLE_ENDIAN {
    0x02, 0x00, 0x00, 0x00
};

std::vector<unsigned char> GNU_PROPERTY_X86_SHSTK_FEATURE_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x02
};


std::vector<unsigned char> GNU_PROPERTY_X86_IBT_AND_SHSTK_FEATURES_LITTLE_ENDIAN {
    0x03, 0x00, 0x00, 0x00
};

std::vector<unsigned char> GNU_PROPERTY_X86_IBT_AND_SHSTK_FEATURES_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x03
};

constexpr bool EXECUTABLE_SECTIONS_COMPATIBLE_WITH_IBT { true };
constexpr bool EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_IBT { false };

constexpr bool EXECUTABLE_SECTIONS_COMPATIBLE_WITH_SHSTK { true };
constexpr bool EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_SHSTK { false };

constexpr int GNU_PROPERTY_X86_FEATURES_DESCRIPTOR_SIZE {  12 };


std::vector<unsigned char> GNU_PROPERTY_ISA_TYPE_OPTIONAL_LITTLE_ENDIAN {
    0x02, 0x00, 0x01, 0xc0
};

std::vector<unsigned char> GNU_PROPERTY_ISA_TYPE_OPTIONAL_BIG_ENDIAN {
    0xc0, 0x01, 0x00, 0x02
};


std::vector<unsigned char> GNU_PROPERTY_ISA_TYPE_REQUIRED_LITTLE_ENDIAN {
    0x02, 0x80, 0x00, 0xc0
};

std::vector<unsigned char> GNU_PROPERTY_ISA_TYPE_REQUIRED_BIG_ENDIAN {
    0xc0, 0x00, 0x80, 0x02
};


std::vector<unsigned char> GNU_PROPERTY_ISA_VERSION_BASELINE_LITTLE_ENDIAN {
    0x01, 0x00, 0x00, 0x00
};

std::vector<unsigned char> GNU_PROPERTY_ISA_VERSION_BASELINE_BIG_ENDIAN {
    0x00, 0x00, 0x00, 0x01
};


std::vector<unsigned char> GNU_PROPERTY_ALIGN {
    0x00, 0x00, 0x00, 0x00
};


constexpr bool GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_OPTIONAL { false };
constexpr bool GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_REQUIRED { true };

constexpr int GNU_PROPERTY_X86_ISA_VERSION_BASELINE { 1 };

constexpr int GNU_PROPERTY_X86_ISA_DESCRIPTOR_SIZE {  12 };


// (2) NOTE_GNU_BUILD_ID
std::vector<unsigned char> NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS {
    0x87, 0xf7, 0xb0, 0x6c, 0xa1, 0xce, 0x3f, 0x0b,
    0x73, 0xc8, 0x91, 0x1d, 0x06, 0x8b, 0xcd, 0xa1,
    0xa9, 0xbc, 0x32, 0xf4
};



// (3) NOTE_ABI_TAG
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


std::string generateGnuPropertyContent(const std::vector<unsigned char>& p_featuresType,
                                       const std::vector<unsigned char>& p_size,
                                       const std::vector<unsigned char>& p_featuresValue)
{
    std::string l_streamContent;

    l_streamContent.append(reinterpret_cast<const char*>(p_featuresType.data()), p_featuresType.size());
    l_streamContent.append(reinterpret_cast<const char*>(p_size.data()), p_size.size());
    l_streamContent.append(reinterpret_cast<const char*>(p_featuresValue.data()), p_featuresValue.size());

    return l_streamContent;
}

std::string generateAbiTagInformationContent(const std::vector<unsigned char>& p_abiTagInformation)
{
    std::string l_streamContent;

    l_streamContent.append(reinterpret_cast<const char*>(p_abiTagInformation.data()), p_abiTagInformation.size());

    return l_streamContent;
}

}

using namespace ::testing;


struct NoteDesriptorBuilderTestSuite : Test
{};

TEST_F(NoteDesriptorBuilderTestSuite, shouldNotBuildAnyBuildIdInformationWhenSizeIsZero)
{
    std::string l_streamContent;
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.size());

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildBuildIdInformation(DESCRIPTOR_OFFSET_ZERO, DESCRIPTOR_SIZE_ZERO) };
    ASSERT_EQ(l_targetDescriptor.size(), 0);
}

TEST_F(NoteDesriptorBuilderTestSuite, shouldBuildCorrectBuildIdInformationWhenDescriptorIsProvided)
{
    std::string l_streamContent;
    l_streamContent.append(reinterpret_cast<const char*>(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.data()), NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.size());

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildBuildIdInformation(DESCRIPTOR_OFFSET_ZERO, NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.size()) };
    ASSERT_EQ(l_targetDescriptor.size(), NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS.size());

    EXPECT_THAT(l_targetDescriptor, ContainerEq(NOTE_GNU_BUILD_ID_DESCRIPTOR_BOTH_ENDIANNESS));
}


class AbiTagInformationDescriptorBuildingTestSuite : public NoteDesriptorBuilderTestSuite, public WithParamInterface<std::tuple<int, std::string>>
{};

TEST_P(AbiTagInformationDescriptorBuildingTestSuite, shouldBuildAbiTagInformationWhenDescriptorIsProvided)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};
    l_targetMachineInfo.endianness = l_endianness;

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildAbiTagInformation(DESCRIPTOR_OFFSET_ZERO) };

    EXPECT_EQ(l_targetDescriptor.osDescriptor, ABI_TAG_INFORMATION.osDescriptor);
    EXPECT_EQ(l_targetDescriptor.majorVersion, ABI_TAG_INFORMATION.majorVersion);
    EXPECT_EQ(l_targetDescriptor.minorVersion, ABI_TAG_INFORMATION.minorVersion);
    EXPECT_EQ(l_targetDescriptor.subminorVersion, ABI_TAG_INFORMATION.subminorVersion);
}

INSTANTIATE_TEST_SUITE_P(AbiTagInformationDescriptorBuilding,
    AbiTagInformationDescriptorBuildingTestSuite,
    Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generateAbiTagInformationContent(NOTE_ABI_TAG_DESCRIPTOR_LITTLE_ENDIAN)),
           std::make_tuple(BIG_ENDIAN_VALUE, generateAbiTagInformationContent(NOTE_ABI_TAG_DESCRIPTOR_BIG_ENDIAN))));


class GnuPropertyX86InstructionSetDesriptorBuildingTestSuite : public NoteDesriptorBuilderTestSuite, public WithParamInterface<std::tuple<int, std::string, int>>
{};

TEST_P(GnuPropertyX86InstructionSetDesriptorBuildingTestSuite, shouldNotBuildAnyGnuPropertyWhenSizeIsZero)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};
    l_targetMachineInfo.endianness = l_endianness;

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildGnuPropertyInformation(DESCRIPTOR_OFFSET_ZERO, DESCRIPTOR_SIZE_ZERO) };
    ASSERT_EQ(l_targetDescriptor.size(), 0);
}

TEST_P(GnuPropertyX86InstructionSetDesriptorBuildingTestSuite, shouldBuildGnuPropertyX86InstructionSetWhenDescriptorIsProvided)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};
    l_targetMachineInfo.endianness = l_endianness;

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildGnuPropertyInformation(DESCRIPTOR_OFFSET_ZERO, GNU_PROPERTY_X86_ISA_DESCRIPTOR_SIZE) };
    ASSERT_EQ(l_targetDescriptor.size(), 1);

    auto l_targetX86InstructionSetProperty { dynamic_cast<GnuProperty<GnuPropertyX86InstructionSet>&>(*l_targetDescriptor[0]).getProperty() };

    auto l_expectedISAHardwareSupport { std::get<2>(l_params) };
    EXPECT_EQ(l_targetX86InstructionSetProperty.isHardwareSupportRequired, l_expectedISAHardwareSupport);
    EXPECT_EQ(l_targetX86InstructionSetProperty.instructionSetVersion, GNU_PROPERTY_X86_ISA_VERSION_BASELINE);
}

INSTANTIATE_TEST_SUITE_P(GnuPropertyX86InstructionSetNoteDescriptorBuilding,
    GnuPropertyX86InstructionSetDesriptorBuildingTestSuite,
    Values(std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_ISA_TYPE_OPTIONAL_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_ISA_VERSION_BASELINE_LITTLE_ENDIAN),
                           GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_OPTIONAL),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_ISA_TYPE_OPTIONAL_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_ISA_VERSION_BASELINE_BIG_ENDIAN),
                           GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_OPTIONAL),
           std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_ISA_TYPE_REQUIRED_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_ISA_VERSION_BASELINE_LITTLE_ENDIAN),
                           GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_REQUIRED),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_ISA_TYPE_REQUIRED_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_ISA_VERSION_BASELINE_BIG_ENDIAN),
                           GNU_PROPERTY_X86_ISA_HARDWARE_SUPPORT_REQUIRED)));


class GnuPropertyX86FeaturesDesriptorBuildingTestSuite : public NoteDesriptorBuilderTestSuite, public WithParamInterface<std::tuple<int, std::string, bool, bool>>
{};

TEST_P(GnuPropertyX86FeaturesDesriptorBuildingTestSuite, shouldBuildGnuPropertyX86FeaturesWhenDescriptorIsProvided)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };

    TargetMachineInfo l_targetMachineInfo {};
    l_targetMachineInfo.endianness = l_endianness;

    NoteDescriptorBuilder l_noteDescriptorBuilder { &l_stubStream, l_targetMachineInfo };

    auto l_targetDescriptor { l_noteDescriptorBuilder.buildGnuPropertyInformation(DESCRIPTOR_OFFSET_ZERO, GNU_PROPERTY_X86_FEATURES_DESCRIPTOR_SIZE) };
    ASSERT_EQ(l_targetDescriptor.size(), 1);

    auto l_targetX86FeaturesProperty { dynamic_cast<GnuProperty<GnuPropertyX86Features>&>(*l_targetDescriptor[0]).getProperty() };

    auto l_expectedIBTSupport { std::get<2>(l_params) };
    auto l_expectedSHSTKSupport { std::get<3>(l_params) };
    EXPECT_EQ(l_targetX86FeaturesProperty.isCompatibleWithIBT, l_expectedIBTSupport);
    EXPECT_EQ(l_targetX86FeaturesProperty.isCompatibleWithSHSTK, l_expectedSHSTKSupport);
}

INSTANTIATE_TEST_SUITE_P(GnuPropertyX86FeaturesNoteDescriptorBuilding,
    GnuPropertyX86FeaturesDesriptorBuildingTestSuite,
    Values(std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_X86_NO_FEATURES),
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_X86_NO_FEATURES),
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_X86_IBT_FEATURE_LITTLE_ENDIAN),
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_X86_IBT_FEATURE_BIG_ENDIAN),
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_X86_SHSTK_FEATURE_LITTLE_ENDIAN),
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_X86_SHSTK_FEATURE_BIG_ENDIAN),
                           EXECUTABLE_SECTIONS_NOT_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(LITTLE_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_LITTLE_ENDIAN, GNU_PROPERTY_SIZE_LITTLE_ENDIAN, GNU_PROPERTY_X86_IBT_AND_SHSTK_FEATURES_LITTLE_ENDIAN),
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_SHSTK),
           std::make_tuple(BIG_ENDIAN_VALUE,
                           generateGnuPropertyContent(GNU_PROPERTY_X86_FEATURES_TYPE_BIG_ENDIAN, GNU_PROPERTY_SIZE_BIG_ENDIAN, GNU_PROPERTY_X86_IBT_AND_SHSTK_FEATURES_BIG_ENDIAN),
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_IBT,
                           EXECUTABLE_SECTIONS_COMPATIBLE_WITH_SHSTK)));

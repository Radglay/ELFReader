#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "NoteDescriptorUtility.hpp"
#include "ElfPart.hpp"
#include <vector>
#include "AbiTagInformation.hpp"
#include <elf.h>
#include <QString>
#include "GnuPropertyMock.hpp"
#include <memory>
#include "GnuProperty.hpp"
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"


namespace
{

    const QString EMPTY_TEXT {};

    constexpr int VERSION_1_VALUE { 1 };
    constexpr int VERSION_2_VALUE { 2 };
    constexpr int VERSION_3_VALUE { 3 };

    constexpr int UNKNOWN_OS_DESCRIPTOR_VALUE { 123 };
    const QString UNKNOWN_OS_DESCRIPTOR_VALUE_TEXT { "123" };
    const QString UNKNOWN_OS_DESCRIPTOR_DESCRIPTION_TEXT { "OS descriptor: Unknown" };

    constexpr int LINUX_OS_DESCRIPTOR_VALUE { ELF_NOTE_OS_LINUX };
    const QString LINUX_OS_DESCRIPTOR_VALUE_TEXT { "0" };
    const QString LINUX_OS_DESCRIPTOR_DESCRIPTION_TEXT { "OS descriptor: ELF_NOTE_OS_LINUX" };

    constexpr int GNU_OS_DESCRIPTOR_VALUE { ELF_NOTE_OS_GNU };
    const QString GNU_OS_DESCRIPTOR_VALUE_TEXT { "1" };
    const QString GNU_OS_DESCRIPTOR_DESCRIPTION_TEXT { "OS descriptor: ELF_NOTE_OS_GNU" };

    const QString OS_DESCRIPTOR_FIELD_NAME_TEXT { "osDescriptor" };
    const QString MAJOR_VERSION_FIELD_NAME_TEXT { "majorVersion" };
    const QString MINOR_VERSION_FIELD_NAME_TEXT { "minorVersion" };
    const QString SUBMINOR_VERSION_FIELD_NAME_TEXT { "subminorVersion" };

    const QString MAJOR_VERSION_FIELD_VALUE_TEXT { "1" };
    const QString MINOR_VERSION_FIELD_VALUE_TEXT { "2" };
    const QString SUBMINOR_VERSION_FIELD_VALUE_TEXT { "3" };

    const QString MAJOR_VERSION_FIELD_DESCRIPTION_TEXT { "Major version of the ABI: 1" };
    const QString MINOR_VERSION_FIELD_DESCRIPTION_TEXT { "Minor version of the ABI: 2" };
    const QString SUBMINOR_VERSION_FIELD_DESCRIPTION_TEXT { "Subminor version of the ABI: 3" };


    const QString ABI_TAG_INFORMATION_TYPE_TEXT { "AbiTagInformation" };

    ElfField generateAbiTagInformationElfField(const QString& p_osDescriptorValueText, const QString& p_osDescriptorDescriptionText)
    {
        std::vector<ElfField> l_fields;
        l_fields.emplace_back(OS_DESCRIPTOR_FIELD_NAME_TEXT, UINT32_T_TEXT, p_osDescriptorValueText, p_osDescriptorDescriptionText);
        l_fields.emplace_back(MAJOR_VERSION_FIELD_NAME_TEXT, UINT32_T_TEXT, MAJOR_VERSION_FIELD_VALUE_TEXT, MAJOR_VERSION_FIELD_DESCRIPTION_TEXT);
        l_fields.emplace_back(MINOR_VERSION_FIELD_NAME_TEXT, UINT32_T_TEXT, MINOR_VERSION_FIELD_VALUE_TEXT, MINOR_VERSION_FIELD_DESCRIPTION_TEXT);
        l_fields.emplace_back(SUBMINOR_VERSION_FIELD_NAME_TEXT, UINT32_T_TEXT, SUBMINOR_VERSION_FIELD_VALUE_TEXT, SUBMINOR_VERSION_FIELD_DESCRIPTION_TEXT);

        return { EMPTY_TEXT, ABI_TAG_INFORMATION_TYPE_TEXT, EMPTY_TEXT, EMPTY_TEXT, l_fields };
    }


    const QString GNU_PROPERTY_TYPE_TEXT { "GnuProperty[]" };

    ElfField generateGnuPropertyElfField(const std::vector<ElfField>& p_fields)
    {
        return { EMPTY_TEXT, GNU_PROPERTY_TYPE_TEXT, EMPTY_TEXT, EMPTY_TEXT, p_fields};
    }


    const QString GNU_BUILD_ID_TYPE_TEXT { "GnuBuildId" };

    const std::vector<unsigned char> GNU_BUILD_ID {
        0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x7, 0x8, 0x9, 0xA, 0xB, 0xC, 0xD, 0xE, 0xF
    };

    const QString GNU_BUILD_ID_TEXT { "123456789ABCDEF" };

    ElfField generateGnuBuildIdElfField()
    {
        return { EMPTY_TEXT, GNU_BUILD_ID_TYPE_TEXT, GNU_BUILD_ID_TEXT, EMPTY_TEXT};
    }
}

using namespace ::testing;


class NoteDescriptorUtilityTestSuite : public ElfPartAssemblingTestSuite
{};

class AbiTagInformationDescriptorTestSuite : public NoteDescriptorUtilityTestSuite
{};

TEST_F(AbiTagInformationDescriptorTestSuite, shouldAssembleElfFieldWhenLinuxOsDescriptorIsProvided)
{
    AbiTagInformation l_abiTagInformation {
        .osDescriptor = LINUX_OS_DESCRIPTOR_VALUE,
        .majorVersion = VERSION_1_VALUE,
        .minorVersion = VERSION_2_VALUE,
        .subminorVersion = VERSION_3_VALUE
    };

    auto l_targetElfField { assembleElfPartFromDescriptor(l_abiTagInformation) };

    auto l_expectedElfField { generateAbiTagInformationElfField(LINUX_OS_DESCRIPTOR_VALUE_TEXT, LINUX_OS_DESCRIPTOR_DESCRIPTION_TEXT) };

    expectElfFieldsAreEqual(l_targetElfField, l_expectedElfField);
}

TEST_F(AbiTagInformationDescriptorTestSuite, shouldAssembleElfFieldWhenGnuOsDescriptorIsProvided)
{
    AbiTagInformation l_abiTagInformation {
        .osDescriptor = GNU_OS_DESCRIPTOR_VALUE,
        .majorVersion = VERSION_1_VALUE,
        .minorVersion = VERSION_2_VALUE,
        .subminorVersion = VERSION_3_VALUE
    };

    auto l_targetElfField { assembleElfPartFromDescriptor(l_abiTagInformation) };

    auto l_expectedElfField { generateAbiTagInformationElfField(GNU_OS_DESCRIPTOR_VALUE_TEXT, GNU_OS_DESCRIPTOR_DESCRIPTION_TEXT) };

    expectElfFieldsAreEqual(l_targetElfField, l_expectedElfField);
}

TEST_F(AbiTagInformationDescriptorTestSuite, shouldAssembleElfFieldWhenUnknownOsDescriptorIsProvided)
{
    AbiTagInformation l_abiTagInformation {
        .osDescriptor = UNKNOWN_OS_DESCRIPTOR_VALUE,
        .majorVersion = VERSION_1_VALUE,
        .minorVersion = VERSION_2_VALUE,
        .subminorVersion = VERSION_3_VALUE
    };

    auto l_targetElfField { assembleElfPartFromDescriptor(l_abiTagInformation) };

    auto l_expectedElfField { generateAbiTagInformationElfField(UNKNOWN_OS_DESCRIPTOR_VALUE_TEXT, UNKNOWN_OS_DESCRIPTOR_DESCRIPTION_TEXT) };

    expectElfFieldsAreEqual(l_targetElfField, l_expectedElfField);
}


class GnuPropertyDescriptorTestSuite : public NoteDescriptorUtilityTestSuite
{};

TEST_F(GnuPropertyDescriptorTestSuite, shouldNotAssembleAnyElfFieldIfThereIsNoGnuPropertyProvided)
{
    std::vector<std::shared_ptr<IGnuProperty>> l_gnuPropertyMocks {};

    auto l_targetGnuPropertyField { assembleElfPartFromDescriptor(l_gnuPropertyMocks)};

    std::vector<ElfField> l_expectedGnuProperties {};
    auto l_expectedGnuPropertyField { generateGnuPropertyElfField(l_expectedGnuProperties) };

    expectElfFieldsAreEqual(l_targetGnuPropertyField, l_expectedGnuPropertyField);
}

TEST_F(GnuPropertyDescriptorTestSuite, shouldAssembleAllElfFieldsIfThereAreGnuPropertiesProvided)
{
    std::vector<std::shared_ptr<IGnuProperty>> l_gnuPropertyMocks {};

    auto l_firstGnuProperty { std::make_shared<GnuPropertyMock>() };
    auto l_secondGnuProperty { std::make_shared<GnuPropertyMock>() };

    l_gnuPropertyMocks.push_back(l_firstGnuProperty);
    l_gnuPropertyMocks.push_back(l_secondGnuProperty);

    EXPECT_CALL(*l_firstGnuProperty, acceptGnuPropertyVisitor)
        .Times(1)
        .WillOnce(Return(ElfField{}));

    EXPECT_CALL(*l_secondGnuProperty, acceptGnuPropertyVisitor)
        .Times(1)
        .WillOnce(Return(ElfField{}));

    auto l_targetGnuPropertyField { assembleElfPartFromDescriptor(l_gnuPropertyMocks)};

    std::vector<ElfField> l_expectedGnuProperties(2);
    auto l_expectedGnuPropertyField { generateGnuPropertyElfField(l_expectedGnuProperties) };

    expectElfFieldsAreEqual(l_targetGnuPropertyField, l_expectedGnuPropertyField);
}


class GnuBuildIdDescriptorTestSuite : public NoteDescriptorUtilityTestSuite
{};

TEST_F(GnuBuildIdDescriptorTestSuite, shouldAssemblerElfFieldIfGnuBuildIdIsProvided)
{
    auto l_targetGnuBuildId { assembleElfPartFromDescriptor(GNU_BUILD_ID)};
    auto l_expectedGnuBuildId { generateGnuBuildIdElfField() };

    expectElfFieldsAreEqual(l_targetGnuBuildId, l_expectedGnuBuildId);
}

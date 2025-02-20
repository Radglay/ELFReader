#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "GnuPropertyVisitor.hpp"
#include "ElfPart.hpp"
#include <QString>
#include <vector>
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"


namespace
{
    const QString EMPTY_TEXT {};

    const QString X86_FEATURES_FIELD_NAME_TEXT { "Features" };
    const QString GNU_PROPERTY_X86_FEATURES_FIELD_TYPE_TEXT { "GnuPropertyX86Features" };

    const QString IS_COMPATIBLE_WITH_IBT_FIELD_NAME_TEXT { "isCompatibleWithIBT" };
    const QString IS_COMPATIBLE_WITH_SHSTK_FIELD_NAME_TEXT { "isCompatibleWithSHSTK" };


    const QString X86_INSTRUCTION_SET_FIELD_NAME_TEXT { "InstructionSet" };
    const QString GNU_PROPERTY_X86_INSTRUCTION_SET_FIELD_TYPE_TEXT { "GnuPropertyX86InstructionSet" };

    const QString IS_HARDWARE_SUPPORT_REQUIRED_FIELD_NAME_TEXT { "isHardwareSupportRequired" };
    const QString INSTRUCTION_SET_VERSION_FIELD_NAME_TEXT { "instructionSetVersion" };

    constexpr int INSTRUCTION_SET_VERSION_1 { 1 };
    const QString INSTRUCTION_SET_VERSION_1_TEXT { "1" };

    const QString INT_TEXT { "int" };
    const QString BOOL_TEXT { "bool" };

    constexpr bool TRUE_VALUE { true };
    const QString TRUE_VALUE_TEXT { "true" };

    constexpr bool FALSE_VALUE { false };
    const QString FALSE_VALUE_TEXT { "false" };


    ElfField generateX86FeaturesElfField()
    {
        std::vector<ElfField> l_fields;
        l_fields.emplace_back(IS_COMPATIBLE_WITH_IBT_FIELD_NAME_TEXT, BOOL_TEXT, TRUE_VALUE_TEXT, EMPTY_TEXT);
        l_fields.emplace_back(IS_COMPATIBLE_WITH_SHSTK_FIELD_NAME_TEXT, BOOL_TEXT, FALSE_VALUE_TEXT, EMPTY_TEXT);

        return { X86_FEATURES_FIELD_NAME_TEXT, GNU_PROPERTY_X86_FEATURES_FIELD_TYPE_TEXT, EMPTY_TEXT, EMPTY_TEXT, l_fields };
    }

    ElfField generateX86InstructionSetElfField()
    {
        std::vector<ElfField> l_fields;
        l_fields.emplace_back(IS_HARDWARE_SUPPORT_REQUIRED_FIELD_NAME_TEXT, BOOL_TEXT, TRUE_VALUE_TEXT, EMPTY_TEXT);
        l_fields.emplace_back(INSTRUCTION_SET_VERSION_FIELD_NAME_TEXT, INT_TEXT, INSTRUCTION_SET_VERSION_1_TEXT, EMPTY_TEXT);

        return { X86_INSTRUCTION_SET_FIELD_NAME_TEXT, GNU_PROPERTY_X86_INSTRUCTION_SET_FIELD_TYPE_TEXT, EMPTY_TEXT, EMPTY_TEXT, l_fields };
    }
}

using namespace ::testing;


class GnuPropertyVisitorTestSuite : public ElfPartAssemblingTestSuite
{};


TEST_F(GnuPropertyVisitorTestSuite, shouldAssembleElfFieldFromGnuPropertyX86Features)
{
    GnuPropertyVisitor l_gnuPropertyVisitor {};

    GnuPropertyX86Features l_gnuPropertyFeatures
    {
        .isCompatibleWithIBT = TRUE_VALUE,
        .isCompatibleWithSHSTK = FALSE_VALUE
    };

    auto l_targetGnuProperty { l_gnuPropertyVisitor.assembleElfFieldFromGnuProperty(l_gnuPropertyFeatures) };

    auto l_expectedGnuProperty { generateX86FeaturesElfField() };

    expectElfFieldsAreEqual(l_targetGnuProperty, l_expectedGnuProperty);
}


TEST_F(GnuPropertyVisitorTestSuite, shouldAssembleElfFieldFromGnuPropertyX86InstructionSet)
{
    GnuPropertyVisitor l_gnuPropertyVisitor {};

    GnuPropertyX86InstructionSet l_gnuPropertyFeatures
    {
        .isHardwareSupportRequired = TRUE_VALUE,
        .instructionSetVersion = INSTRUCTION_SET_VERSION_1
    };

    auto l_targetGnuProperty { l_gnuPropertyVisitor.assembleElfFieldFromGnuProperty(l_gnuPropertyFeatures) };

    auto l_expectedGnuProperty { generateX86InstructionSetElfField() };

    expectElfFieldsAreEqual(l_targetGnuProperty, l_expectedGnuProperty);
}

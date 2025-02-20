#include "GnuPropertyVisitor.hpp"
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"
#include "ElfPart.hpp"
#include <vector>
#include "FieldDescriptionUtility.hpp"


ElfField GnuPropertyVisitor::assembleElfFieldFromGnuProperty(const GnuPropertyX86Features& p_gnuProperty)
{
    std::vector<ElfField> l_fields;
    l_fields.emplace_back("isCompatibleWithIBT", "bool", getBooleanString(p_gnuProperty.isCompatibleWithIBT), "");
    l_fields.emplace_back("isCompatibleWithSHSTK", "bool", getBooleanString(p_gnuProperty.isCompatibleWithSHSTK), "");

    return { "Features", "GnuPropertyX86Features", "", "", l_fields };
}

ElfField GnuPropertyVisitor::assembleElfFieldFromGnuProperty(const GnuPropertyX86InstructionSet& p_gnuProperty)
{
    std::vector<ElfField> l_fields;
    l_fields.emplace_back("isHardwareSupportRequired", "bool", getBooleanString(p_gnuProperty.isHardwareSupportRequired), "");
    l_fields.emplace_back("instructionSetVersion", "int", getDecimalNumberString(p_gnuProperty.instructionSetVersion), "");

    return { "InstructionSet", "GnuPropertyX86InstructionSet", "", "", l_fields };
}

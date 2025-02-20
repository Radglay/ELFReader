
#include "NoteDescriptorUtility.hpp"
#include "ElfPart.hpp"
#include "GnuProperty.hpp"
#include "AbiTagInformation.hpp"
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"
#include <QString>
#include "FieldDescriptionUtility.hpp"
#include <elf.h>
#include "GnuPropertyVisitor.hpp"


namespace
{

QString getOSDescriptorHighLevelInformation(uint32_t p_osDescriptor)
{
    QString l_osDescriptorText { "OS descriptor: " };
    if (p_osDescriptor == ELF_NOTE_OS_LINUX)
        l_osDescriptorText += "ELF_NOTE_OS_LINUX";
    else if (p_osDescriptor == ELF_NOTE_OS_GNU)
        l_osDescriptorText += "ELF_NOTE_OS_GNU";
    else
        l_osDescriptorText += "Unknown";

    return l_osDescriptorText;
}

}

ElfField assembleElfPartFromDescriptor(const AbiTagInformation& p_abiTagInformation)
{
    std::vector<ElfField> l_fields;
    l_fields.emplace_back("osDescriptor", "uint32_t", getDecimalNumberString(p_abiTagInformation.osDescriptor), getOSDescriptorHighLevelInformation(p_abiTagInformation.osDescriptor));
    l_fields.emplace_back("majorVersion", "uint32_t", getDecimalNumberString(p_abiTagInformation.majorVersion), "Major version of the ABI: " + getDecimalNumberString(p_abiTagInformation.majorVersion));
    l_fields.emplace_back("minorVersion", "uint32_t", getDecimalNumberString(p_abiTagInformation.minorVersion), "Minor version of the ABI: " + getDecimalNumberString(p_abiTagInformation.minorVersion));
    l_fields.emplace_back("subminorVersion", "uint32_t", getDecimalNumberString(p_abiTagInformation.subminorVersion), "Subminor version of the ABI: " + getDecimalNumberString(p_abiTagInformation.subminorVersion));

    return { "", "AbiTagInformation", "", "", l_fields };
}

ElfField assembleElfPartFromDescriptor(const std::vector<std::shared_ptr<IGnuProperty>>& p_gnuProperties)
{
    std::vector<ElfField> l_fields;
    GnuPropertyVisitor l_gnuPropertyVisitor {};

    for (const auto& p_gnuProperty : p_gnuProperties)
    {
        l_fields.emplace_back(p_gnuProperty->acceptGnuPropertyVisitor(l_gnuPropertyVisitor));
    }

    return { "", "GnuProperty[]", "", "", l_fields };
}


ElfField assembleElfPartFromDescriptor(const std::vector<unsigned char>& p_buildId)
{
    return { "", "GnuBuildId", getBytesString(p_buildId), "" };
}

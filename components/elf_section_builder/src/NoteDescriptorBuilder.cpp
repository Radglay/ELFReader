#include "NoteDescriptorBuilder.hpp"
#include "NoteSection.hpp"
#include <elf.h>
#include "BytesReadingUtility.hpp"
#include "EndiannessUtility.hpp"
#include <vector>
#include <string>
#include "TargetMachineInfo.hpp"
#include "AbiTagInformation.hpp"
#include "GnuPropertyX86Features.hpp"
#include "GnuPropertyX86InstructionSet.hpp"
#include "IGnuProperty.hpp"
#include "GnuProperty.hpp"


AbiTagInformation NoteDescriptorBuilder::buildAbiTagInformation(int p_offset)
{
    AbiTagInformation l_abiTagInformation {};
    readBytesFromFile(l_abiTagInformation, p_offset, m_fileStream);

    if (isEndiannessCorrect(m_targetMachineInfo.endianness)
        and shouldConvertEndianness(m_targetMachineInfo.endianness))
    {
        convertEndianness(l_abiTagInformation.osDescriptor);
        convertEndianness(l_abiTagInformation.majorVersion);
        convertEndianness(l_abiTagInformation.minorVersion);
        convertEndianness(l_abiTagInformation.subminorVersion);
    }

    return l_abiTagInformation;
}

std::vector<unsigned char> NoteDescriptorBuilder::buildBuildIdInformation(int p_offset, int p_size)
{
    std::vector<unsigned char> l_gnuBuildId;
    readBytesFromFileToVector(l_gnuBuildId, p_offset, p_size, m_fileStream);

    return l_gnuBuildId;
}

GnuPropertyX86Features NoteDescriptorBuilder::buildGnuPropertyX86Features(int p_offset)
{
    GnuPropertyX86Features l_x86Features {};

    uint32_t l_features {};
    readBytesFromFile(l_features, p_offset, m_fileStream);

    if (isEndiannessCorrect(m_targetMachineInfo.endianness)
        and shouldConvertEndianness(m_targetMachineInfo.endianness))
    {
        convertEndianness(l_features);
    }

    if (l_features & GNU_PROPERTY_X86_FEATURE_1_IBT)
        l_x86Features.isCompatibleWithIBT = true;

    if (l_features & GNU_PROPERTY_X86_FEATURE_1_SHSTK)
        l_x86Features.isCompatibleWithSHSTK = true;

    return l_x86Features;
}

GnuPropertyX86InstructionSet NoteDescriptorBuilder::buildGnuPropertyX86InstructionSet(int p_offset)
{
    GnuPropertyX86InstructionSet l_x86InstructionSet {};

    uint32_t l_instructionSetVersion {};
    readBytesFromFile(l_x86InstructionSet.instructionSetVersion, p_offset, m_fileStream);

    if (isEndiannessCorrect(m_targetMachineInfo.endianness)
        and shouldConvertEndianness(m_targetMachineInfo.endianness))
    {
        convertEndianness(l_x86InstructionSet.instructionSetVersion);
    }

    return l_x86InstructionSet;
}

std::vector<std::shared_ptr<IGnuProperty>> NoteDescriptorBuilder::buildGnuPropertyInformation(int p_offset, int p_size)
{
    auto l_currentOffset { p_offset };
    auto l_endOffset { p_offset + p_size };
    std::vector<std::shared_ptr<IGnuProperty>> l_gnuProperty;

    while (l_currentOffset != l_endOffset)
    {
        uint32_t l_type {};
        uint32_t l_size {};

        readBytesFromFile(l_type, l_currentOffset, m_fileStream);
        l_currentOffset += sizeof(uint32_t);

        readBytesFromFile(l_size, l_currentOffset, m_fileStream);
        l_currentOffset += sizeof(uint32_t);

        auto l_targetMachineEndianness { m_targetMachineInfo.endianness };
        if (isEndiannessCorrect(l_targetMachineEndianness)
            and shouldConvertEndianness(l_targetMachineEndianness))
        {
            convertEndianness(l_type);
            convertEndianness(l_size);
        }

        if (l_size == sizeof(uint32_t))
        {
            switch (l_type)
            {
                case GNU_PROPERTY_X86_ISA_1_USED:
                    {
                        auto l_x86InstructionSet { buildGnuPropertyX86InstructionSet(l_currentOffset) };
                        l_x86InstructionSet.isHardwareSupportRequired = false;
                        l_gnuProperty.emplace_back(std::make_shared<GnuProperty<GnuPropertyX86InstructionSet>>(l_x86InstructionSet));
                    }
                    break;
                case GNU_PROPERTY_X86_ISA_1_NEEDED:
                    {
                        auto l_x86InstructionSet { buildGnuPropertyX86InstructionSet(l_currentOffset) };
                        l_x86InstructionSet.isHardwareSupportRequired = true;
                        l_gnuProperty.emplace_back(std::make_shared<GnuProperty<GnuPropertyX86InstructionSet>>(l_x86InstructionSet));
                    }
                    break;
                case GNU_PROPERTY_X86_FEATURE_1_AND:
                    {
                        l_gnuProperty.emplace_back(std::make_shared<GnuProperty<GnuPropertyX86Features>>(
                            buildGnuPropertyX86Features(l_currentOffset)));
                    }
                    break;
                default:
                    break;
            }

            l_currentOffset += l_size;

            if (m_targetMachineInfo.bitVersion == ELFCLASS64)
                l_currentOffset = alignOffset(l_currentOffset, 8);
        }
    }

    return l_gnuProperty;
}

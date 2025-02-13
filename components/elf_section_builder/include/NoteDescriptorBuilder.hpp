#pragma once

#include "INoteDescriptorBuilder.hpp"
#include <string>
#include <istream>
#include "TargetMachineInfo.hpp"


struct GnuPropertyX86Features;
struct GnuPropertyX86InstructionSet;


class NoteDescriptorBuilder : public INoteDescriptorBuilder
{
public:
    NoteDescriptorBuilder(std::istream* p_fileStream, TargetMachineInfo p_targetMachineInfo)
        : m_fileStream { p_fileStream }
        , m_targetMachineInfo { p_targetMachineInfo }
    {}

    AbiTagInformation buildAbiTagInformation(int p_offset) override;
    std::vector<unsigned char> buildBuildIdInformation(int p_offset, int p_size) override;
    std::vector<std::shared_ptr<IGnuProperty>> buildGnuPropertyInformation(int p_offset, int p_size) override;

private:
    std::istream* m_fileStream;
    TargetMachineInfo m_targetMachineInfo;

    GnuPropertyX86Features buildGnuPropertyX86Features(int p_offset);
    GnuPropertyX86InstructionSet buildGnuPropertyX86InstructionSet(int p_offset);
};


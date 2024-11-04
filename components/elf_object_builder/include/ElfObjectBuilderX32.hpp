#pragma once

#include "IElfObjectBuilder.hpp"
#include <istream>
#include "TargetMachineInfo.hpp"
#include "ElfObjectX32.hpp"
#include "ElfStructureInfoBuilderX32.hpp"


class ElfObjectBuilderX32 : public IElfObjectBuilder
{
public:
    ElfObjectBuilderX32(std::istream* p_fileStream, ElfStructureInfoBuilderX32 p_elfStructureInfoBuilder, TargetMachineInfo p_targetMachineInfo)
        : m_fileStream { p_fileStream }
        , m_elfStructureInfoBuilder { p_elfStructureInfoBuilder }
        , m_targetMachineInfo { p_targetMachineInfo }
    {}

    void reset() override;
    void buildElfStructureInfo() override;

private:
    std::istream* m_fileStream;
    ElfStructureInfoBuilderX32 m_elfStructureInfoBuilder;
    TargetMachineInfo m_targetMachineInfo;
    ElfObjectX32 m_elfObject;
};

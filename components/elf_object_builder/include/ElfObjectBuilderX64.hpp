#pragma once

#include "IElfObjectBuilder.hpp"
#include <istream>
#include "TargetMachineInfo.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoBuilderX64.hpp"


class ElfObjectBuilderX64 : public IElfObjectBuilder
{
public:
    ElfObjectBuilderX64(std::istream* p_fileStream, ElfStructureInfoBuilderX64 p_elfStructureInfoBuilder, TargetMachineInfo p_targetMachineInfo)
        : m_fileStream { p_fileStream }
        , m_elfStructureInfoBuilder { p_elfStructureInfoBuilder }
        , m_targetMachineInfo { p_targetMachineInfo }
    {}

    void reset() override;
    void buildElfStructureInfo() override;

private:
    std::istream* m_fileStream;
    ElfStructureInfoBuilderX64 m_elfStructureInfoBuilder;
    TargetMachineInfo m_targetMachineInfo;
    ElfObjectX64 m_elfObject;
};


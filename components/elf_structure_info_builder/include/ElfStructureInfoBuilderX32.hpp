#pragma once

#include "IElfStructureInfoBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX32.hpp"
#include <istream>


class ElfStructureInfoBuilderX32 : public IElfStructureInfoBuilder
{
public:
    ElfStructureInfoBuilderX32(std::istream* p_fileStream, int p_targetMachineEndianness)
        : m_elfStructureInfo { new ElfStructureInfoX32() }
        , m_fileStream { p_fileStream }
        , m_targetMachineEndianness { p_targetMachineEndianness }
    {}

    void reset() override;
    void buildFileHeader() override;
    void buildSectionHeaders() override;
    void buildProgramHeaders() override;

    ElfStructureInfoX32* getResult();


private:
    ElfStructureInfoX32* m_elfStructureInfo;
    std::istream* m_fileStream;
    int m_targetMachineEndianness;
};

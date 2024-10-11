#pragma once

#include "IElfStructureInfoBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include "ElfStructureInfoX64.hpp"
#include <istream>


class ElfStructureInfoBuilderX64 : public IElfStructureInfoBuilder
{
public:
    ElfStructureInfoBuilderX64(std::istream* p_fileStream, int p_targetMachineEndianness)
        : m_elfStructureInfo { new ElfStructureInfoX64() }
        , m_fileStream { p_fileStream }
        , m_targetMachineEndianness { p_targetMachineEndianness }
    {}

    void reset() override;
    void buildFileHeader() override;
    void buildSectionHeaders() override;
    void buildProgramHeaders() override;

    ElfStructureInfoX64* getResult();


private:
    ElfStructureInfoX64* m_elfStructureInfo;
    std::istream* m_fileStream;
    int m_targetMachineEndianness;
};

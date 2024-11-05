#pragma once

#include "IElfStructureInfoBuilder.hpp"
#include <istream>
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfStructureInfoTraits.hpp"


template <typename T, typename Traits = elf_structure_info_traits<T>>
class ElfStructureInfoBuilder : public IElfStructureInfoBuilder<T, Traits>
{
public:
    ElfStructureInfoBuilder(std::istream* p_fileStream, int p_targetMachineEndianness)
        : m_elfStructureInfo { new T() }
        , m_fileStream { p_fileStream }
        , m_targetMachineEndianness { p_targetMachineEndianness }
    {}

    void reset() override;
    void buildFileHeader() override;
    void buildSectionHeaders() override;
    void buildProgramHeaders() override;
    T* getResult() override;
private:
    T* m_elfStructureInfo;
    std::istream* m_fileStream;
    int m_targetMachineEndianness;
};


template class ElfStructureInfoBuilder<ElfStructureInfoX32>;
template class ElfStructureInfoBuilder<ElfStructureInfoX64>;

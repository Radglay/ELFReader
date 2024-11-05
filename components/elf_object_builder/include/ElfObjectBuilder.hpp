#pragma once

#include "IElfObjectBuilder.hpp"
#include <istream>
#include "TargetMachineInfo.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include "IElfStructureInfoBuilder.hpp"
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"


template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class ElfObjectBuilder : public IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>
{
public:
    ElfObjectBuilder(std::istream* p_fileStream, 
                     IElfStructureInfoBuilder<U, ElfStructureInfoTraits>& p_elfStructureInfoBuilder,
                     TargetMachineInfo& p_targetMachineInfo)
        : m_fileStream { p_fileStream }
        , m_elfStructureInfoBuilder { p_elfStructureInfoBuilder }
        , m_targetMachineInfo { p_targetMachineInfo }
    {}

    void reset() override;
    void buildElfStructureInfo() override;
    void buildSymbols() override;

private:
    std::istream* m_fileStream;
    IElfStructureInfoBuilder<U, ElfStructureInfoTraits>& m_elfStructureInfoBuilder;
    TargetMachineInfo m_targetMachineInfo;
    T m_elfObject;
};

template class ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32>;
template class ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64>;
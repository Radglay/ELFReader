#pragma once

#include "IElfObjectBuilder.hpp"
#include <istream>
#include "TargetMachineInfo.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"


template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class ElfObjectBuilder : public IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>
{
public:
    ElfObjectBuilder(std::istream* p_fileStream, 
                     TargetMachineInfo& p_targetMachineInfo)
        : m_fileStream { p_fileStream }
        , m_targetMachineInfo { p_targetMachineInfo }
        , m_elfObject { new T{} }
    {}

    void reset() override;
    void buildSymbolSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildNoteSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildRelocationSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildRelocationWithAddendSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildStringTableSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildProgbitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildNobitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    T* getResult() override;

private:
    std::istream* m_fileStream;
    TargetMachineInfo m_targetMachineInfo;
    T* m_elfObject;
};

template class ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32>;
template class ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64>;

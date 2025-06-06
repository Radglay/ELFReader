#pragma once

#include "IElfSectionBuilder.hpp"
#include <istream>
#include "TargetMachineInfo.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"
#include "INoteSection.hpp"
#include "INoteDescriptorBuilder.hpp"
#include <memory>


template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class ElfSectionBuilder : public IElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>
{
public:
    ElfSectionBuilder(std::istream* p_fileStream, 
                      TargetMachineInfo& p_targetMachineInfo,
                      INoteDescriptorBuilder& p_noteDescriptorBuilder)
        : m_fileStream { p_fileStream }
        , m_targetMachineInfo { p_targetMachineInfo }
        , m_noteDescriptorBuilder { p_noteDescriptorBuilder }
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
    void buildNullSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    void buildUnknownSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) override;
    T* getResult() override;

private:
    std::istream* m_fileStream;
    TargetMachineInfo m_targetMachineInfo;
    INoteDescriptorBuilder& m_noteDescriptorBuilder;
    T* m_elfObject;

    std::shared_ptr<INoteSection<typename ElfStructureInfoTraits::section_header_type, typename ElfObjectTraits::note_header_type>> createNoteSection(
        std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>,
        typename ElfObjectTraits::note_header_type&,
        std::string);
};

template class ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32>;
template class ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64>;

#pragma once

#include <memory>
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"



template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class IElfObjectBuilder
{
public:
    virtual void reset() = 0;
    virtual void buildSymbolSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildNoteSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildRelocationSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildRelocationWithAddendSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildStringTableSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildProgbitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual void buildNobitsSection(std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>) = 0;
    virtual T* getResult() = 0;

    virtual ~IElfObjectBuilder() = default;
};

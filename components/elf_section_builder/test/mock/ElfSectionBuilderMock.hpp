#pragma once

#include <gmock/gmock.h>
#include "IElfSectionBuilder.hpp"
#include "ElfObjectTraits.hpp"
#include "ElfStructureInfoTraits.hpp"
#include <memory>


template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
class ElfSectionBuilderMock : public IElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>
{
public:
    MOCK_METHOD(void, reset, (), (override));
    MOCK_METHOD(void, buildSymbolSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildNoteSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildRelocationSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildRelocationWithAddendSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildStringTableSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildProgbitsSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildNobitsSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildNullSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(void, buildUnknownSection, (std::shared_ptr<typename ElfStructureInfoTraits::section_header_type>), (override));
    MOCK_METHOD(T*, getResult, (), (override));
};

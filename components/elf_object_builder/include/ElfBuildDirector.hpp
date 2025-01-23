#pragma once

#include <IElfObjectBuilder.hpp>
#include "IElfStructureInfoBuilder.hpp"
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"


class ElfBuildDirector
{
public:
    template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
    std::unique_ptr<T> makeElfObject(IElfStructureInfoBuilder<U, ElfStructureInfoTraits>&,
                                     IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>&);
};

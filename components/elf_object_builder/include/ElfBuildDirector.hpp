#pragma once

#include <memory>
#include <IElfObjectBuilder.hpp>
#include "ElfStructureInfoTraits.hpp"
#include "ElfObjectTraits.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"


class ElfBuildDirector
{
public:

    template <typename T, typename U, typename ElfStructureInfoTraits = elf_structure_info_traits<U>, typename ElfObjectTraits = elf_object_traits<T>>
    std::unique_ptr<T> makeElfObject(
        IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>& p_elfObjectBuilder);
};


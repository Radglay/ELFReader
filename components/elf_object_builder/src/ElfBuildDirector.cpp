#include "ElfBuildDirector.hpp"
#include <memory>
#include "IElfObjectBuilder.hpp"
#include "ElfObjectBuilder.hpp"



template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
std::unique_ptr<T> ElfBuildDirector::makeElfObject(
    IElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>& p_elfObjectBuilder)
{
    p_elfObjectBuilder.buildElfStructureInfo();
    p_elfObjectBuilder.buildSymbolSections();
    p_elfObjectBuilder.buildNoteSections();
    p_elfObjectBuilder.buildRelocationSections();
    p_elfObjectBuilder.buildRelocationWithAddendSections();
    p_elfObjectBuilder.buildStringTableSections();
    p_elfObjectBuilder.buildProgbitsSections();
    p_elfObjectBuilder.buildNobitsSections();

    return std::make_unique<T>(*p_elfObjectBuilder.getResult());
}

template
std::unique_ptr<ElfObjectX32> ElfBuildDirector::makeElfObject <
    ElfObjectX32,
    ElfStructureInfoX32,
    elf_structure_info_traits<ElfStructureInfoX32>,
    elf_object_traits<ElfObjectX32> >
    (IElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32, elf_structure_info_traits<ElfStructureInfoX32>, elf_object_traits<ElfObjectX32>>&);

template
std::unique_ptr<ElfObjectX64> ElfBuildDirector::makeElfObject <
    ElfObjectX64,
    ElfStructureInfoX64,
    elf_structure_info_traits<ElfStructureInfoX64>,
    elf_object_traits<ElfObjectX64> >
    (IElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64, elf_structure_info_traits<ElfStructureInfoX64>, elf_object_traits<ElfObjectX64>>&);

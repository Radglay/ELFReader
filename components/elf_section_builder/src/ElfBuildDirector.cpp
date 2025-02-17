#include "ElfBuildDirector.hpp"
#include "ElfSectionBuilder.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"


template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
std::unique_ptr<T> ElfBuildDirector::makeElfObject(IElfStructureInfoBuilder<U, ElfStructureInfoTraits>& p_elfStructureInfoBuilder,
                                                   IElfSectionBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>& p_elfObjectBuilder)
{
    p_elfStructureInfoBuilder.buildFileHeader();
    p_elfStructureInfoBuilder.buildSectionHeaders();
    p_elfStructureInfoBuilder.buildProgramHeaders();
    auto l_elfStructureInfo { p_elfStructureInfoBuilder.getResult() };

    const auto& l_sectionHeaders { l_elfStructureInfo->sectionHeaders };

    for (const auto& l_sectionHeader : l_sectionHeaders)
    {
        switch (l_sectionHeader->sh_type)
        {
            case SHT_NULL:
                p_elfObjectBuilder.buildNullSection(l_sectionHeader);
                break;
            case SHT_PROGBITS:
                p_elfObjectBuilder.buildProgbitsSection(l_sectionHeader);
                break;
            case SHT_NOBITS:
                p_elfObjectBuilder.buildNobitsSection(l_sectionHeader);
                break;
            case SHT_SYMTAB:
                p_elfObjectBuilder.buildSymbolSection(l_sectionHeader);
                break;
            case SHT_STRTAB:
                p_elfObjectBuilder.buildStringTableSection(l_sectionHeader);
                break;
            case SHT_RELA:
                p_elfObjectBuilder.buildRelocationWithAddendSection(l_sectionHeader);
                break;
            case SHT_REL:
                p_elfObjectBuilder.buildRelocationSection(l_sectionHeader);
                break;
            case SHT_NOTE:
                p_elfObjectBuilder.buildNoteSection(l_sectionHeader);
                break;
            default:
                p_elfObjectBuilder.buildUnknownSection(l_sectionHeader);
                break;
        }
    }

    auto l_elfObject { p_elfObjectBuilder.getResult() };
    l_elfObject->elfStructureInfo = *l_elfStructureInfo;
    return std::make_unique<T>(*l_elfObject);
}

template
std::unique_ptr<ElfObjectX32> ElfBuildDirector::makeElfObject <
    ElfObjectX32,
    ElfStructureInfoX32,
    elf_structure_info_traits<ElfStructureInfoX32>,
    elf_object_traits<ElfObjectX32> >
    (IElfStructureInfoBuilder<ElfStructureInfoX32, elf_structure_info_traits<ElfStructureInfoX32>>&,
    IElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32, elf_structure_info_traits<ElfStructureInfoX32>, elf_object_traits<ElfObjectX32>>&);

template
std::unique_ptr<ElfObjectX64> ElfBuildDirector::makeElfObject <
    ElfObjectX64,
    ElfStructureInfoX64,
    elf_structure_info_traits<ElfStructureInfoX64>,
    elf_object_traits<ElfObjectX64> >
    (IElfStructureInfoBuilder<ElfStructureInfoX64, elf_structure_info_traits<ElfStructureInfoX64>>&,
     IElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64, elf_structure_info_traits<ElfStructureInfoX64>, elf_object_traits<ElfObjectX64>>&);

#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include <istream>
#include <algorithm>
#include <vector>
#include <elf.h>


namespace
{
    template <typename T>
    T& findSectionHeaderByType(std::vector<T>& p_sectionHeaders, int p_type)
    {
        return *std::find_if(p_sectionHeaders.begin(),
                             p_sectionHeaders.end(),
                             [](auto& l_header) { return l_header.sh_type == SHT_SYMTAB; } );
    }
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::reset()
{
    m_elfStructureInfoBuilder.reset();
    delete m_elfObject;
}

template <typename T, typename U, typename ElfStructureInfoTraits, typename ElfObjectTraits>
void ElfObjectBuilder<T, U, ElfStructureInfoTraits, ElfObjectTraits>::buildElfStructureInfo()
{
    m_elfStructureInfoBuilder.buildFileHeader();
    m_elfStructureInfoBuilder.buildSectionHeaders();
    m_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = m_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}


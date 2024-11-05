#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include <istream>


template <typename T, typename U, typename Traits>
void ElfObjectBuilder<T, U, Traits>::reset()
{}

template <typename T, typename U, typename Traits>
void ElfObjectBuilder<T, U, Traits>::buildElfStructureInfo()
{
    m_elfStructureInfoBuilder.buildFileHeader();
    m_elfStructureInfoBuilder.buildSectionHeaders();
    m_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = m_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}

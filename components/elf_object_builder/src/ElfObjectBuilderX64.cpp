#include "ElfObjectBuilderX64.hpp"
#include "ElfStructureInfoBuilderX64.hpp"
#include <istream>


void ElfObjectBuilderX64::reset()
{}


void ElfObjectBuilderX64::buildElfStructureInfo()
{
    m_elfStructureInfoBuilder.buildFileHeader();
    m_elfStructureInfoBuilder.buildSectionHeaders();
    m_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = m_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}

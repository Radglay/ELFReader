#include "ElfObjectBuilderX32.hpp"
#include "ElfStructureInfoBuilderX32.hpp"
#include <istream>


void ElfObjectBuilderX32::reset()
{}


void ElfObjectBuilderX32::buildElfStructureInfo()
{
    m_elfStructureInfoBuilder.buildFileHeader();
    m_elfStructureInfoBuilder.buildSectionHeaders();
    m_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = m_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}

#include "ElfObjectBuilderX64.hpp"
#include "ElfStructureInfoBuilderX64.hpp"
#include <istream>


void ElfObjectBuilderX64::reset()
{}


void ElfObjectBuilderX64::buildElfStructureInfo()
{
    auto l_elfStructureInfoBuilder = ElfStructureInfoBuilderX64(m_fileStream, m_targetMachineInfo.bitVersion);
    l_elfStructureInfoBuilder.buildFileHeader();
    l_elfStructureInfoBuilder.buildSectionHeaders();
    l_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = l_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}

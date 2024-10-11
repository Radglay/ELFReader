#include "ElfObjectBuilderX32.hpp"
#include "ElfStructureInfoBuilderX32.hpp"
#include <istream>


void ElfObjectBuilderX32::reset()
{}


void ElfObjectBuilderX32::buildElfStructureInfo()
{
    auto l_elfStructureInfoBuilder = ElfStructureInfoBuilderX32(m_fileStream, m_targetMachineInfo.bitVersion);
    l_elfStructureInfoBuilder.buildFileHeader();
    l_elfStructureInfoBuilder.buildSectionHeaders();
    l_elfStructureInfoBuilder.buildProgramHeaders();

    auto l_result = l_elfStructureInfoBuilder.getResult();
    m_elfObject.elfStructureInfo = *l_result;
}

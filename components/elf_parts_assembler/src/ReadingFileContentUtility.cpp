#include "ReadingFileContentUtility.hpp"
#include "ElfPart.hpp"
#include "TargetMachineInfo.hpp"
#include "BytesReadingUtility.hpp"
#include "ElfObjectX32.hpp"
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfBuildDirector.hpp"
#include "ElfStructureInfoBuilder.hpp"
#include "ElfSectionBuilder.hpp"
#include "ElfPartAssembler.hpp"
#include <memory>
#include "IElfSection.hpp"
#include "NoteSection.hpp"
#include "StringTableSection.hpp"
#include "ElfPartFromSectionVisitor.hpp"
#include "NoteDescriptorBuilder.hpp"


namespace
{

template <typename T>
StringTableSection<T>* getStringTableSection(const std::vector<std::shared_ptr<IElfSection<T>>>& p_sections, int p_strTabIndex)
{
    return dynamic_cast<StringTableSection<T>*>(p_sections[p_strTabIndex].get());
}

template <typename ElfObject>
std::vector<ElfPart> assembleElfPartsFromElfObject(ElfObject& p_elfObject)
{
    std::vector<ElfPart> l_elfParts;

    ElfPartAssembler l_elfPartAssembler;

    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor;


    auto l_shstrndx { p_elfObject.elfStructureInfo.fileHeader.e_shstrndx };
    auto l_sectionNamesTable { getStringTableSection(p_elfObject.sections, l_shstrndx) };

    l_elfParts.push_back(l_elfPartAssembler.assembleElfPartFromFileHeader(p_elfObject.elfStructureInfo.fileHeader) );

    auto l_programHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromProgramHeaders(p_elfObject.elfStructureInfo.programHeaders,
                                                                                          p_elfObject.elfStructureInfo.fileHeader.e_phoff) };
    l_elfParts.insert(l_elfParts.end(), l_programHeadersElfParts.begin(), l_programHeadersElfParts.end());

    auto l_sectionsElfParts { l_elfPartAssembler.assembleElfPartsFromSections(p_elfObject.sections, l_sectionNamesTable, l_elfPartFromSectionVisitor) };
    l_elfParts.insert(l_elfParts.end(), l_sectionsElfParts.begin(), l_sectionsElfParts.end());

    auto l_sectionHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(p_elfObject.elfStructureInfo.sectionHeaders,
                                                                                          p_elfObject.elfStructureInfo.fileHeader.e_shoff) };
    l_elfParts.insert(l_elfParts.end(), l_sectionHeadersElfParts.begin(), l_sectionHeadersElfParts.end());

    return l_elfParts;
}

}


std::vector<ElfPart> readElfPartsFromFile(std::istream* p_fileStream)
{
    std::vector<ElfPart> l_elfParts;

    TargetMachineInfo l_targetMachineInfo;
    readBytesFromFile(l_targetMachineInfo, 0, p_fileStream);

    ElfBuildDirector l_elfBuildDirector;
    NoteDescriptorBuilder l_noteDescriptorBuilder { p_fileStream, l_targetMachineInfo };

    if (l_targetMachineInfo.bitVersion == ELFCLASS32)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX32> l_elfStructureInfoBuilder { p_fileStream, l_targetMachineInfo.endianness };
        ElfSectionBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder { p_fileStream, l_targetMachineInfo, l_noteDescriptorBuilder };
    
        auto l_elfObject { l_elfBuildDirector.makeElfObject(l_elfStructureInfoBuilder, l_elfObjectBuilder) };

        l_elfParts = assembleElfPartsFromElfObject(*l_elfObject);
    }
    else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { p_fileStream, l_targetMachineInfo.endianness };
        ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder { p_fileStream, l_targetMachineInfo, l_noteDescriptorBuilder };

        auto l_elfObject { l_elfBuildDirector.makeElfObject(l_elfStructureInfoBuilder, l_elfObjectBuilder) };

        l_elfParts = assembleElfPartsFromElfObject(*l_elfObject);
    }

    return l_elfParts;
}

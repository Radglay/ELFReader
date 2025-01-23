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
#include "ElfObjectBuilder.hpp"
#include "ElfPartAssembler.hpp"
#include <memory>
#include "IElfSection.hpp"
#include "NoteSection.hpp"
#include "StringTableSection.hpp"
#include "ElfPartFromSectionVisitor.hpp"


namespace
{

template <typename T>
StringTableSection<T>* getStrtabSectionWithSectionNames(T& p_strtabSectionHeader,
                                                        std::vector<std::shared_ptr<IElfSection<T>>>& p_sections)
{
    for (const auto& p_section : p_sections)
    {
        if (p_section->getSectionHeader()->sh_offset == p_strtabSectionHeader.sh_offset)
            return dynamic_cast<StringTableSection<T>*>(p_section.get());
    }

    return nullptr;
}

}


std::vector<ElfPart> readElfPartsFromFile(std::istream* p_fileStream)
{
    std::vector<ElfPart> l_elfParts;

    TargetMachineInfo l_targetMachineInfo;
    readBytesFromFile(l_targetMachineInfo, 0, p_fileStream);

    ElfBuildDirector l_elfBuildDirector;

    ElfPartAssembler l_elfPartAssembler;

    ElfPartFromSectionVisitor l_elfPartFromSectionVisitor;

    if (l_targetMachineInfo.bitVersion == ELFCLASS32)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX32> l_elfStructureInfoBuilder { p_fileStream, l_targetMachineInfo.endianness };
        ElfObjectBuilder<ElfObjectX32, ElfStructureInfoX32> l_elfObjectBuilder {
            p_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo };

        auto l_elfObject { l_elfBuildDirector.makeElfObject(l_elfObjectBuilder) };

        auto l_shstrndx { l_elfObject->elfStructureInfo.fileHeader.e_shstrndx };
        auto l_sectionNamesTable { getStrtabSectionWithSectionNames(*l_elfObject->elfStructureInfo.sectionHeaders[l_shstrndx], l_elfObject->sections) };

        l_elfParts.push_back(l_elfPartAssembler.assembleElfPartFromFileHeader(l_elfObject->elfStructureInfo.fileHeader) );

        auto l_programHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromProgramHeaders(l_elfObject->elfStructureInfo.programHeaders,
                                                                                              l_elfObject->elfStructureInfo.fileHeader.e_phoff) };
        l_elfParts.insert(l_elfParts.end(), l_programHeadersElfParts.begin(), l_programHeadersElfParts.end());

        auto l_sectionsElfParts { l_elfPartAssembler.assembleElfPartsFromSections(l_elfObject->sections, l_sectionNamesTable, l_elfPartFromSectionVisitor) };
        l_elfParts.insert(l_elfParts.end(), l_sectionsElfParts.begin(), l_sectionsElfParts.end());

        auto l_sectionHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_elfObject->elfStructureInfo.sectionHeaders,
                                                                                              l_elfObject->elfStructureInfo.fileHeader.e_shoff) };
        l_elfParts.insert(l_elfParts.end(), l_sectionHeadersElfParts.begin(), l_sectionHeadersElfParts.end());
    }
    else if (l_targetMachineInfo.bitVersion == ELFCLASS64)
    {
        ElfStructureInfoBuilder<ElfStructureInfoX64> l_elfStructureInfoBuilder { p_fileStream, l_targetMachineInfo.endianness };
        ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder {
            p_fileStream, l_elfStructureInfoBuilder, l_targetMachineInfo };

        auto l_elfObject { l_elfBuildDirector.makeElfObject(l_elfObjectBuilder) };

        auto l_shstrndx { l_elfObject->elfStructureInfo.fileHeader.e_shstrndx };
        auto l_sectionNamesTable { getStrtabSectionWithSectionNames(*l_elfObject->elfStructureInfo.sectionHeaders[l_shstrndx], l_elfObject->sections) };

        l_elfParts.push_back(l_elfPartAssembler.assembleElfPartFromFileHeader(l_elfObject->elfStructureInfo.fileHeader) );

        auto l_programHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromProgramHeaders(l_elfObject->elfStructureInfo.programHeaders,
                                                                                              l_elfObject->elfStructureInfo.fileHeader.e_phoff) };
        l_elfParts.insert(l_elfParts.end(), l_programHeadersElfParts.begin(), l_programHeadersElfParts.end());

        auto l_sectionsElfParts { l_elfPartAssembler.assembleElfPartsFromSections(l_elfObject->sections, l_sectionNamesTable, l_elfPartFromSectionVisitor) };
        l_elfParts.insert(l_elfParts.end(), l_sectionsElfParts.begin(), l_sectionsElfParts.end());

        auto l_sectionHeadersElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_elfObject->elfStructureInfo.sectionHeaders,
                                                                                              l_elfObject->elfStructureInfo.fileHeader.e_shoff) };
        l_elfParts.insert(l_elfParts.end(), l_sectionHeadersElfParts.begin(), l_sectionHeadersElfParts.end());
    }

    return l_elfParts;
}

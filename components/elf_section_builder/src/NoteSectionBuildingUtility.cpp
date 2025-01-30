#include "NoteSectionBuildingUtility.hpp"
#include "NoteSection.hpp"
#include <elf.h>
#include "AbiTagInformation.hpp"
#include "BytesReadingUtility.hpp"
#include "EndiannessUtility.hpp"


namespace
{

template <typename SectionHeader, typename NoteHeader>
AbiTagInformation readAbiTagInformation(std::istream* p_fileStream, int p_targetMachineEndianness, std::shared_ptr<SectionHeader> p_sectionHeader, NoteHeader& p_noteHeader)
{
    auto l_offset { p_sectionHeader->sh_offset + sizeof(NoteHeader) + p_noteHeader.n_namesz};
    AbiTagInformation l_abiTagInformation {};
    readBytesFromFile(l_abiTagInformation, l_offset, p_fileStream);

    if (isEndiannessCorrect(p_targetMachineEndianness)
        and shouldConvertEndianness(p_targetMachineEndianness))
    {
        convertEndianness(l_abiTagInformation.osDescriptor);
        convertEndianness(l_abiTagInformation.majorVersion);
        convertEndianness(l_abiTagInformation.minorVersion);
        convertEndianness(l_abiTagInformation.subminorVersion);
    }

    return l_abiTagInformation;
}

}


template <typename SectionHeader, typename NoteHeader>
std::shared_ptr<INoteSection<SectionHeader, NoteHeader>> createNoteSection(std::istream* p_fileStream,
                                                                           int p_targetMachineEndianness,
                                                                           std::shared_ptr<SectionHeader> p_sectionHeader, 
                                                                           NoteHeader& p_noteHeader,
                                                                           std::string p_namespace)
{
    if (p_namespace == "GNU")
    {
        switch (p_noteHeader.n_type)
        {
            case NT_GNU_ABI_TAG:
                return std::make_shared<NoteSection<SectionHeader, NoteHeader, AbiTagInformation>>(
                    p_sectionHeader, p_noteHeader, p_namespace, readAbiTagInformation(p_fileStream, p_targetMachineEndianness, p_sectionHeader, p_noteHeader));
            // case NT_GNU_HWCAP:
            //     return ()
            // case NT_GNU_BUILD_ID:
            //     return ();
            // case NT_GNU_GOLD_VERSION:
            //     return ();
            // case NT_GNU_PROPERTY_TYPE_0:
            //     return ();
        }
    }
}

template
std::shared_ptr<INoteSection<Elf32_Shdr, Elf32_Nhdr>> createNoteSection<Elf32_Shdr, Elf32_Nhdr>(std::istream*, int, std::shared_ptr<Elf32_Shdr>, Elf32_Nhdr&, std::string);

template
std::shared_ptr<INoteSection<Elf64_Shdr, Elf64_Nhdr>> createNoteSection<Elf64_Shdr, Elf64_Nhdr>(std::istream*, int, std::shared_ptr<Elf64_Shdr>, Elf64_Nhdr&, std::string);


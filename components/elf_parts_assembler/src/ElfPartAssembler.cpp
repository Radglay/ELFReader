#include "ElfPartAssembler.hpp"
#include "ElfPart.hpp"
#include "NoteSection.hpp"
#include "NobitsSection.hpp"
#include "ProgbitsSection.hpp"
#include "RelocationSection.hpp"
#include "RelocationWithAddendSection.hpp"
#include "StringTableSection.hpp"
#include "SymbolTableSection.hpp"
#include <elf.h>
#include <QString>
#include <vector>
#include "FieldDescriptionUtility.hpp"


namespace
{
    QString getBinaryNumberString(uint64_t p_number, int p_length)
    {
        return QString("0b") + QStringLiteral("%1").arg(p_number, p_length, 2, QLatin1Char('0'));
    }

    QString getHexNumberString(uint64_t p_number)
    {
        return QString("0x") + QString::number(p_number, 16).toUpper();
    }

    QString getDecimalNumberString(uint64_t p_number)
    {
        return QString::number(p_number, 10);
    }

    const QString  FILE_HEADER_DESCRIPTION {
        "File Header provides information about the architecture on which the file was created and information necessary for parsing the whole file parts."
        "The architecture information part contains for example endianness and procesor architecture and bit version."
        "Information required during parsing of the other elf parts such as offsets to the appropriate header table and number of entries is provided in the second part of the file header."
    };

    const QString SECTION_HEADER_DESCRIPTION {
        "Section Header contains information required during parsing of the Sections. Their type, offset in the file, total size and number of entries or linked sections."
    };

    const QString PROGRAM_HEADER_DESCRIPTION {
        "Program Header contains information necessary for creating the process image. Each segment of the process image is described by the type, memory access flags, its offset and total size."
    };

    const QString SHT_NOTE_DESCRIPTION {
        "NOTE section is used to provide system, compiler or linker specific information."
    };

    ElfField createIdentField(unsigned char(&p_ident)[EI_NIDENT])
    {
        std::vector<ElfField> l_identFields;
        l_identFields.reserve(EI_NIDENT);

        bool l_isMagicNumberCorrect = (p_ident[EI_MAG0] != ELFMAG0) ? true : false;
        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_MAG0], 16).toUpper(), "Magic number byte 0: " + QString(l_isMagicNumberCorrect ? "correct" : "incorrect"));

        l_isMagicNumberCorrect = (p_ident[EI_MAG1] != ELFMAG1) ? true : false;
        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_MAG1], 16).toUpper(), "Magic number byte 1: " + QString(l_isMagicNumberCorrect ? "correct" : "incorrect"));

        l_isMagicNumberCorrect = (p_ident[EI_MAG2] != ELFMAG2) ? true : false;
        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_MAG2], 16).toUpper(), "Magic number byte 2: " + QString(l_isMagicNumberCorrect ? "correct" : "incorrect"));

        l_isMagicNumberCorrect = (p_ident[EI_MAG3] != ELFMAG3) ? true : false;
        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_MAG3], 16).toUpper(), "Magic number byte 3: " + QString(l_isMagicNumberCorrect ? "correct" : "incorrect"));


        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_CLASS], 16).toUpper(), "Bit Version: " + getBitVersionHighLevelValue(p_ident[EI_CLASS]));
        

        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_DATA], 16).toUpper(), "Endianness: " + getEndiannessHighLevelValue(p_ident[EI_DATA]));


        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_OSABI], 16).toUpper(), "ABI: " + getABIHighLevelValue(p_ident[EI_OSABI]));


        l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[EI_ABIVERSION], 16).toUpper(), "ABI Version: " + getABIVersionHighLevelValue(p_ident[EI_ABIVERSION]));

        for (int i = EI_PAD; i < EI_NIDENT; ++i)
            l_identFields.emplace_back("", "unsigned char", QString::number(p_ident[i], 16).toUpper(), "padding");

        return {"e_ident", "unsigned char[]", "", "", l_identFields};
    }
}

ElfPart ElfPartAssembler::assembleElfPartFromFileHeader(Elf32_Ehdr& p_fileHeader)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back(createIdentField(p_fileHeader.e_ident));
    l_fields.emplace_back("e_type", "Elf32_Half", QString::number(p_fileHeader.e_type, 16).toUpper(), "Object file type: " + getObjectTypeHighLevelValue(p_fileHeader.e_type));
    l_fields.emplace_back("e_machine", "Elf32_Half", QString::number(p_fileHeader.e_machine, 16).toUpper(), "Instruction set architecture: " + getInstructionSetArchitectureHighLevelValue(p_fileHeader.e_machine));
    l_fields.emplace_back("e_version", "Elf32_Word", QString::number(p_fileHeader.e_version, 16).toUpper(),  "ELF version: " + getELFVersionHighLevelValue(p_fileHeader.e_version));
    l_fields.emplace_back("e_entry", "Elf32_Addr", QString::number(p_fileHeader.e_entry, 16).toUpper(), "");
    l_fields.emplace_back("e_phoff", "Elf32_Off", QString::number(p_fileHeader.e_phoff, 16).toUpper(), "");
    l_fields.emplace_back("e_shoff", "Elf32_Off", QString::number(p_fileHeader.e_shoff, 16).toUpper(), "");
    l_fields.emplace_back("e_flags", "Elf32_Word", QString::number(p_fileHeader.e_flags, 2), "");
    l_fields.emplace_back("e_ehsize", "Elf32_Half", QString::number(p_fileHeader.e_ehsize, 10).toUpper(), "");
    l_fields.emplace_back("e_phentsize", "Elf32_Half", QString::number(p_fileHeader.e_phentsize, 10).toUpper(), "");
    l_fields.emplace_back("e_phnum", "Elf32_Half", QString::number(p_fileHeader.e_phnum, 10).toUpper(), "");
    l_fields.emplace_back("e_shentsize", "Elf32_Half", QString::number(p_fileHeader.e_shentsize, 10).toUpper(), "");
    l_fields.emplace_back("e_shnum", "Elf32_Half", QString::number(p_fileHeader.e_shnum, 10).toUpper(), "");
    l_fields.emplace_back("e_shstrndx", "Elf32_Half", QString::number(p_fileHeader.e_shstrndx, 10).toUpper(), "");

    return ElfPart{ {}, ElfPartType::FileHeader, 0, sizeof(Elf32_Ehdr), FILE_HEADER_DESCRIPTION, l_fields };
}

ElfPart ElfPartAssembler::assembleElfPartFromFileHeader(Elf64_Ehdr& p_fileHeader)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back(createIdentField(p_fileHeader.e_ident));
    l_fields.emplace_back("e_type", "Elf64_Half", QString::number(p_fileHeader.e_type, 16).toUpper(), "Object file type: " + getObjectTypeHighLevelValue(p_fileHeader.e_type));
    l_fields.emplace_back("e_machine", "Elf64_Half", QString::number(p_fileHeader.e_machine, 16).toUpper(), "Instruction set architecture: " + getInstructionSetArchitectureHighLevelValue(p_fileHeader.e_machine));
    l_fields.emplace_back("e_version", "Elf64_Word", QString::number(p_fileHeader.e_version, 16).toUpper(), "ELF version: " + getELFVersionHighLevelValue(p_fileHeader.e_version));
    l_fields.emplace_back("e_entry", "Elf64_Addr", QString::number(p_fileHeader.e_entry, 16).toUpper(), "");
    l_fields.emplace_back("e_phoff", "Elf64_Off", QString::number(p_fileHeader.e_phoff, 16).toUpper(), "");
    l_fields.emplace_back("e_shoff", "Elf64_Off", QString::number(p_fileHeader.e_shoff, 16).toUpper(), "");
    l_fields.emplace_back("e_flags", "Elf64_Word", QString::number(p_fileHeader.e_flags, 16).toUpper(), "");
    l_fields.emplace_back("e_ehsize", "Elf64_Half", QString::number(p_fileHeader.e_ehsize, 16).toUpper(), "");
    l_fields.emplace_back("e_phentsize", "Elf64_Half", QString::number(p_fileHeader.e_phentsize, 16).toUpper(), "");
    l_fields.emplace_back("e_phnum", "Elf64_Half", QString::number(p_fileHeader.e_phnum, 16).toUpper(), "");
    l_fields.emplace_back("e_shentsize", "Elf64_Half", QString::number(p_fileHeader.e_shentsize, 16).toUpper(), "");
    l_fields.emplace_back("e_shnum", "Elf64_Half", QString::number(p_fileHeader.e_shnum, 16).toUpper(), "");
    l_fields.emplace_back("e_shstrndx", "Elf64_Half", QString::number(p_fileHeader.e_shstrndx, 16).toUpper(), "");

    return ElfPart{ "FileHeader", ElfPartType::FileHeader, 0, sizeof(Elf64_Ehdr), FILE_HEADER_DESCRIPTION, l_fields };
}

ElfPart ElfPartAssembler::assembleElfPartFromSectionHeader(Elf32_Shdr& p_sectionHeader, int p_sectionHeaderOffset)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back("sh_name", "Elf32_Word", QString::number(p_sectionHeader.sh_name, 16).toUpper(), "");
    l_fields.emplace_back("sh_type", "Elf32_Word", QString::number(p_sectionHeader.sh_type, 16).toUpper(), "");
    l_fields.emplace_back("sh_flags", "Elf32_Word", QString::number(p_sectionHeader.sh_flags, 16).toUpper(), "");
    l_fields.emplace_back("sh_addr", "Elf32_Addr", QString::number(p_sectionHeader.sh_addr, 16).toUpper(), "");
    l_fields.emplace_back("sh_offset", "Elf32_Off", QString::number(p_sectionHeader.sh_offset, 16).toUpper(), "");
    l_fields.emplace_back("sh_size", "Elf32_Word", QString::number(p_sectionHeader.sh_size, 16).toUpper(), "");
    l_fields.emplace_back("sh_link", "Elf32_Word", QString::number(p_sectionHeader.sh_link, 16).toUpper(), "");
    l_fields.emplace_back("sh_info", "Elf32_Word", QString::number(p_sectionHeader.sh_info, 16).toUpper(), "");
    l_fields.emplace_back("sh_addralign", "Elf32_Word", QString::number(p_sectionHeader.sh_addralign, 16).toUpper(), "");
    l_fields.emplace_back("sh_entsize", "Elf32_Word", QString::number(p_sectionHeader.sh_entsize, 16).toUpper(), "");

    return ElfPart{"SectionHeader", ElfPartType::SectionHeader, p_sectionHeaderOffset, sizeof(Elf32_Shdr), SECTION_HEADER_DESCRIPTION, l_fields};
}

ElfPart ElfPartAssembler::assembleElfPartFromSectionHeader(Elf64_Shdr& p_sectionHeader, int p_sectionHeaderOffset)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back("sh_name", "Elf64_Word", QString::number(p_sectionHeader.sh_name, 16).toUpper(), "");
    l_fields.emplace_back("sh_type", "Elf64_Word", QString::number(p_sectionHeader.sh_type, 16).toUpper(), "");
    l_fields.emplace_back("sh_flags", "Elf64_Xword", QString::number(p_sectionHeader.sh_flags, 16).toUpper(), "");
    l_fields.emplace_back("sh_addr", "Elf64_Addr", QString::number(p_sectionHeader.sh_addr, 16).toUpper(), "");
    l_fields.emplace_back("sh_offset", "Elf64_Off", QString::number(p_sectionHeader.sh_offset, 16).toUpper(), "");
    l_fields.emplace_back("sh_size", "Elf64_Xword", QString::number(p_sectionHeader.sh_size, 16).toUpper(), "");
    l_fields.emplace_back("sh_link", "Elf64_Word", QString::number(p_sectionHeader.sh_link, 16).toUpper(), "");
    l_fields.emplace_back("sh_info", "Elf64_Word", QString::number(p_sectionHeader.sh_info, 16).toUpper(), "");
    l_fields.emplace_back("sh_addralign", "Elf64_Xword", QString::number(p_sectionHeader.sh_addralign, 16).toUpper(), "");
    l_fields.emplace_back("sh_entsize", "Elf64_Xword", QString::number(p_sectionHeader.sh_entsize, 16).toUpper(), "");

    return ElfPart{"SectionHeader", ElfPartType::SectionHeader, p_sectionHeaderOffset, sizeof(Elf64_Shdr), SECTION_HEADER_DESCRIPTION, l_fields};
}

ElfPart ElfPartAssembler::assembleElfPartFromProgramHeader(Elf32_Phdr& p_programHeader, int p_programHeaderOffset)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back("p_type", "Elf32_Word", QString::number(p_programHeader.p_type, 16).toUpper(), getSegmentTypeHighLevelValue(p_programHeader.p_type));
    l_fields.emplace_back("p_offset", "Elf32_Off", QString::number(p_programHeader.p_offset, 16).toUpper(), "");
    l_fields.emplace_back("p_vaddr", "Elf32_Addr", QString::number(p_programHeader.p_vaddr, 16).toUpper(), "");
    l_fields.emplace_back("p_paddr", "Elf32_Addr", QString::number(p_programHeader.p_paddr, 16).toUpper(), "");
    l_fields.emplace_back("p_filesz", "Elf32_Word", QString::number(p_programHeader.p_filesz, 16).toUpper(), "");
    l_fields.emplace_back("p_memsz", "Elf32_Word", QString::number(p_programHeader.p_memsz, 16).toUpper(), "");

    auto l_binaryFormatFlags { QStringLiteral("%1").arg(p_programHeader.p_flags, 4, 2, QLatin1Char('0')) };
    l_fields.emplace_back("p_flags", "Elf32_Word", l_binaryFormatFlags, getSegmentFlagsHighLevelValues(p_programHeader.p_flags));
    l_fields.emplace_back("p_align", "Elf32_Word", QString::number(p_programHeader.p_align, 16).toUpper(), "");

    return ElfPart{"ProgramHeader", ElfPartType::ProgramHeader, p_programHeaderOffset, sizeof(Elf32_Phdr), PROGRAM_HEADER_DESCRIPTION, l_fields};
}

ElfPart ElfPartAssembler::assembleElfPartFromProgramHeader(Elf64_Phdr& p_programHeader, int p_programHeaderOffset)
{
    std::vector<ElfField> l_fields;

    l_fields.emplace_back("p_type", "Elf64_Word", QString::number(p_programHeader.p_type, 16).toUpper(), getSegmentTypeHighLevelValue(p_programHeader.p_type));

    auto l_binaryFormatFlags { QStringLiteral("%1").arg(p_programHeader.p_flags, 4, 2, QLatin1Char('0')) };
    l_fields.emplace_back("p_flags", "Elf64_Word", l_binaryFormatFlags, getSegmentFlagsHighLevelValues(p_programHeader.p_flags));
    l_fields.emplace_back("p_offset", "Elf64_Off", QString::number(p_programHeader.p_offset, 16).toUpper(), "");
    l_fields.emplace_back("p_vaddr", "Elf64_Addr", QString::number(p_programHeader.p_vaddr, 16).toUpper(), "");
    l_fields.emplace_back("p_paddr", "Elf64_Addr", QString::number(p_programHeader.p_paddr, 16).toUpper(), "");
    l_fields.emplace_back("p_filesz", "Elf64_Xword", QString::number(p_programHeader.p_filesz, 16).toUpper(), "");
    l_fields.emplace_back("p_memsz", "Elf64_Xword", QString::number(p_programHeader.p_memsz, 16).toUpper(), "");
    l_fields.emplace_back("p_align", "Elf64_Xword", QString::number(p_programHeader.p_align, 16).toUpper(), "");

    return ElfPart{"ProgramHeader", ElfPartType::ProgramHeader, p_programHeaderOffset, sizeof(Elf64_Phdr), PROGRAM_HEADER_DESCRIPTION, l_fields};
}


ElfPart ElfPartAssembler::assembleElfPartFromSection(NoteSection<Elf32_Shdr, Elf32_Nhdr>& p_noteSection, const std::string& p_sectionName)
{
    auto l_noteHeader { p_noteSection.getNoteHeader() };

    auto l_offset { static_cast<int>(p_noteSection.getSectionHeader()->sh_offset) };

    std::vector<ElfField> l_fields;

    l_fields.emplace_back("n_namesz", "Elf32_Word", QString::number(l_noteHeader.n_namesz, 16).toUpper(), "");
    l_fields.emplace_back("n_descsz", "Elf32_Word", QString::number(l_noteHeader.n_descsz, 16).toUpper(), "");
    l_fields.emplace_back("n_type", "Elf32_Word", QString::number(l_noteHeader.n_type, 16).toUpper(), "");

    return ElfPart{QString::fromStdString(p_sectionName), ElfPartType::Section, l_offset, sizeof(Elf32_Nhdr), "", l_fields};
}

ElfPart ElfPartAssembler::assembleElfPartFromSection(NoteSection<Elf64_Shdr, Elf64_Nhdr>& p_noteSection, const std::string& p_sectionName)
{
    auto l_noteHeader { p_noteSection.getNoteHeader() };

    auto l_offset { static_cast<int>(p_noteSection.getSectionHeader()->sh_offset) };

    std::vector<ElfField> l_fields;

    l_fields.emplace_back("n_namesz", "Elf64_Word", QString::number(l_noteHeader.n_namesz, 16).toUpper(), "");
    l_fields.emplace_back("n_descsz", "Elf64_Word", QString::number(l_noteHeader.n_descsz, 16).toUpper(), "");
    l_fields.emplace_back("n_type", "Elf64_Word", QString::number(l_noteHeader.n_type, 16).toUpper(), "");

    return ElfPart{QString::fromStdString(p_sectionName), ElfPartType::Section, l_offset, sizeof(Elf64_Nhdr), "", l_fields};
}

ElfPart ElfPartAssembler::assembleElfPartFromSection(NobitsSection<Elf32_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(NobitsSection<Elf64_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(ProgbitsSection<Elf32_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(ProgbitsSection<Elf64_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(RelocationSection<Elf32_Shdr, Elf32_Rel>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(RelocationSection<Elf64_Shdr, Elf32_Rel>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(StringTableSection<Elf32_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(StringTableSection<Elf64_Shdr>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(SymbolTableSection<Elf32_Shdr, Elf32_Sym>&, const std::string&)
{}

ElfPart ElfPartAssembler::assembleElfPartFromSection(SymbolTableSection<Elf64_Shdr, Elf64_Sym>&, const std::string&)
{}

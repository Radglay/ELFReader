#include "FieldDescriptionUtility.hpp"
#include <elf.h>
#include <QString>


QString getBitVersionHighLevelValue(int p_ident)
{
    switch (p_ident)
    {
        case ELFCLASS32:
            return "32 Bit Version";
        case ELFCLASS64:
            return "64 Bit Version";
    }
}

QString getEndiannessHighLevelValue(int p_endianness)
{
    switch (p_endianness)
    {
        case ELFDATA2LSB:
            return "Little Endian";
        case ELFDATA2MSB:
            return "Big Endian";
    }
}

QString getELFVersionHighLevelValue(int p_version)
{
    if (p_version == EV_CURRENT)
        return "Original ELF Version";
}

QString getABIHighLevelValue(int p_abi)
{
    switch (p_abi)
    {
        case ELFOSABI_SYSV:
            return "UNIX System V ABI";
    }
}

QString getABIVersionHighLevelValue(int p_version)
{
    return QString::number(p_version);
}

QString getObjectTypeHighLevelValue(int p_type)
{
    switch (p_type)
    {
        case ET_REL:
            return "Relocatable file";
        case ET_EXEC:
            return "Executable file";
        case ET_DYN:
            return "Shared object file";
        case ET_CORE:
            return "Core file";
    }
}

QString getInstructionSetArchitectureHighLevelValue(int p_architecture)
{
    switch (p_architecture)
    {
        case EM_X86_64:
            return "AMD x86-64 architecture";
    }
}



QString getSegmentTypeHighLevelValue(int p_type)
{
    switch (p_type)
    {
        case PT_NULL:
            return "Program header table entry unused";
        case PT_LOAD:
            return "Loadable program segment";
        case PT_DYNAMIC:
            return "Dynamic linking information";
        case PT_INTERP:
            return "Program interpreter";
        case PT_NOTE:
            return "Auxiliary information";
        case PT_SHLIB:
            return "Reserved";
        case PT_PHDR:
            return "Entry for header table itself";
        case PT_TLS:
            return "Thread-local storage segment";
        case PT_NUM:
            return "Number of defined types";
        default:
            return "Unsupported segment type";
    }
}

QString getSegmentFlagsHighLevelValues(uint32_t p_flags)
{
    QStringList l_flagsValues;

    if (p_flags & PF_X)
        l_flagsValues.append("0x1 Executable");
    
    if (p_flags & PF_W)
        l_flagsValues.append("0x2 Writeable");

    if (p_flags & PF_R)
        l_flagsValues.append("0x4 Readable");

    return l_flagsValues.join('\n');
}


QString getSectionTypeHighLevelValue(int p_type)
{
    switch (p_type)
    {
        case SHT_NULL:
            return "Section header table entry unused";
        case SHT_PROGBITS:
            return "Program data";
        case SHT_SYMTAB:
            return "Symbol table";
        case SHT_STRTAB:
            return "String table";
        case SHT_RELA:
            return "Relocation entries with addends";
        case SHT_HASH:
            return "Symbol hash table";
        case SHT_DYNAMIC:
            return "Dynamic linking information";
        case SHT_NOTE:
            return "Notes";
        case SHT_NOBITS:
            return "Program space with no data (bss)";
        case SHT_REL:
            return "Relocation entries, no addends";
        case SHT_SHLIB:
            return "Reserved";
        case SHT_DYNSYM:
            return "Dynamic linker symbol table";
        case SHT_INIT_ARRAY:
            return "Array of constructors";
        case SHT_FINI_ARRAY:
            return "Array of destructors";
        case SHT_PREINIT_ARRAY:
            return "Array of pre-constructors";
        case SHT_GROUP:
            return "Section group";
        case SHT_SYMTAB_SHNDX:
            return "Extended section indices";
        default:
            return "Unsupported section type";
    }
        // case SHT_LOOS:	  0x60000000	/* Start OS-specific.  */
        // case SHT_GNU_ATTRIBUTES: 0x6ffffff5	/* Object attributes.  */
        // case SHT_GNU_HASH:	  0x6ffffff6	/* GNU-style hash table.  */
        // case SHT_GNU_LIBLIST:	  0x6ffffff7	/* Prelink library list */
        // case SHT_CHECKSUM:	  0x6ffffff8	/* Checksum for DSO content.  */
        // case SHT_LOSUNW:	  0x6ffffffa	/* Sun-specific low bound.  */
        // case SHT_SUNW_move:	  0x6ffffffa
        // case SHT_SUNW_COMDAT:   0x6ffffffb
        // case SHT_SUNW_syminfo:  0x6ffffffc
        // case SHT_GNU_verdef:	  0x6ffffffd	/* Version definition section.  */
        // case SHT_GNU_verneed:	  0x6ffffffe	/* Version needs section.  */
        // case SHT_GNU_versym:	  0x6fffffff	/* Version symbol table.  */
        // case SHT_HISUNW:	  0x6fffffff	/* Sun-specific high bound.  */
        // case SHT_HIOS:	  0x6fffffff	/* End OS-specific type */
        // case SHT_LOPROC:	  0x70000000	/* Start of processor-specific */
        // case SHT_HIPROC:	  0x7fffffff	/* End of processor-specific */
        // case SHT_LOUSER:	  0x80000000	/* Start of application-specific */
        // case SHT_HIUSER:	  0x8fffffff	/* End of application-specific */
}

QString getSectionAtrributesHighLevelValues(uint64_t p_attributes)
{
    QStringList l_attributesValues;

    if (p_attributes & SHF_WRITE)
        l_attributesValues.append("Writeable");
    if (p_attributes & SHF_ALLOC)
        l_attributesValues.append("Occupies memory during execution");
    if (p_attributes & SHF_EXECINSTR)
        l_attributesValues.append("Executable");
    if (p_attributes & SHF_MERGE)
        l_attributesValues.append("Might be merged");
    if (p_attributes & SHF_STRINGS)
        l_attributesValues.append("Contains nul-terminated strings");
    if (p_attributes & SHF_INFO_LINK)
        l_attributesValues.append("\"sh_info\" contains SHT index");
    if (p_attributes & SHF_LINK_ORDER)
        l_attributesValues.append("Preserve order after combining");
    if (p_attributes & SHF_OS_NONCONFORMING)
        l_attributesValues.append("Non-standard OS specific handling required");
    if (p_attributes & SHF_GROUP)
        l_attributesValues.append("Section is member of a group");
    if (p_attributes & SHF_TLS)
        l_attributesValues.append("Section hold thread-local data");
    if (p_attributes & SHF_COMPRESSED)
        l_attributesValues.append("Section with compressed data");
    if (p_attributes & SHF_MASKOS)
        l_attributesValues.append("OS-specific");
    if (p_attributes & SHF_MASKPROC)
        l_attributesValues.append("Processor-specific");
    if (p_attributes & SHF_GNU_RETAIN)
        l_attributesValues.append("Not to be GCed by linker");
    if (p_attributes & SHF_ORDERED)
        l_attributesValues.append("Special ordering requirement (Solaris)");
    if (p_attributes & SHF_EXCLUDE)
        l_attributesValues.append("Section is excluded unless referenced or allocated (Solaris)");

    return l_attributesValues.join("\n");
}

#include "FieldDescriptionUtility.hpp"
#include <elf.h>
#include <QString>


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
    return getDecimalNumberString(p_version);
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
        // case SHT_SYMTAB_SHNDX:
        //     return "Extended section indices";
    }
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

QString getSectionInfoHighLevelValue(uint32_t p_sectionType, uint32_t p_info)
{
    switch (p_sectionType)
    {
        case SHT_REL:
        case SHT_RELA:
            return "Section header index to which the relocation applies: " + getDecimalNumberString(p_info);
        case SHT_SYMTAB:
        case SHT_DYNSYM:
            return "One greater than the last symbol table index of the local symbol (binding STB_LOCAL): " + getDecimalNumberString(p_info);
        case SHT_GROUP:
            return "The index in the associated symbol table. Symbol's name is the signature for the section group: " + getDecimalNumberString(p_info);
        default:
            return "";
    }
}

QString getSectionLinkHighLevelValue(uint32_t p_sectionType, uint32_t p_link)
{
    switch (p_sectionType)
    {
        case SHT_DYNAMIC:
            return "The section header index of the associated string table: " + getDecimalNumberString(p_link);
        case SHT_HASH:
            return "The section header index of the associated symbol table: " + getDecimalNumberString(p_link);
        case SHT_REL:
        case SHT_RELA:
            return "The section header index of the associated symbol table: " + getDecimalNumberString(p_link);
        case SHT_SYMTAB:
        case SHT_DYNSYM:
            return "The section header index of the associated string table: " + getDecimalNumberString(p_link);
        case SHT_GROUP:
            return "The section header index of the associated symbol table: " + getDecimalNumberString(p_link);
        default:
            return "";
    }
}

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
        default:
            return "Unsupported Bit Version";
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
        default:
            return "Unsupported Endianness";
    }
}

QString getELFVersionHighLevelValue(int p_version)
{
    if (p_version == EV_CURRENT)
        return "Original Elf Version";
    return "Unsupported Elf Version";
}

QString getABIHighLevelValue(int p_abi)
{
    switch (p_abi)
    {
        case ELFOSABI_SYSV:
            return "UNIX System V ABI";
        case ELFOSABI_HPUX:
            return "HP-UX";
        case ELFOSABI_NETBSD:
            return "NetBSD";
        case ELFOSABI_GNU:
            return "GNU ELF extensions";
        case ELFOSABI_SOLARIS:
            return "Sun Solaris";
        case ELFOSABI_AIX:
            return "IBM AIX";
        case ELFOSABI_IRIX:
            return "SGI Irix";
        case ELFOSABI_FREEBSD:
            return "FreeBSD";
        case ELFOSABI_TRU64:
            return "Compaq TRU64 UNIX";
        case ELFOSABI_MODESTO:
            return "Novell Modesto";
        case ELFOSABI_OPENBSD:
            return "OpenBSD";
        case ELFOSABI_ARM_AEABI:
            return "ARM EABI";
        case ELFOSABI_ARM:
            return "ARM";
        case ELFOSABI_STANDALONE:
            return "Standalone (embedded) application";
        default:
            return "Unsupported ABI";
    }
}

QString getABIVersionHighLevelValue(int p_version)
{
    return QString::number(p_version);
}

QString getObjectTypeHighLevelValue(int p_type)
{
    if (p_type >= ET_LOOS and p_type <= ET_HIOS)
        return "OS-specific";
    else if (p_type >= ET_LOPROC and p_type <= ET_HIPROC)
        return "Processor-specific";

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
        default:
            return "Unsupported File Type";
    }
}

QString getInstructionSetArchitectureHighLevelValue(int p_architecture)
{
    switch (p_architecture)
    {
        case EM_M32:
            return "AT&T WE 32100";
        case EM_SPARC:
            return "SUN SPARC";
        case EM_386:
            return "Intel 80386";
        case EM_68K:
            return "Motorola m68k family";
        case EM_88K:
            return "Motorola m88k family";
        case EM_IAMCU:
            return "Intel MCU";
        case EM_860:
            return "Intel 80860";
        case EM_MIPS:
            return "MIPS R3000 big-endian";
        case EM_S370:
            return "IBM System/370";
        case EM_MIPS_RS3_LE:
            return "MIPS R3000 little-endian";
        case EM_PARISC:
            return "HPPA";
        case EM_VPP500:
            return "Fujitsu VPP500";
        case EM_SPARC32PLUS:
            return "Sun's \"v8plus\"";
        case EM_960:
            return "Intel 80960";
        case EM_PPC:
            return "PowerPC";
        case EM_PPC64:
            return "PowerPC 64-bit";
        case EM_S390:
            return "IBM S390";
        case EM_SPU:
            return "IBM SPU/SPC";
        case EM_V800:
            return "NEC V800 series";
        case EM_FR20:
            return "Fujitsu FR20";
        case EM_RH32:
            return "TRW RH-32";
        case EM_RCE:
            return "Motorola RCE";
        case EM_ARM:
            return "ARM";
        case EM_FAKE_ALPHA:
            return "Digital Alpha";
        case EM_SH:
            return "Hitachi SH";
        case EM_SPARCV9:
            return "SPARC v9 64-bit";
        case EM_TRICORE:
            return "Siemens Tricore";
        case EM_ARC:
            return "Argonaut RISC Core";
        case EM_H8_300:
            return "Hitachi H8/300";
        case EM_H8_300H:
            return "Hitachi H8/300H";
        case EM_H8S:
            return "Hitachi H8S";
        case EM_H8_500:
            return "Hitachi H8/500";
        case EM_IA_64:
            return "Intel Merced";
        case EM_MIPS_X:
            return "Stanford MIPS-X";
        case EM_COLDFIRE:
            return "Motorola Coldfire";
        case EM_68HC12:
            return "Motorola M68HC12";
        case EM_MMA:
            return "Fujitsu MMA Multimedia Accelerator";
        case EM_PCP:
            return "Siemens PCP";
        case EM_NCPU:
            return "Sony nCPU embeeded RISC";
        case EM_NDR1:
            return "Denso NDR1 microprocessor";
        case EM_STARCORE:
            return "Motorola Start*Core processor";
        case EM_ME16:
            return "Toyota ME16 processor";
        case EM_ST100:
            return "STMicroelectronic ST100 processor";
        case EM_TINYJ:
            return "Advanced Logic Corp. Tinyj emb.fam";
        case EM_X86_64:
            return "AMD x86-64 architecture";
        case EM_PDSP:
            return "Sony DSP Processor";
        case EM_PDP10:
            return "Digital PDP-10";
        case EM_PDP11:
            return "Digital PDP-11";
        case EM_FX66:
            return "Siemens FX66 microcontroller";
        case EM_ST9PLUS:
            return "STMicroelectronics ST9+ 8/16 mc";
        case EM_ST7:
            return "STmicroelectronics ST7 8 bit mc";
        case EM_68HC16:
            return "Motorola MC68HC16 microcontroller";
        case EM_68HC11:
            return "Motorola MC68HC11 microcontroller";
        case EM_68HC08:
            return "Motorola MC68HC08 microcontroller";
        case EM_68HC05:
            return "Motorola MC68HC05 microcontroller";
        case EM_SVX:
            return "Silicon Graphics SVx";
        case EM_ST19:
            return "STMicroelectronics ST19 8 bit mc";
        case EM_VAX:
            return "Digital VAX";
        case EM_CRIS:
            return "Axis Communications 32-bit emb.proc";
        case EM_JAVELIN:
            return "Infineon Technologies 32-bit emb.proc";
        case EM_FIREPATH:
            return "Element 14 64-bit DSP Processor";
        case EM_ZSP:
            return "LSI Logic 16-bit DSP Processor";
        case EM_MMIX:
            return "Donald Knuth's educational 64-bit proc";
        case EM_HUANY:
            return "Harvard University machine-independent object files";
        case EM_PRISM:
            return "SiTera Prism";
        case EM_AVR:
            return "Atmel AVR 8-bit microcontroller";
        case EM_FR30:
            return "Fujitsu FR30";
        case EM_D10V:
            return "Mitsubishi D10V";
        case EM_D30V:
            return "Mitsubishi D30V";
        case EM_V850:
            return "NEC v850";
        case EM_M32R:
            return "Mitsubishi M32R";
        case EM_MN10300:
            return "Matsushita MN10300";
        case EM_MN10200:
            return "Matsushita MN10200";
        case EM_PJ:
            return "picoJava";
        case EM_OPENRISC:
            return "OpenRISC 32-bit embedded processor";
        case EM_ARC_COMPACT:
            return "ARC International ARCompact";
        case EM_XTENSA:
            return "Tensilica Xtensa Architecture";
        case EM_VIDEOCORE:
            return "Alphamosaic VideoCore";
        case EM_TMM_GPP:
            return "Thompson Multimedia General Purpose Proc";
        case EM_NS32K:
            return "National Semi. 32000";
        case EM_TPC:
            return "Tenor Network TPC";
        case EM_SNP1K:
            return "Trebia SNP 1000";
        case EM_ST200:
            return "STMicroelectronics ST200";
        case EM_IP2K:
            return "Ubicom IP2xxx";
        case EM_MAX:
            return "MAX processor";
        case EM_CR:
            return "National Semi. CompactRISC";
        case EM_F2MC16:
            return "Fujitsu F2MC16";
        case EM_MSP430:
            return "Texas Instruments msp430";
        case EM_BLACKFIN:
            return "Analog Devices Blackfin DSP";
        case EM_SE_C33:
            return "Seiko Epson S1C33 family";
        case EM_SEP:
            return "Sharp embedded microprocessor";
        case EM_ARCA:
            return "Arca RISC";
        case EM_UNICORE:
            return "PKU-Unity & MPRC Peking Uni. mc series";
        case EM_EXCESS:
            return "eXcess configurable cpu";
        case EM_DXP:
            return "Icera Semi. Deep Execution Processor";
        case EM_ALTERA_NIOS2:
            return "Altera Nios II";
        case EM_CRX:
            return "National Semi. CompactRISC CRX";
        case EM_XGATE:
            return "Motorola XGATE";
        case EM_C166:
            return "Infineon C16x/XC16x";
        case EM_M16C:
            return "Renesas M16C";
        case EM_DSPIC30F:
            return "Microchip Technology dsPIC30F";
        case EM_CE:
            return "Freescale Communication Engine RISC";
        case EM_M32C:
            return "Renesas M32C";
        case EM_TSK3000:
            return "Altium TSK3000";
        case EM_RS08:
            return "Freescale RS08";
        case EM_SHARC:
            return "Analog Devices SHARC family";
        case EM_ECOG2:
            return "Cyan Technology eCOG2";
        case EM_SCORE7:
            return "Sunplus S+core7 RISC";
        case EM_DSP24:
            return "New Japan Radio (NJR) 24-bit DSP";
        case EM_VIDEOCORE3:
            return "Broadcom VideoCore III";
        case EM_LATTICEMICO32:
            return "RISC for Lattice FPGA";
        case EM_SE_C17:
            return "Seiko Epson C17";
        case EM_TI_C6000:
            return "Texas Instruments TMS320C6000 DSP";
        case EM_TI_C2000:
            return "Texas Instruments TMS320C2000 DSP";
        case EM_TI_C5500:
            return "Texas Instruments TMS320C55x DSP";
        case EM_TI_ARP32:
            return "Texas Instruments App. Specific RISC";
        case EM_TI_PRU:
            return "Texas Instruments Prog. Realtime Unit";
        case EM_MMDSP_PLUS:
            return "STMicroelectronics 64bit VLIW DSP";
        case EM_CYPRESS_M8C:
            return "Cypress M8C";
        case EM_R32C:
            return "Renesas R32C";
        case EM_TRIMEDIA:
            return "NXP Semi. TriMedia";
        case EM_QDSP6:
            return "QUALCOMM DSP6";
        case EM_8051:
            return "Intel 8051 and variants";
        case EM_STXP7X:
            return "STMicroelectronics STxP7x";
        case EM_NDS32:
            return "Andes Tech. compact code emb. RISC";
        case EM_ECOG1X:
            return "Cyan Technology eCOG1X";
        case EM_MAXQ30:
            return "Dallas Semi. MAXQ30 mc";
        case EM_XIMO16:
            return "New Japan Radio (NJR) 16-bit DSP";
        case EM_MANIK:
            return "M2000 Reconfigurable RISC";
        case EM_CRAYNV2:
            return "Cray NV2 vector architecture";
        case EM_RX:
            return "Renesas RX";
        case EM_METAG:
            return "Imagination Tech. META";
        case EM_MCST_ELBRUS:
            return "MCST Elbrus";
        case EM_ECOG16:
            return "Cyan Technology eCOG16";
        case EM_CR16:
            return "National Semi. CompactRISC CR16";
        case EM_ETPU:
            return "Freescale Extended Time Processing Unit";
        case EM_SLE9X:
            return "Infineon Tech. SLE9X";
        case EM_L10M:
            return "Intel L10M";
        case EM_K10M:
            return "Intel K10M";
        case EM_AARCH64:
            return "ARM AARCH64";
        case EM_AVR32:
            return "Amtel 32-bit microprocessor";
        case EM_STM8:
            return "STMicroelectronics STM8";
        case EM_TILE64:
            return "Tilera TILE64";
        case EM_TILEPRO:
            return "Tilera TILEPro";
        case EM_MICROBLAZE:
            return "Xilinx MicroBlaze";
        case EM_CUDA:
            return "NVIDIA CUDA";
        case EM_TILEGX:
            return "Tilera TILE-Gx";
        case EM_CLOUDSHIELD:
            return "CloudShield";
        case EM_COREA_1ST:
            return "KIPO-KAIST Core-A 1st gen.";
        case EM_COREA_2ND:
            return "KIPO-KAIST Core-A 2nd gen.";
        case EM_ARCV2:
            return "Synopsys ARCv2 ISA. ";
        case EM_OPEN8:
            return "Open8 RISC";
        case EM_RL78:
            return "Renesas RL78";
        case EM_VIDEOCORE5:
            return "Broadcom VideoCore V";
        case EM_78KOR:
            return "Renesas 78KOR";
        case EM_56800EX:
            return "Freescale 56800EX DSC";
        case EM_BA1:
            return "Beyond BA1";
        case EM_BA2:
            return "Beyond BA2";
        case EM_XCORE:
            return "XMOS xCORE";
        case EM_MCHP_PIC:
            return "Microchip 8-bit PIC(r)";
        case EM_INTELGT:
            return "Intel Graphics Technology";
        case EM_KM32:
            return "KM211 KM32";
        case EM_KMX32:
            return "KM211 KMX32";
        case EM_EMX16:
            return "KM211 KMX16";
        case EM_EMX8:
            return "KM211 KMX8";
        case EM_KVARC:
            return "KM211 KVARC";
        case EM_CDP:
            return "Paneve CDP";
        case EM_COGE:
            return "Cognitive Smart Memory Processor";
        case EM_COOL:
            return "Bluechip CoolEngine";
        case EM_NORC:
            return "Nanoradio Optimized RISC";
        case EM_CSR_KALIMBA:
            return "CSR Kalimba";
        case EM_Z80:
            return "Zilog Z80";
        case EM_VISIUM:
            return "Controls and Data Services VISIUMcore";
        case EM_FT32:
            return "FTDI Chip FT32";
        case EM_MOXIE:
            return "Moxie processor";
        case EM_AMDGPU:
            return "AMD GPU";
        case EM_RISCV:
            return "RISC-V";
        case EM_BPF:
            return "Linux BPF -- in-kernel virtual machine";
        case EM_CSKY:
            return "C-SKY";
        default:
            return "Unsupported Architecture";
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
// #define PT_LOOS		0x60000000	/* Start of OS-specific */
// #define PT_GNU_EH_FRAME	0x6474e550	/* GCC .eh_frame_hdr segment */
// #define PT_GNU_STACK	0x6474e551	/* Indicates stack executability */
// #define PT_GNU_RELRO	0x6474e552	/* Read-only after relocation */
// #define PT_GNU_PROPERTY	0x6474e553	/* GNU property */
// #define PT_LOSUNW	0x6ffffffa
// #define PT_SUNWBSS	0x6ffffffa	/* Sun Specific segment */
// #define PT_SUNWSTACK	0x6ffffffb	/* Stack segment */
// #define PT_HISUNW	0x6fffffff
// #define PT_HIOS		0x6fffffff	/* End of OS-specific */
// #define PT_LOPROC	0x70000000	/* Start of processor-specific */
// #define PT_HIPROC	0x7fffffff	/* End of processor-specific */
//     }
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
    // #define PF_MASKOS	0x0ff00000	/* OS-specific */
    // #define PF_MASKPROC	0xf0000000	/* Processor-specific */
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

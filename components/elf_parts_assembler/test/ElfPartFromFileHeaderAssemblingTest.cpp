#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "ElfPartAssembler.hpp"
#include "ElfPart.hpp"
#include <elf.h>
#include <stdint.h>
#include <QString>
#include <vector>


namespace
{
    constexpr unsigned char FIRST_MAGIC_NUMBER_VALUE { 0x7f };
    const QString FIRST_MAGIC_NUMBER_VALUE_TEXT { "0x7F" };
    const QString FIRST_MAGIC_NUMBER_DESCRIPTION_TEXT { "Magic number byte 0" };

    constexpr unsigned char SECOND_MAGIC_NUMBER_VALUE { 0x45 };
    const QString SECOND_MAGIC_NUMBER_VALUE_TEXT { "0x45" };
    const QString SECOND_MAGIC_NUMBER_DESCRIPTION_TEXT { "Magic number byte 1" };

    constexpr unsigned char THIRD_MAGIC_NUMBER_VALUE { 0x4c };
    const QString THIRD_MAGIC_NUMBER_VALUE_TEXT { "0x4C" };
    const QString THIRD_MAGIC_NUMBER_DESCRIPTION_TEXT { "Magic number byte 2" };

    constexpr unsigned char FOURTH_MAGIC_NUMBER_VALUE { 0x46 };
    const QString FOURTH_MAGIC_NUMBER_VALUE_TEXT { "0x46" };
    const QString FOURTH_MAGIC_NUMBER_DESCRIPTION_TEXT { "Magic number byte 3" };

    constexpr unsigned char BIT_VERSION_32_VALUE { 1 };
    const QString BIT_VERSION_32_VALUE_TEXT { "1" };
    const QString BIT_VERSION_32_DESCRIPTION_TEXT { "Bit Version: 32 Bit Version" };

    constexpr unsigned char BIT_VERSION_64_VALUE { 2 };
    const QString BIT_VERSION_64_VALUE_TEXT { "2" };
    const QString BIT_VERSION_64_DESCRIPTION_TEXT { "Bit Version: 64 Bit Version" };

    constexpr unsigned char LITTLE_ENDIAN_VALUE { 1 };
    const QString LITTLE_ENDIAN_VALUE_TEXT { "1" };
    const QString LITTLE_ENDIAN_DESCRIPTION_TEXT { "Endianness: Little Endian" };

    constexpr unsigned char IDENT_ELF_VERSION_VALUE { 1 };
    const QString IDENT_ELF_VERSION_VALUE_TEXT { "1" };
    const QString IDENT_ELF_VERSION_DESCRIPTION_TEXT { "ELF Version: Original ELF Version" };

    constexpr unsigned char ABI_VALUE {};
    const QString ABI_VALUE_TEXT { "0" };
    const QString ABI_DESCRIPTION_TEXT { "ABI: UNIX System V ABI" };

    constexpr unsigned char ABI_VERSION_VALUE { 1 };
    const QString ABI_VERSION_VALUE_TEXT { "1" };
    const QString ABI_VERSION_DESCRIPTION_TEXT { "ABI Version: 1" };

    constexpr unsigned char PADDING_VALUE {};
    const QString PADDING_VALUE_TEXT { "0" };
    const QString PADDING_DESCRIPTION_TEXT { "Padding" };

    constexpr uint16_t OBJECT_FILE_TYPE_VALUE { 3 };
    const QString OBJECT_FILE_TYPE_VALUE_TEXT { "3" };
    const QString OBJECT_FILE_TYPE_DESCRIPTION_TEXT { "Object file type: Shared object file" };

    constexpr uint16_t INSTRUCTION_SET_ARCHITECTURE_VALUE { 62 };
    const QString INSTRUCTION_SET_ARCHITECTURE_VALUE_TEXT { "62" };
    const QString INSTRUCTION_SET_ARCHITECTURE_DESCRIPTION_TEXT { "Instruction set architecture: AMD x86-64 architecture" };

    constexpr uint32_t ELF_VERSION_VALUE {1 };
    const QString ELF_VERSION_VALUE_TEXT { "1" };
    const QString ELF_VERSION_DESCRIPTION_TEXT { "ELF Version: 1" };

    constexpr Elf32_Addr ENTRY_ADDRESS_32_VALUE { 0x10a0 };
    const QString ENTRY_ADDRESS_32_VALUE_TEXT { "0x10A0" };
    const QString ENTRY_ADDRESS_32_DESCRIPTION_TEXT { "Program entry point: 0x10A0" };

    constexpr Elf64_Addr ENTRY_ADDRESS_64_VALUE { 0x20a0 };
    const QString ENTRY_ADDRESS_64_VALUE_TEXT { "0x20A0" };
    const QString ENTRY_ADDRESS_64_DESCRIPTION_TEXT { "Program entry point: 0x20A0" };

    constexpr uint32_t FLAGS_VALUE { 0x0 };
    const QString FLAGS_VALUE_TEXT { "0x0" };
    // const QString FLAGS_DESCRIPTION_TEXT {};

    constexpr uint16_t FILE_HEADER_SIZE_32_VALUE { 52 };
    const QString FILE_HEADER_SIZE_32_VALUE_TEXT { "52" };
    const QString FILE_HEADER_SIZE_32_DESCRIPTION_TEXT { "File header size: 52 bytes" };

    constexpr uint16_t FILE_HEADER_SIZE_64_VALUE { 64 };
    const QString FILE_HEADER_SIZE_64_VALUE_TEXT { "64" };
    const QString FILE_HEADER_SIZE_64_DESCRIPTION_TEXT { "File header size: 64 bytes" };

    constexpr Elf32_Off PROGRAM_HEADER_TABLE_OFFSET_32_VALUE { FILE_HEADER_SIZE_32_VALUE };
    const QString PROGRAM_HEADER_TABLE_OFFSET_32_VALUE_TEXT { "0x34" };
    const QString PROGRAM_HEADER_TABLE_OFFSET_32_DESCRIPTION_TEXT { "Program Header Table offset: 0x34" };

    constexpr Elf64_Off PROGRAM_HEADER_TABLE_OFFSET_64_VALUE { FILE_HEADER_SIZE_64_VALUE };
    const QString PROGRAM_HEADER_TABLE_OFFSET_64_VALUE_TEXT { "0x40" };
    const QString PROGRAM_HEADER_TABLE_OFFSET_64_DESCRIPTION_TEXT { "Program Header Table offset: 0x40" };

    constexpr uint16_t PROGRAM_HEADER_ENTRIES_VALUE { 5 };
    const QString PROGRAM_HEADER_ENTRIES_VALUE_TEXT { "5" };
    const QString PROGRAM_HEADER_ENTRIES_DESCRIPTION_TEXT { "Program Header Table entries: 5" };
    
    constexpr uint16_t SECTION_HEADER_ENTRIES_VALUE { 10 };
    const QString SECTION_HEADER_ENTRIES_VALUE_TEXT { "10"};
    const QString SECTION_HEADER_ENTRIES_DESCRIPTION_TEXT { "Section Header Table entries: 10" };

    constexpr uint16_t PROGRAM_HEADER_SIZE_32_VALUE { 32 };
    const QString PROGRAM_HEADER_SIZE_32_VALUE_TEXT { "32" };
    const QString PROGRAM_HEADER_SIZE_32_DESCRIPTION_TEXT { "Program Header Table entry size: 32 bytes" };

    constexpr uint16_t PROGRAM_HEADER_SIZE_64_VALUE { 56 };
    const QString PROGRAM_HEADER_SIZE_64_VALUE_TEXT { "56" };
    const QString PROGRAM_HEADER_SIZE_64_DESCRIPTION_TEXT { "Program Header Table entry size: 56 bytes" };

    constexpr Elf32_Off SECTION_HEADER_TABLE_OFFSET_32_VALUE { 0x2000 };
    const QString SECTION_HEADER_TABLE_OFFSET_32_VALUE_TEXT { "0x2000" };
    const QString SECTION_HEADER_TABLE_OFFSET_32_DESCRIPTION_TEXT { "Section Header Table offset: 0x2000" };

    constexpr Elf64_Off SECTION_HEADER_TABLE_OFFSET_64_VALUE { 0x3000 };
    const QString SECTION_HEADER_TABLE_OFFSET_64_VALUE_TEXT { "0x3000" };
    const QString SECTION_HEADER_TABLE_OFFSET_64_DESCRIPTION_TEXT { "Section Header Table offset: 0x3000" };

    constexpr uint16_t SECTION_HEADER_SIZE_32_VALUE { 40 };
    const QString SECTION_HEADER_SIZE_32_VALUE_TEXT { "40" };
    const QString SECTION_HEADER_SIZE_32_DESCRIPTION_TEXT { "Section Header Table entry size: 40 bytes" };

    constexpr uint16_t SECTION_HEADER_SIZE_64_VALUE { 64 };
    const QString SECTION_HEADER_SIZE_64_VALUE_TEXT { "64" };
    const QString SECTION_HEADER_SIZE_64_DESCRIPTION_TEXT { "Section Header Table entry size: 64 bytes" };

    constexpr uint16_t SECTION_NAMES_SECTION_INDEX_VALUE { 9 };
    const QString SECTION_NAMES_SECTION_INDEX_VALUE_TEXT { "9" };
    const QString SECTION_NAMES_SECTION_INDEX_DESCRIPTION_TEXT { "Index of the section containing section names: 9" };


    const QString E_IDENT_NAME_TEXT { "e_ident" };
    const QString E_TYPE_NAME_TEXT { "e_type" };
    const QString E_MACHINE_NAME_TEXT { "e_machine" };
    const QString E_VERSION_NAME_TEXT { "e_version" };
    const QString E_ENTRY_NAME_TEXT { "e_entry" };
    const QString E_PHOFF_NAME_TEXT { "e_phoff" };
    const QString E_SHOFF_NAME_TEXT { "e_shoff" };
    const QString E_FLAGS_NAME_TEXT { "e_flags" };
    const QString E_EHSIZE_NAME_TEXT { "e_ehsize" };
    const QString E_PHENTSIZE_NAME_TEXT { "e_phentsize" };
    const QString E_PHNUM_NAME_TEXT { "e_phnum" };
    const QString E_SHENTSIZE_NAME_TEXT { "e_shentsize" };
    const QString E_SHNUM_NAME_TEXT { "e_shnum" };
    const QString E_SHSTRNDX_NAME_TEXT { "e_shstrndx" };

    const QString EMPTY_TEXT {};

    const QString FILE_HEADER_ELF_PART_NAME { "FileHeader" };
    const ElfPartType FILE_HEADER_ELF_PART_TYPE { ElfPartType::FileHeader };
    constexpr int FILE_HEADER_ELF_PART_OFFSET {};
    constexpr int FILE_HEADER_ELF_PART_X32_SIZE { FILE_HEADER_SIZE_32_VALUE };
    constexpr int FILE_HEADER_ELF_PART_X64_SIZE { FILE_HEADER_SIZE_64_VALUE };
    const QString FILE_HEADER_ELF_PART_DESCRIPTION {
        "File Header provides information about the architecture on which the file was created and information necessary for parsing the whole file parts."
        "The architecture information part contains for example endianness and procesor architecture and bit version."
        "Information required during parsing of the other elf parts such as offsets to the appropriate header table and number of entries is provided in the second part of the file header."
    };
    
    ElfField generateExpectedIdentField(const QString p_bitVersionValueText, const QString p_bitVersionDescriptionText) {
        std::vector<ElfField> l_subFields {
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, FIRST_MAGIC_NUMBER_VALUE_TEXT, FIRST_MAGIC_NUMBER_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, SECOND_MAGIC_NUMBER_VALUE_TEXT, SECOND_MAGIC_NUMBER_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, THIRD_MAGIC_NUMBER_VALUE_TEXT, THIRD_MAGIC_NUMBER_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, FOURTH_MAGIC_NUMBER_VALUE_TEXT, FOURTH_MAGIC_NUMBER_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, p_bitVersionValueText, p_bitVersionDescriptionText),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, LITTLE_ENDIAN_VALUE_TEXT, LITTLE_ENDIAN_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, IDENT_ELF_VERSION_VALUE_TEXT, IDENT_ELF_VERSION_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, ABI_VALUE_TEXT, ABI_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, ABI_VERSION_VALUE_TEXT, ABI_VERSION_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT),
            ElfField(EMPTY_TEXT, UNSIGNED_CHAR_TYPE_TEXT, PADDING_VALUE_TEXT, PADDING_DESCRIPTION_TEXT)
        };

        return {E_IDENT_NAME_TEXT, UNSIGNED_CHAR_ARRAY_TYPE_TEXT, EMPTY_TEXT, EMPTY_TEXT, l_subFields};
    };

    std::vector<ElfField> generate32BitFileHeaderElfFields()
    {
        return std::vector<ElfField> {
            generateExpectedIdentField(BIT_VERSION_32_VALUE_TEXT, BIT_VERSION_32_DESCRIPTION_TEXT),
            {E_TYPE_NAME_TEXT, ELF32_HALF_TEXT, OBJECT_FILE_TYPE_VALUE_TEXT, OBJECT_FILE_TYPE_DESCRIPTION_TEXT},
            {E_MACHINE_NAME_TEXT, ELF32_HALF_TEXT, INSTRUCTION_SET_ARCHITECTURE_VALUE_TEXT, INSTRUCTION_SET_ARCHITECTURE_DESCRIPTION_TEXT},
            {E_VERSION_NAME_TEXT, ELF32_WORD_TEXT, ELF_VERSION_VALUE_TEXT, ELF_VERSION_DESCRIPTION_TEXT},
            {E_ENTRY_NAME_TEXT, ELF32_ADDR_TEXT, ENTRY_ADDRESS_32_VALUE_TEXT, ENTRY_ADDRESS_32_DESCRIPTION_TEXT},
            {E_PHOFF_NAME_TEXT, ELF32_OFF_TEXT, PROGRAM_HEADER_TABLE_OFFSET_32_VALUE_TEXT, PROGRAM_HEADER_TABLE_OFFSET_32_DESCRIPTION_TEXT},
            {E_SHOFF_NAME_TEXT, ELF32_OFF_TEXT, SECTION_HEADER_TABLE_OFFSET_32_VALUE_TEXT, SECTION_HEADER_TABLE_OFFSET_32_DESCRIPTION_TEXT},
            {E_FLAGS_NAME_TEXT, ELF32_WORD_TEXT, FLAGS_VALUE_TEXT, EMPTY_TEXT},
            {E_EHSIZE_NAME_TEXT, ELF32_HALF_TEXT, FILE_HEADER_SIZE_32_VALUE_TEXT, FILE_HEADER_SIZE_32_DESCRIPTION_TEXT},
            {E_PHENTSIZE_NAME_TEXT, ELF32_HALF_TEXT, PROGRAM_HEADER_SIZE_32_VALUE_TEXT, PROGRAM_HEADER_SIZE_32_DESCRIPTION_TEXT},
            {E_PHNUM_NAME_TEXT, ELF32_HALF_TEXT, PROGRAM_HEADER_ENTRIES_VALUE_TEXT, PROGRAM_HEADER_ENTRIES_DESCRIPTION_TEXT},
            {E_SHENTSIZE_NAME_TEXT, ELF32_HALF_TEXT, SECTION_HEADER_SIZE_32_VALUE_TEXT, SECTION_HEADER_SIZE_32_DESCRIPTION_TEXT},
            {E_SHNUM_NAME_TEXT, ELF32_HALF_TEXT, SECTION_HEADER_ENTRIES_VALUE_TEXT, SECTION_HEADER_ENTRIES_DESCRIPTION_TEXT},
            {E_SHSTRNDX_NAME_TEXT, ELF32_HALF_TEXT, SECTION_NAMES_SECTION_INDEX_VALUE_TEXT, SECTION_NAMES_SECTION_INDEX_DESCRIPTION_TEXT}
        };
    }

    std::vector<ElfField> generate64BitFileHeaderElfFields()
    {
        return std::vector<ElfField> {
            generateExpectedIdentField(BIT_VERSION_64_VALUE_TEXT, BIT_VERSION_64_DESCRIPTION_TEXT),
            {E_TYPE_NAME_TEXT, ELF64_HALF_TEXT, OBJECT_FILE_TYPE_VALUE_TEXT, OBJECT_FILE_TYPE_DESCRIPTION_TEXT},
            {E_MACHINE_NAME_TEXT, ELF64_HALF_TEXT, INSTRUCTION_SET_ARCHITECTURE_VALUE_TEXT, INSTRUCTION_SET_ARCHITECTURE_DESCRIPTION_TEXT},
            {E_VERSION_NAME_TEXT, ELF64_WORD_TEXT, ELF_VERSION_VALUE_TEXT, ELF_VERSION_DESCRIPTION_TEXT},
            {E_ENTRY_NAME_TEXT, ELF64_ADDR_TEXT, ENTRY_ADDRESS_64_VALUE_TEXT, ENTRY_ADDRESS_64_DESCRIPTION_TEXT},
            {E_PHOFF_NAME_TEXT, ELF64_OFF_TEXT, PROGRAM_HEADER_TABLE_OFFSET_64_VALUE_TEXT, PROGRAM_HEADER_TABLE_OFFSET_64_DESCRIPTION_TEXT},
            {E_SHOFF_NAME_TEXT, ELF64_OFF_TEXT, SECTION_HEADER_TABLE_OFFSET_64_VALUE_TEXT, SECTION_HEADER_TABLE_OFFSET_64_DESCRIPTION_TEXT},
            {E_FLAGS_NAME_TEXT, ELF64_WORD_TEXT, FLAGS_VALUE_TEXT, EMPTY_TEXT},
            {E_EHSIZE_NAME_TEXT, ELF64_HALF_TEXT, FILE_HEADER_SIZE_64_VALUE_TEXT, FILE_HEADER_SIZE_64_DESCRIPTION_TEXT},
            {E_PHENTSIZE_NAME_TEXT, ELF64_HALF_TEXT, PROGRAM_HEADER_SIZE_64_VALUE_TEXT, PROGRAM_HEADER_SIZE_64_DESCRIPTION_TEXT},
            {E_PHNUM_NAME_TEXT, ELF64_HALF_TEXT, PROGRAM_HEADER_ENTRIES_VALUE_TEXT, PROGRAM_HEADER_ENTRIES_DESCRIPTION_TEXT},
            {E_SHENTSIZE_NAME_TEXT, ELF64_HALF_TEXT, SECTION_HEADER_SIZE_64_VALUE_TEXT, SECTION_HEADER_SIZE_64_DESCRIPTION_TEXT},
            {E_SHNUM_NAME_TEXT, ELF64_HALF_TEXT, SECTION_HEADER_ENTRIES_VALUE_TEXT, SECTION_HEADER_ENTRIES_DESCRIPTION_TEXT},
            {E_SHSTRNDX_NAME_TEXT, ELF64_HALF_TEXT, SECTION_NAMES_SECTION_INDEX_VALUE_TEXT, SECTION_NAMES_SECTION_INDEX_DESCRIPTION_TEXT}
        };
    }
}

using namespace ::testing;


struct ElfPartAssemblerHeaderFileAssemblingX32TestSuite : public ElfPartAssemblerTestSuite
{
    Elf32_Ehdr m_fileHeader {
        .e_ident { FIRST_MAGIC_NUMBER_VALUE, SECOND_MAGIC_NUMBER_VALUE, THIRD_MAGIC_NUMBER_VALUE, FOURTH_MAGIC_NUMBER_VALUE,
                   BIT_VERSION_32_VALUE,
                   LITTLE_ENDIAN_VALUE,
                   IDENT_ELF_VERSION_VALUE,
                   ABI_VALUE,
                   ABI_VERSION_VALUE
                },
        .e_type = OBJECT_FILE_TYPE_VALUE,
        .e_machine = INSTRUCTION_SET_ARCHITECTURE_VALUE,
        .e_version = ELF_VERSION_VALUE,
        .e_entry = ENTRY_ADDRESS_32_VALUE,
        .e_phoff = PROGRAM_HEADER_TABLE_OFFSET_32_VALUE,
        .e_shoff = SECTION_HEADER_TABLE_OFFSET_32_VALUE,
        .e_flags = FLAGS_VALUE,
        .e_ehsize = FILE_HEADER_SIZE_32_VALUE,
        .e_phentsize = PROGRAM_HEADER_SIZE_32_VALUE,
        .e_phnum = PROGRAM_HEADER_ENTRIES_VALUE,
        .e_shentsize = SECTION_HEADER_SIZE_32_VALUE,
        .e_shnum = SECTION_HEADER_ENTRIES_VALUE,
        .e_shstrndx = SECTION_NAMES_SECTION_INDEX_VALUE
    };
};

TEST_F(ElfPartAssemblerHeaderFileAssemblingX32TestSuite, shouldAssembleCorrectElfPartFrom32BitFileHeader)
{
    auto l_targetElfPart { m_elfPartAssembler.assembleElfPartFromFileHeader(m_fileHeader) };

    ElfPart l_expectedElfPart {
        FILE_HEADER_ELF_PART_NAME,
        FILE_HEADER_ELF_PART_TYPE,
        FILE_HEADER_ELF_PART_OFFSET,
        FILE_HEADER_ELF_PART_X32_SIZE,
        FILE_HEADER_ELF_PART_DESCRIPTION,
        generate32BitFileHeaderElfFields()
    };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}


struct ElfPartAssemblerHeaderFileAssemblingX64TestSuite : public ElfPartAssemblerTestSuite
{
    Elf64_Ehdr m_fileHeader {
        .e_ident { FIRST_MAGIC_NUMBER_VALUE, SECOND_MAGIC_NUMBER_VALUE, THIRD_MAGIC_NUMBER_VALUE, FOURTH_MAGIC_NUMBER_VALUE,
                   BIT_VERSION_64_VALUE,
                   LITTLE_ENDIAN_VALUE,
                   IDENT_ELF_VERSION_VALUE,
                   ABI_VALUE,
                   ABI_VERSION_VALUE
                },
        .e_type = OBJECT_FILE_TYPE_VALUE,
        .e_machine = INSTRUCTION_SET_ARCHITECTURE_VALUE,
        .e_version = ELF_VERSION_VALUE,
        .e_entry = ENTRY_ADDRESS_64_VALUE,
        .e_phoff = PROGRAM_HEADER_TABLE_OFFSET_64_VALUE,
        .e_shoff = SECTION_HEADER_TABLE_OFFSET_64_VALUE,
        .e_flags = FLAGS_VALUE,
        .e_ehsize = FILE_HEADER_SIZE_64_VALUE,
        .e_phentsize = PROGRAM_HEADER_SIZE_64_VALUE,
        .e_phnum = PROGRAM_HEADER_ENTRIES_VALUE,
        .e_shentsize = SECTION_HEADER_SIZE_64_VALUE,
        .e_shnum = SECTION_HEADER_ENTRIES_VALUE,
        .e_shstrndx = SECTION_NAMES_SECTION_INDEX_VALUE
    };
};

TEST_F(ElfPartAssemblerHeaderFileAssemblingX64TestSuite, shouldAssembleCorrectElfPartFrom64BitFileHeader)
{
    auto l_targetElfPart { m_elfPartAssembler.assembleElfPartFromFileHeader(m_fileHeader) };
    ElfPart l_expectedElfPart {
        FILE_HEADER_ELF_PART_NAME,
        FILE_HEADER_ELF_PART_TYPE,
        FILE_HEADER_ELF_PART_OFFSET,
        FILE_HEADER_ELF_PART_X64_SIZE,
        FILE_HEADER_ELF_PART_DESCRIPTION,
        generate64BitFileHeaderElfFields()
    };

    expectElfPartsAreEqual(l_targetElfPart, l_expectedElfPart);
}


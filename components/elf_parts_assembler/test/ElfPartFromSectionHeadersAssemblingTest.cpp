#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfPartTestingUtility.hpp"
#include "ElfPartAssembler.hpp"
#include <elf.h>
#include <memory>
#include <vector>
#include <cstdint>
#include "ElfPart.hpp"


namespace
{
    constexpr uint16_t FILE_HEADER_SIZE_32_VALUE { 52 };
    constexpr uint16_t FILE_HEADER_SIZE_64_VALUE { 64 };

    constexpr uint16_t SECTION_HEADER_SIZE_32_VALUE { 40 };
    constexpr uint16_t SECTION_HEADER_SIZE_64_VALUE { 64 };

    constexpr Elf32_Off SECTION_HEADER_TABLE_OFFSET_32_VALUE { 0x2000 };
    constexpr Elf64_Off SECTION_HEADER_TABLE_OFFSET_64_VALUE { 0x3000 };

    const QString SH_NAME_NAME_TEXT { "sh_name" };
    const QString SH_TYPE_NAME_TEXT { "sh_type" };
    const QString SH_FLAGS_NAME_TEXT { "sh_flags" };
    const QString SH_ADDR_NAME_TEXT { "sh_addr" };
    const QString SH_OFFSET_NAME_TEXT { "sh_offset" };
    const QString SH_SIZE_NAME_TEXT { "sh_size" };
    const QString SH_LINK_NAME_TEXT { "sh_link" };
    const QString SH_INFO_NAME_TEXT { "sh_info" };
    const QString SH_ADDRALIGN_NAME_TEXT { "sh_addralign" };
    const QString SH_ENTSIZE_NAME_TEXT { "sh_entsize" };

    const QString EMPTY_TEXT {};


    // First section header
    constexpr uint32_t FIRST_SECTION_HEADER_NAME_VALUE { 0x1 };
    const QString FIRST_SECTION_HEADER_NAME_VALUE_TEXT { "0x1" };
    const QString FIRST_SECTION_HEADER_NAME_DESCRIPTION_TEXT {};

    constexpr uint32_t FIRST_SECTION_HEADER_TYPE_VALUE { 1 };
    const QString FIRST_SECTION_HEADER_TYPE_VALUE_TEXT { "1" };
    const QString FIRST_SECTION_HEADER_TYPE_DESCRIPTION_TEXT { "Section type: Program data" };

    constexpr Elf32_Word FIRST_SECTION_HEADER_FLAGS_VALUE { 0x06 };
    const QString FIRST_SECTION_HEADER_FLAGS_VALUE_TEXT { "0x6" };
    const QString FIRST_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT {};

    constexpr Elf32_Addr FIRST_SECTION_HEADER_ADDR_VALUE { 0x1010 };
    const QString FIRST_SECTION_HEADER_ADDR_VALUE_TEXT { "0x1010" };
    const QString FIRST_SECTION_HEADER_ADDR_DESCRIPTION_TEXT { "Section address in the process image: 0x1010" };

    constexpr Elf32_Off FIRST_SECTION_HEADER_OFFSET_VALUE { 0x1010 };
    const QString FIRST_SECTION_HEADER_OFFSET_VALUE_TEXT { "0x1010" };
    const QString FIRST_SECTION_HEADER_OFFSET_DESCRIPTION_TEXT { "Section offset in the file: 0x1010" };

    constexpr Elf32_Word FIRST_SECTION_HEADER_SIZE_32BIT_VALUE { 2468 };
    const QString FIRST_SECTION_HEADER_SIZE_VALUE_32BIT_TEXT { "2468" };
    const QString FIRST_SECTION_HEADER_SIZE_DESCRIPTION_32BIT_TEXT { "Section size: 2468 bytes" };

    constexpr Elf64_Word FIRST_SECTION_HEADER_SIZE_64BIT_VALUE { 3468 };
    const QString FIRST_SECTION_HEADER_SIZE_VALUE_64BIT_TEXT { "3468" };
    const QString FIRST_SECTION_HEADER_SIZE_DESCRIPTION_64BIT_TEXT { "Section size: 3468 bytes" };

    constexpr Elf32_Word FIRST_SECTION_HEADER_LINK_VALUE {};
    const QString FIRST_SECTION_HEADER_LINK_VALUE_TEXT { "0" };
    const QString FIRST_SECTION_HEADER_LINK_DESCRIPTION_TEXT {};

    constexpr Elf32_Word FIRST_SECTION_HEADER_INFO_VALUE {};
    const QString FIRST_SECTION_HEADER_INFO_VALUE_TEXT { "0" };
    const QString FIRST_SECTION_HEADER_INFO_DESCRIPTION_TEXT {};

    constexpr Elf32_Word FIRST_SECTION_HEADER_ADDRALIGN_VALUE { 16 };
    const QString FIRST_SECTION_HEADER_ADDRALIGN_VALUE_TEXT { "16" };
    const QString FIRST_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT { "Address align: 16 bytes" };

    constexpr Elf32_Word FIRST_SECTION_HEADER_ENTSIZE_VALUE {};
    const QString FIRST_SECTION_HEADER_ENTSIZE_VALUE_TEXT { "0" };
    const QString FIRST_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT { "Section contains no entries" };


    // Second section header
    constexpr uint32_t SECOND_SECTION_HEADER_NAME_VALUE { 0x7 };
    const QString SECOND_SECTION_HEADER_NAME_VALUE_TEXT { "0x7" };
    const QString SECOND_SECTION_HEADER_NAME_DESCRIPTION_TEXT {};

    constexpr uint32_t SECOND_SECTION_HEADER_TYPE_VALUE { 2 };
    const QString SECOND_SECTION_HEADER_TYPE_VALUE_TEXT { "2" };
    const QString SECOND_SECTION_HEADER_TYPE_DESCRIPTION_TEXT { "Section type: Symbol table" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_FLAGS_VALUE {};
    const QString SECOND_SECTION_HEADER_FLAGS_VALUE_TEXT { "0x0" };
    const QString SECOND_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT {};

    constexpr Elf32_Addr SECOND_SECTION_HEADER_ADDR_VALUE {};
    const QString SECOND_SECTION_HEADER_ADDR_VALUE_TEXT { "0x0" };
    const QString SECOND_SECTION_HEADER_ADDR_DESCRIPTION_TEXT { "Section not loadable" };

    constexpr Elf32_Off SECOND_SECTION_HEADER_OFFSET_32BIT_VALUE { 0x19B4 };
    const QString SECOND_SECTION_HEADER_OFFSET_VALUE_32BIT_TEXT { "0x19B4" };
    const QString SECOND_SECTION_HEADER_OFFSET_DESCRIPTION_32BIT_TEXT { "Section offset in the file: 0x19B4" };

    constexpr Elf64_Off SECOND_SECTION_HEADER_OFFSET_64BIT_VALUE { 0x1D9C };
    const QString SECOND_SECTION_HEADER_OFFSET_VALUE_64BIT_TEXT { "0x1D9C" };
    const QString SECOND_SECTION_HEADER_OFFSET_DESCRIPTION_64BIT_TEXT { "Section offset in the file: 0x1D9C" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_SIZE_32BIT_VALUE { 480 };
    const QString SECOND_SECTION_HEADER_SIZE_VALUE_32BIT_TEXT { "480" };
    const QString SECOND_SECTION_HEADER_SIZE_DESCRIPTION_32BIT_TEXT { "Section size: 480 bytes" };

    constexpr Elf64_Word SECOND_SECTION_HEADER_SIZE_64BIT_VALUE { 720 };
    const QString SECOND_SECTION_HEADER_SIZE_VALUE_64BIT_TEXT { "720" };
    const QString SECOND_SECTION_HEADER_SIZE_DESCRIPTION_64BIT_TEXT { "Section size: 720 bytes" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_LINK_VALUE { 2 };
    const QString SECOND_SECTION_HEADER_LINK_VALUE_TEXT { "2" };
    const QString SECOND_SECTION_HEADER_LINK_DESCRIPTION_TEXT { "The section header index of the associated string table: 2" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_INFO_VALUE {};
    const QString SECOND_SECTION_HEADER_INFO_VALUE_TEXT { "0" };
    const QString SECOND_SECTION_HEADER_INFO_DESCRIPTION_TEXT { "One greater than the last symbol table index of the local symbol (binding STB_LOCAL): 0" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_ADDRALIGN_VALUE { 8 };
    const QString SECOND_SECTION_HEADER_ADDRALIGN_VALUE_TEXT { "8" };
    const QString SECOND_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT { "Address align: 8 bytes" };

    constexpr Elf32_Word SECOND_SECTION_HEADER_ENTSIZE_32BIT_VALUE { 16 };
    const QString SECOND_SECTION_HEADER_ENTSIZE_VALUE_32BIT_TEXT { "16" };
    const QString SECOND_SECTION_HEADER_ENTSIZE_DESCRIPTION_32BIT_TEXT { "Entry size: 16 bytes" };

    constexpr Elf64_Word SECOND_SECTION_HEADER_ENTSIZE_64BIT_VALUE { 24 };
    const QString SECOND_SECTION_HEADER_ENTSIZE_VALUE_64BIT_TEXT { "24" };
    const QString SECOND_SECTION_HEADER_ENTSIZE_DESCRIPTION_64BIT_TEXT { "Entry size: 24 bytes" };



    // Third section header
    constexpr uint32_t THIRD_SECTION_HEADER_NAME_VALUE { 0x15 };
    const QString THIRD_SECTION_HEADER_NAME_VALUE_TEXT { "0x15" };
    const QString THIRD_SECTION_HEADER_NAME_DESCRIPTION_TEXT {};

    constexpr uint32_t THIRD_SECTION_HEADER_TYPE_VALUE { 3 };
    const QString THIRD_SECTION_HEADER_TYPE_VALUE_TEXT { "3" };
    const QString THIRD_SECTION_HEADER_TYPE_DESCRIPTION_TEXT { "Section type: String table" };

    constexpr Elf32_Word THIRD_SECTION_HEADER_FLAGS_VALUE {};
    const QString THIRD_SECTION_HEADER_FLAGS_VALUE_TEXT { "0x0" };
    const QString THIRD_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT {};

    constexpr Elf32_Addr THIRD_SECTION_HEADER_ADDR_VALUE {};
    const QString THIRD_SECTION_HEADER_ADDR_VALUE_TEXT { "0x0" };
    const QString THIRD_SECTION_HEADER_ADDR_DESCRIPTION_TEXT { "Section not loadable" };

    constexpr Elf32_Off THIRD_SECTION_HEADER_OFFSET_32BIT_VALUE { 0x1B94 };
    const QString THIRD_SECTION_HEADER_OFFSET_VALUE_32BIT_TEXT { "0x1B94" };
    const QString THIRD_SECTION_HEADER_OFFSET_DESCRIPTION_32BIT_TEXT { "Section offset in the file: 0x1B94" };

    constexpr Elf64_Off THIRD_SECTION_HEADER_OFFSET_64BIT_VALUE { 0x206C };
    const QString THIRD_SECTION_HEADER_OFFSET_VALUE_64BIT_TEXT { "0x206C" };
    const QString THIRD_SECTION_HEADER_OFFSET_DESCRIPTION_64BIT_TEXT { "Section offset in the file: 0x206C" };

    constexpr Elf32_Word THIRD_SECTION_HEADER_SIZE_VALUE { 200 };
    const QString THIRD_SECTION_HEADER_SIZE_VALUE_TEXT { "200" };
    const QString THIRD_SECTION_HEADER_SIZE_DESCRIPTION_TEXT { "Section size: 200 bytes" };

    constexpr Elf32_Word THIRD_SECTION_HEADER_LINK_VALUE {};
    const QString THIRD_SECTION_HEADER_LINK_VALUE_TEXT { "0" };
    const QString THIRD_SECTION_HEADER_LINK_DESCRIPTION_TEXT {};

    constexpr Elf32_Word THIRD_SECTION_HEADER_INFO_VALUE {};
    const QString THIRD_SECTION_HEADER_INFO_VALUE_TEXT { "0" };
    const QString THIRD_SECTION_HEADER_INFO_DESCRIPTION_TEXT {};

    constexpr Elf32_Word THIRD_SECTION_HEADER_ADDRALIGN_VALUE { 1 };
    const QString THIRD_SECTION_HEADER_ADDRALIGN_VALUE_TEXT { "1" };
    const QString THIRD_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT { "Address align: 1 bytes" };

    constexpr Elf32_Word THIRD_SECTION_HEADER_ENTSIZE_VALUE {};
    const QString THIRD_SECTION_HEADER_ENTSIZE_VALUE_TEXT { "0" };
    const QString THIRD_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT { "Section contains no entries" };


    const QString SECTION_HEADER_ELF_PART_NAME { "SectionHeader" };
    const ElfPartType SECTION_HEADER_ELF_PART_TYPE { ElfPartType::SectionHeader };
    constexpr int SECTION_HEADER_ELF_PART_X32_OFFSET { FILE_HEADER_SIZE_32_VALUE };
    constexpr int SECTION_HEADER_ELF_PART_X64_OFFSET { FILE_HEADER_SIZE_64_VALUE };
    constexpr int SECTION_HEADER_ELF_PART_X32_SIZE { SECTION_HEADER_SIZE_32_VALUE };
    constexpr int SECTION_HEADER_ELF_PART_X64_SIZE { SECTION_HEADER_SIZE_64_VALUE };
    const QString SECTION_HEADER_ELF_PART_DESCRIPTION {
        "Section Header contains information required during parsing of the Sections. Their type, offset in the file, total size and number of entries or linked sections."
    };


    ElfPart generateSectionHeaderElfPart(std::vector<ElfField> p_fields, int p_offset, int p_size)
    {
        return { SECTION_HEADER_ELF_PART_NAME, SECTION_HEADER_ELF_PART_TYPE, p_offset, p_size, SECTION_HEADER_ELF_PART_DESCRIPTION, p_fields };
    }

    std::vector<ElfPart> generate32BitElfParts()
    {
        return {
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_NAME_VALUE_TEXT, FIRST_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_TYPE_VALUE_TEXT, FIRST_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_FLAGS_VALUE_TEXT, FIRST_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF32_ADDR_TEXT, FIRST_SECTION_HEADER_ADDR_VALUE_TEXT, FIRST_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF32_OFF_TEXT, FIRST_SECTION_HEADER_OFFSET_VALUE_TEXT, FIRST_SECTION_HEADER_OFFSET_DESCRIPTION_TEXT},
                {SH_SIZE_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_SIZE_VALUE_32BIT_TEXT, FIRST_SECTION_HEADER_SIZE_DESCRIPTION_32BIT_TEXT},
                {SH_LINK_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_LINK_VALUE_TEXT, FIRST_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_INFO_VALUE_TEXT, FIRST_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, FIRST_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF32_WORD_TEXT, FIRST_SECTION_HEADER_ENTSIZE_VALUE_TEXT, FIRST_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_32_VALUE, SECTION_HEADER_SIZE_32_VALUE),
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_NAME_VALUE_TEXT, SECOND_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_TYPE_VALUE_TEXT, SECOND_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_FLAGS_VALUE_TEXT, SECOND_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF32_ADDR_TEXT, SECOND_SECTION_HEADER_ADDR_VALUE_TEXT, SECOND_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF32_OFF_TEXT, SECOND_SECTION_HEADER_OFFSET_VALUE_32BIT_TEXT, SECOND_SECTION_HEADER_OFFSET_DESCRIPTION_32BIT_TEXT},
                {SH_SIZE_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_SIZE_VALUE_32BIT_TEXT, SECOND_SECTION_HEADER_SIZE_DESCRIPTION_32BIT_TEXT},
                {SH_LINK_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_LINK_VALUE_TEXT, SECOND_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_INFO_VALUE_TEXT, SECOND_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, SECOND_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF32_WORD_TEXT, SECOND_SECTION_HEADER_ENTSIZE_VALUE_32BIT_TEXT, SECOND_SECTION_HEADER_ENTSIZE_DESCRIPTION_32BIT_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_32_VALUE + SECTION_HEADER_SIZE_32_VALUE, SECTION_HEADER_SIZE_32_VALUE),
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_NAME_VALUE_TEXT, THIRD_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_TYPE_VALUE_TEXT, THIRD_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_FLAGS_VALUE_TEXT, THIRD_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF32_ADDR_TEXT, THIRD_SECTION_HEADER_ADDR_VALUE_TEXT, THIRD_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF32_OFF_TEXT, THIRD_SECTION_HEADER_OFFSET_VALUE_32BIT_TEXT, THIRD_SECTION_HEADER_OFFSET_DESCRIPTION_32BIT_TEXT},
                {SH_SIZE_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_SIZE_VALUE_TEXT, THIRD_SECTION_HEADER_SIZE_DESCRIPTION_TEXT},
                {SH_LINK_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_LINK_VALUE_TEXT, THIRD_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_INFO_VALUE_TEXT, THIRD_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, THIRD_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF32_WORD_TEXT, THIRD_SECTION_HEADER_ENTSIZE_VALUE_TEXT, THIRD_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_32_VALUE + 2 * SECTION_HEADER_SIZE_32_VALUE, SECTION_HEADER_SIZE_32_VALUE)
        };
    }

    std::vector<ElfPart> generate64BitElfParts()
    {
        return {
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF64_WORD_TEXT, FIRST_SECTION_HEADER_NAME_VALUE_TEXT, FIRST_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF64_WORD_TEXT, FIRST_SECTION_HEADER_TYPE_VALUE_TEXT, FIRST_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF64_XWORD_TEXT, FIRST_SECTION_HEADER_FLAGS_VALUE_TEXT, FIRST_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF64_ADDR_TEXT, FIRST_SECTION_HEADER_ADDR_VALUE_TEXT, FIRST_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF64_OFF_TEXT, FIRST_SECTION_HEADER_OFFSET_VALUE_TEXT, FIRST_SECTION_HEADER_OFFSET_DESCRIPTION_TEXT},
                {SH_SIZE_NAME_TEXT, ELF64_XWORD_TEXT, FIRST_SECTION_HEADER_SIZE_VALUE_64BIT_TEXT, FIRST_SECTION_HEADER_SIZE_DESCRIPTION_64BIT_TEXT},
                {SH_LINK_NAME_TEXT, ELF64_WORD_TEXT, FIRST_SECTION_HEADER_LINK_VALUE_TEXT, FIRST_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF64_WORD_TEXT, FIRST_SECTION_HEADER_INFO_VALUE_TEXT, FIRST_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF64_XWORD_TEXT, FIRST_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, FIRST_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF64_XWORD_TEXT, FIRST_SECTION_HEADER_ENTSIZE_VALUE_TEXT, FIRST_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_64_VALUE, SECTION_HEADER_SIZE_64_VALUE),
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF64_WORD_TEXT, SECOND_SECTION_HEADER_NAME_VALUE_TEXT, SECOND_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF64_WORD_TEXT, SECOND_SECTION_HEADER_TYPE_VALUE_TEXT, SECOND_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF64_XWORD_TEXT, SECOND_SECTION_HEADER_FLAGS_VALUE_TEXT, SECOND_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF64_ADDR_TEXT, SECOND_SECTION_HEADER_ADDR_VALUE_TEXT, SECOND_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF64_OFF_TEXT, SECOND_SECTION_HEADER_OFFSET_VALUE_64BIT_TEXT, SECOND_SECTION_HEADER_OFFSET_DESCRIPTION_64BIT_TEXT},
                {SH_SIZE_NAME_TEXT, ELF64_XWORD_TEXT, SECOND_SECTION_HEADER_SIZE_VALUE_64BIT_TEXT, SECOND_SECTION_HEADER_SIZE_DESCRIPTION_64BIT_TEXT},
                {SH_LINK_NAME_TEXT, ELF64_WORD_TEXT, SECOND_SECTION_HEADER_LINK_VALUE_TEXT, SECOND_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF64_WORD_TEXT, SECOND_SECTION_HEADER_INFO_VALUE_TEXT, SECOND_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF64_XWORD_TEXT, SECOND_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, SECOND_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF64_XWORD_TEXT, SECOND_SECTION_HEADER_ENTSIZE_VALUE_64BIT_TEXT, SECOND_SECTION_HEADER_ENTSIZE_DESCRIPTION_64BIT_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_64_VALUE + SECTION_HEADER_SIZE_64_VALUE, SECTION_HEADER_SIZE_64_VALUE),
            generateSectionHeaderElfPart({
                {SH_NAME_NAME_TEXT, ELF64_WORD_TEXT, THIRD_SECTION_HEADER_NAME_VALUE_TEXT, THIRD_SECTION_HEADER_NAME_DESCRIPTION_TEXT},
                {SH_TYPE_NAME_TEXT, ELF64_WORD_TEXT, THIRD_SECTION_HEADER_TYPE_VALUE_TEXT, THIRD_SECTION_HEADER_TYPE_DESCRIPTION_TEXT},
                {SH_FLAGS_NAME_TEXT, ELF64_XWORD_TEXT, THIRD_SECTION_HEADER_FLAGS_VALUE_TEXT, THIRD_SECTION_HEADER_FLAGS_DESCRIPTION_TEXT},
                {SH_ADDR_NAME_TEXT, ELF64_ADDR_TEXT, THIRD_SECTION_HEADER_ADDR_VALUE_TEXT, THIRD_SECTION_HEADER_ADDR_DESCRIPTION_TEXT},
                {SH_OFFSET_NAME_TEXT, ELF64_OFF_TEXT, THIRD_SECTION_HEADER_OFFSET_VALUE_64BIT_TEXT, THIRD_SECTION_HEADER_OFFSET_DESCRIPTION_64BIT_TEXT},
                {SH_SIZE_NAME_TEXT, ELF64_XWORD_TEXT, THIRD_SECTION_HEADER_SIZE_VALUE_TEXT, THIRD_SECTION_HEADER_SIZE_DESCRIPTION_TEXT},
                {SH_LINK_NAME_TEXT, ELF64_WORD_TEXT, THIRD_SECTION_HEADER_LINK_VALUE_TEXT, THIRD_SECTION_HEADER_LINK_DESCRIPTION_TEXT},
                {SH_INFO_NAME_TEXT, ELF64_WORD_TEXT, THIRD_SECTION_HEADER_INFO_VALUE_TEXT, THIRD_SECTION_HEADER_INFO_DESCRIPTION_TEXT},
                {SH_ADDRALIGN_NAME_TEXT, ELF64_XWORD_TEXT, THIRD_SECTION_HEADER_ADDRALIGN_VALUE_TEXT, THIRD_SECTION_HEADER_ADDRALIGN_DESCRIPTION_TEXT},
                {SH_ENTSIZE_NAME_TEXT, ELF64_XWORD_TEXT, THIRD_SECTION_HEADER_ENTSIZE_VALUE_TEXT, THIRD_SECTION_HEADER_ENTSIZE_DESCRIPTION_TEXT}
            }, SECTION_HEADER_TABLE_OFFSET_64_VALUE + 2 * SECTION_HEADER_SIZE_64_VALUE, SECTION_HEADER_SIZE_64_VALUE)
        };
    }
}


using namespace ::testing;

struct ElfPartAssemblerSectionHeadersAssemblingX32TestSuite : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartAssemblerSectionHeadersAssemblingX32TestSuite, shouldNotAssembleAnyCorrectElfPartFromEmpty32BitSectionHeaderContainer)
{
    ElfPartAssembler l_elfPartAssembler {};
    std::vector<std::shared_ptr<Elf32_Shdr>> l_sectionHeaders;

    const auto& l_targetElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_sectionHeaders, SECTION_HEADER_TABLE_OFFSET_32_VALUE) };

    std::vector<ElfPart> l_expectedElfParts;

    expectElfPartContainersAreEqual(l_targetElfParts, l_expectedElfParts);
}

TEST_F(ElfPartAssemblerSectionHeadersAssemblingX32TestSuite, shouldAssembleAllCorrectElfPartsFrom32BitSectionHeaderContainer)
{
    Elf32_Shdr l_firstSectionHeader {
        .sh_name = FIRST_SECTION_HEADER_NAME_VALUE,
    	.sh_type = FIRST_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = FIRST_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = FIRST_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = FIRST_SECTION_HEADER_OFFSET_VALUE,
    	.sh_size = FIRST_SECTION_HEADER_SIZE_32BIT_VALUE,
    	.sh_link = FIRST_SECTION_HEADER_LINK_VALUE,
    	.sh_info = FIRST_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = FIRST_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = FIRST_SECTION_HEADER_ENTSIZE_VALUE
    };

    Elf32_Shdr l_secondSectionHeader {
        .sh_name = SECOND_SECTION_HEADER_NAME_VALUE,
    	.sh_type = SECOND_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = SECOND_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = SECOND_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = SECOND_SECTION_HEADER_OFFSET_32BIT_VALUE,
    	.sh_size = SECOND_SECTION_HEADER_SIZE_32BIT_VALUE,
    	.sh_link = SECOND_SECTION_HEADER_LINK_VALUE,
    	.sh_info = SECOND_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = SECOND_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = SECOND_SECTION_HEADER_ENTSIZE_32BIT_VALUE
    };

    Elf32_Shdr l_thirdSectionHeader {
        .sh_name = THIRD_SECTION_HEADER_NAME_VALUE,
    	.sh_type = THIRD_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = THIRD_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = THIRD_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = THIRD_SECTION_HEADER_OFFSET_32BIT_VALUE,
    	.sh_size = THIRD_SECTION_HEADER_SIZE_VALUE,
    	.sh_link = THIRD_SECTION_HEADER_LINK_VALUE,
    	.sh_info = THIRD_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = THIRD_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = THIRD_SECTION_HEADER_ENTSIZE_VALUE
    };

    std::vector<std::shared_ptr<Elf32_Shdr>> l_sectionHeaders {std::make_shared<Elf32_Shdr>(l_firstSectionHeader),
                                                               std::make_shared<Elf32_Shdr>(l_secondSectionHeader),
                                                               std::make_shared<Elf32_Shdr>(l_thirdSectionHeader)};

    ElfPartAssembler l_elfPartAssembler {};
    const auto& l_targetElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_sectionHeaders, SECTION_HEADER_TABLE_OFFSET_32_VALUE) };

    expectElfPartContainersAreEqual(l_targetElfParts, generate32BitElfParts());
}



struct ElfPartAssemblerSectionHeadersAssemblingX64TestSuite : public ElfPartAssemblingTestSuite
{};

TEST_F(ElfPartAssemblerSectionHeadersAssemblingX64TestSuite, shouldNotAssembleAnyCorrectElfPartFromEmpty64BitSectionHeaderContainer)
{
    ElfPartAssembler l_elfPartAssembler {};
    std::vector<std::shared_ptr<Elf64_Shdr>> l_sectionHeaders;

    const auto& l_targetElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_sectionHeaders, SECTION_HEADER_TABLE_OFFSET_64_VALUE) };

    std::vector<ElfPart> l_expectedElfParts;

    expectElfPartContainersAreEqual(l_targetElfParts, l_expectedElfParts);
}


TEST_F(ElfPartAssemblerSectionHeadersAssemblingX64TestSuite, shouldAssembleAllCorrectElfPartsFrom64BitSectionHeaderContainer)
{
    Elf64_Shdr l_firstSectionHeader {
        .sh_name = FIRST_SECTION_HEADER_NAME_VALUE,
    	.sh_type = FIRST_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = FIRST_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = FIRST_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = FIRST_SECTION_HEADER_OFFSET_VALUE,
    	.sh_size = FIRST_SECTION_HEADER_SIZE_64BIT_VALUE,
    	.sh_link = FIRST_SECTION_HEADER_LINK_VALUE,
    	.sh_info = FIRST_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = FIRST_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = FIRST_SECTION_HEADER_ENTSIZE_VALUE
    };

    Elf64_Shdr l_secondSectionHeader {
        .sh_name = SECOND_SECTION_HEADER_NAME_VALUE,
    	.sh_type = SECOND_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = SECOND_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = SECOND_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = SECOND_SECTION_HEADER_OFFSET_64BIT_VALUE,
    	.sh_size = SECOND_SECTION_HEADER_SIZE_64BIT_VALUE,
    	.sh_link = SECOND_SECTION_HEADER_LINK_VALUE,
    	.sh_info = SECOND_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = SECOND_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = SECOND_SECTION_HEADER_ENTSIZE_64BIT_VALUE
    };

    Elf64_Shdr l_thirdSectionHeader {
        .sh_name = THIRD_SECTION_HEADER_NAME_VALUE,
    	.sh_type = THIRD_SECTION_HEADER_TYPE_VALUE,
    	.sh_flags = THIRD_SECTION_HEADER_FLAGS_VALUE,
    	.sh_addr = THIRD_SECTION_HEADER_ADDR_VALUE,
    	.sh_offset = THIRD_SECTION_HEADER_OFFSET_64BIT_VALUE,
    	.sh_size = THIRD_SECTION_HEADER_SIZE_VALUE,
    	.sh_link = THIRD_SECTION_HEADER_LINK_VALUE,
    	.sh_info = THIRD_SECTION_HEADER_INFO_VALUE,
    	.sh_addralign = THIRD_SECTION_HEADER_ADDRALIGN_VALUE,
    	.sh_entsize = THIRD_SECTION_HEADER_ENTSIZE_VALUE
    };

    std::vector<std::shared_ptr<Elf64_Shdr>> l_sectionHeaders {std::make_shared<Elf64_Shdr>(l_firstSectionHeader),
                                                               std::make_shared<Elf64_Shdr>(l_secondSectionHeader),
                                                               std::make_shared<Elf64_Shdr>(l_thirdSectionHeader)};

    ElfPartAssembler l_elfPartAssembler {};
    const auto& l_targetElfParts { l_elfPartAssembler.assembleElfPartsFromSectionHeaders(l_sectionHeaders, SECTION_HEADER_TABLE_OFFSET_64_VALUE) };

    expectElfPartContainersAreEqual(l_targetElfParts, generate64BitElfParts());
}
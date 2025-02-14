#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfBuildDirector.hpp"
#include "ElfSectionBuilderMock.hpp"
#include "ElfStructureInfoBuilderMock.hpp"
#include "ElfStructureInfoX32.hpp"
#include "ElfObjectX32.hpp"
#include <elf.h>


namespace
{
    Elf32_Shdr SYMBOL_TABLE_SECTION_HEADER
    {
        .sh_type = SHT_SYMTAB
    };

    Elf32_Shdr NOTE_SECTION_HEADER
    {
        .sh_type = SHT_NOTE
    };

    Elf32_Shdr RELOCATION_SECTION_HEADER
    {
        .sh_type = SHT_REL
    };

    Elf32_Shdr RELOCATION_WITH_ADDEND_SECTION_HEADER
    {
        .sh_type = SHT_RELA
    };

    Elf32_Shdr STRING_TABLE_SECTION_HEADER
    {
        .sh_type = SHT_STRTAB
    };

    Elf32_Shdr PROGBITS_SECTION_HEADER
    {
        .sh_type = SHT_PROGBITS
    };

    Elf32_Shdr NOBITS_SECTION_HEADER
    {
        .sh_type = SHT_NOBITS
    };

    Elf32_Shdr NULL_SECTION_HEADER
    {
        .sh_type = SHT_NULL
    };

}


using namespace ::testing;

TEST(Elf32BitElfBuildDrectorTestSuite, shouldCallAllTheNecessary32BitElfBuilderFunctions)
{
    StrictMock<ElfStructureInfoBuilderMock<ElfStructureInfoX32>> l_elfStructureInfoBuilderMock;
    StrictMock<ElfSectionBuilderMock<ElfObjectX32, ElfStructureInfoX32>> l_elfObjectBuilderMock;

    ElfBuildDirector l_elfBuildDirector;

    ElfStructureInfoX32 l_elfstructureInfo
    {
        .sectionHeaders = {
            std::make_shared<Elf32_Shdr>(SYMBOL_TABLE_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(NOTE_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(RELOCATION_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(RELOCATION_WITH_ADDEND_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(STRING_TABLE_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(PROGBITS_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(NOBITS_SECTION_HEADER),
            std::make_shared<Elf32_Shdr>(NULL_SECTION_HEADER),
        }
    };

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildFileHeader)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildSectionHeaders)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildProgramHeaders)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(&l_elfstructureInfo));

    EXPECT_CALL(l_elfObjectBuilderMock, buildSymbolSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildNoteSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildRelocationSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildRelocationWithAddendSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildStringTableSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildProgbitsSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildNobitsSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildNullSection)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, getResult)
        .WillOnce(Return(new ElfObjectX32));

    l_elfBuildDirector.makeElfObject(l_elfStructureInfoBuilderMock, l_elfObjectBuilderMock);
}

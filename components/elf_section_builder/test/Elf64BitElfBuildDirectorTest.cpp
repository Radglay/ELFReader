#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfBuildDirector.hpp"
#include "ElfSectionBuilderMock.hpp"
#include "ElfStructureInfoBuilderMock.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfObjectX64.hpp"
#include <elf.h>


namespace
{
    Elf64_Shdr SYMBOL_TABLE_SECTION_HEADER
    {
        .sh_type = SHT_SYMTAB
    };

    Elf64_Shdr NOTE_SECTION_HEADER
    {
        .sh_type = SHT_NOTE
    };

    Elf64_Shdr RELOCATION_SECTION_HEADER
    {
        .sh_type = SHT_REL
    };

    Elf64_Shdr RELOCATION_WITH_ADDEND_SECTION_HEADER
    {
        .sh_type = SHT_RELA
    };

    Elf64_Shdr STRING_TABLE_SECTION_HEADER
    {
        .sh_type = SHT_STRTAB
    };

    Elf64_Shdr PROGBITS_SECTION_HEADER
    {
        .sh_type = SHT_PROGBITS
    };

    Elf64_Shdr NOBITS_SECTION_HEADER
    {
        .sh_type = SHT_NOBITS
    };
}


using namespace ::testing;

TEST(Elf64BitElfBuildDrectorTestSuite, shouldCallAllTheNecessary64BitElfBuilderFunctions)
{
    StrictMock<ElfStructureInfoBuilderMock<ElfStructureInfoX64>> l_elfStructureInfoBuilderMock;
    StrictMock<ElfSectionBuilderMock<ElfObjectX64, ElfStructureInfoX64>> l_elfObjectBuilderMock;

    ElfBuildDirector l_elfBuildDirector;


    ElfStructureInfoX64 l_elfstructureInfo
    {
        .sectionHeaders = {
            std::make_shared<Elf64_Shdr>(SYMBOL_TABLE_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(NOTE_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(RELOCATION_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(RELOCATION_WITH_ADDEND_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(STRING_TABLE_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(PROGBITS_SECTION_HEADER),
            std::make_shared<Elf64_Shdr>(NOBITS_SECTION_HEADER)
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
    EXPECT_CALL(l_elfObjectBuilderMock, getResult)
        .WillOnce(Return(new ElfObjectX64));

    l_elfBuildDirector.makeElfObject(l_elfStructureInfoBuilderMock, l_elfObjectBuilderMock);
}

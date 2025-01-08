#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfBuildDirector.hpp"
#include "ElfObjectBuilderMock.hpp"
#include "ElfStructureInfoX64.hpp"
#include "ElfObjectX64.hpp"
#include <memory>


using namespace ::testing;

TEST(Elf64BitElfBuildDrectorTestSuite, shouldCallAllTheNecessary64BitElfBuilderFunctions)
{
    StrictMock<ElfObjectBuilderMock<ElfObjectX64, ElfStructureInfoX64>> l_elfObjectBuilderMock;

    ElfBuildDirector l_elfBuildDirector;

    EXPECT_CALL(l_elfObjectBuilderMock, buildElfStructureInfo)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildSymbolSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildNoteSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildRelocationSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildRelocationWithAddendSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildStringTableSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildProgbitsSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, buildNobitsSections)
        .Times(1);
    EXPECT_CALL(l_elfObjectBuilderMock, getResult)
        .WillOnce(Return(new ElfObjectX64));

    l_elfBuildDirector.makeElfObject(l_elfObjectBuilderMock);
}

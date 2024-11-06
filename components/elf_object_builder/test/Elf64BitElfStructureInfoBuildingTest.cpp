#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfObjectBuilder.hpp"
#include "ElfStructureInfoBuilderMock.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX64.hpp"


using namespace ::testing;



TEST(ElfObjectBuilderX64TestSuite, shouldBuild64BitLittleEndianElfStructureInfo)
{
    std::stringstream l_stubStream;
    StrictMock<ElfStructureInfoBuilderMock<ElfStructureInfoX64>> l_elfStructureInfoBuilderMock;
    TargetMachineInfo l_targetMachineInfo;

    ElfObjectBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_elfStructureInfoBuilderMock, l_targetMachineInfo);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildFileHeader)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildSectionHeaders)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, buildProgramHeaders)
        .Times(1);

    EXPECT_CALL(l_elfStructureInfoBuilderMock, getResult)
        .WillOnce(Return(new ElfStructureInfoX64));

    l_elfObjectBuilder.buildElfStructureInfo();
}

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfFileParserCreator.hpp"
#include <sstream>
#include <string>
#include "ElfFileParserX32.hpp"
#include "ElfFileParserX64.hpp"


namespace
{

char ELF_MAGIC_NUMBERS[] { 0x7f, 0x45, 0x4c, 0x46 };

constexpr char SYSTEM_VERSION_WRONG { 0x5 };
constexpr char SYSTEM_VERSION_32_BIT { 0x1 };
constexpr char SYSTEM_VERSION_64_BIT { 0x2 };

}

namespace parser
{

using namespace ::testing;


class ElfFileParserCreatorTestSuite : public Test
{
protected:
    ElfFileParserCreatorTestSuite()
        : m_streamContent{ ELF_MAGIC_NUMBERS }
    {}

    std::string m_streamContent;
};


TEST_F(ElfFileParserCreatorTestSuite,
       shouldReturnNullptrWhenNoVersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream() };
    auto* l_fileParser { ElfFileParserCreator::createElfFileParser(l_stubStream) };
    ASSERT_EQ(l_fileParser, nullptr);

    delete l_stubStream;
}

TEST_F(ElfFileParserCreatorTestSuite,
       shouldReturnNullptrWhenWrongVersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream(m_streamContent + SYSTEM_VERSION_WRONG) };
    auto* l_fileParser { ElfFileParserCreator::createElfFileParser(l_stubStream) };
    ASSERT_EQ(l_fileParser, nullptr);

    delete l_stubStream;
}

TEST_F(ElfFileParserCreatorTestSuite,
       shouldReturnPointerTo32BitVersionWhen32VersionIdentifierExtracted)
{
    std::istringstream* l_stubStream {new std::istringstream( m_streamContent + SYSTEM_VERSION_32_BIT) };
    auto* l_fileParser { ElfFileParserCreator::createElfFileParser(l_stubStream) };
    ASSERT_NE(l_fileParser, nullptr);
    EXPECT_THAT(l_fileParser,
                WhenDynamicCastTo<ElfFileParserX32*>(Pointee(A<ElfFileParserX32>())));

    delete l_stubStream;
}

TEST_F(ElfFileParserCreatorTestSuite,
       shouldReturnPointerTo64BitVersionWhen64VersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream(m_streamContent + SYSTEM_VERSION_64_BIT) };
    auto* l_fileParser { ElfFileParserCreator::createElfFileParser(l_stubStream) };
    ASSERT_NE(l_fileParser, nullptr);
    EXPECT_THAT(l_fileParser,
                WhenDynamicCastTo<ElfFileParserX64*>(Pointee(A<ElfFileParserX64>())));
    
    delete l_stubStream;
}

}

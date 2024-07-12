#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfHeaderReaderCreator.hpp"
#include <sstream>
#include <string>
#include "ElfHeaderReaderX32.hpp"
#include "ElfHeaderReaderX64.hpp"


namespace
{

char ELF_MAGIC_NUMBERS[] { 0x7f, 0x45, 0x4c, 0x46 };

constexpr char SYSTEM_VERSION_WRONG { 0x5 };
constexpr char SYSTEM_VERSION_32_BIT { 0x1 };
constexpr char SYSTEM_VERSION_64_BIT { 0x2 };

}

namespace reader
{

using namespace ::testing;


class ElfHeaderReaderCreatorTestSuite : public Test
{
protected:
    ElfHeaderReaderCreatorTestSuite()
        : m_streamContent{ ELF_MAGIC_NUMBERS }
    {}

    std::string m_streamContent;
};


TEST_F(ElfHeaderReaderCreatorTestSuite,
       shouldReturnNullptrWhenNoVersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream() };
    auto* l_fileParser { ElfHeaderReaderCreator::createElfHeaderReader(l_stubStream) };
    ASSERT_EQ(l_fileParser, nullptr);

    delete l_stubStream;
}

TEST_F(ElfHeaderReaderCreatorTestSuite,
       shouldReturnNullptrWhenWrongVersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream(m_streamContent + SYSTEM_VERSION_WRONG) };
    auto* l_fileParser { ElfHeaderReaderCreator::createElfHeaderReader(l_stubStream) };
    ASSERT_EQ(l_fileParser, nullptr);

    delete l_stubStream;
}

TEST_F(ElfHeaderReaderCreatorTestSuite,
       shouldReturnPointerTo32BitVersionWhen32VersionIdentifierExtracted)
{
    std::istringstream* l_stubStream {new std::istringstream( m_streamContent + SYSTEM_VERSION_32_BIT) };
    auto* l_fileParser { ElfHeaderReaderCreator::createElfHeaderReader(l_stubStream) };
    ASSERT_NE(l_fileParser, nullptr);
    EXPECT_THAT(l_fileParser,
                WhenDynamicCastTo<ElfHeaderReaderX32*>(Pointee(A<ElfHeaderReaderX32>())));

    delete l_stubStream;
}

TEST_F(ElfHeaderReaderCreatorTestSuite,
       shouldReturnPointerTo64BitVersionWhen64VersionIdentifierExtracted)
{
    std::istringstream* l_stubStream { new std::istringstream(m_streamContent + SYSTEM_VERSION_64_BIT) };
    auto* l_fileParser { ElfHeaderReaderCreator::createElfHeaderReader(l_stubStream) };
    ASSERT_NE(l_fileParser, nullptr);
    EXPECT_THAT(l_fileParser,
                WhenDynamicCastTo<ElfHeaderReaderX64*>(Pointee(A<ElfHeaderReaderX64>())));
    
    delete l_stubStream;
}

}

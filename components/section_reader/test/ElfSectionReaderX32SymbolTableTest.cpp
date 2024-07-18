#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfSectionReaderX32.hpp"
#include <string>
#include <sstream>
#include <elf.h>
#include <vector>
#include <algorithm>


namespace
{

constexpr int HOST_ENDIANNESS { BYTE_ORDER == BIG_ENDIAN ? ELFDATA2MSB : ELFDATA2LSB };

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };

constexpr Elf32_Shdr SYMBOL_TABLE_SECTION_WITH_ZERO_ELEMENTS
{
    .sh_name = 0x1,
    .sh_type = 0x2,
    .sh_flags = 0x0,
    .sh_addr = 0x0,
    .sh_offset = 0x0,
    .sh_size = 0x0,
    .sh_link = 0x1d,
    .sh_info = 0x16,
    .sh_addralign = 0x8,
    .sh_entsize = 0x18
};

constexpr Elf32_Shdr SYMBOL_TABLE_SECTION_WITH_FIVE_ELEMENTS
{
    .sh_name = 0x1,
    .sh_type = 0x2,
    .sh_flags = 0x0,
    .sh_addr = 0x0,
    .sh_offset = 0x0,
    .sh_size = 0x5 * 0x10,
    .sh_link = 0x1d,
    .sh_info = 0x16,
    .sh_addralign = 0x4,
    .sh_entsize = 0x10
};


// (1) UND
constexpr char ST_NAME_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_VALUE_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_SIZE_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_INFO_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0 };
constexpr char ST_OTHER_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0 };
constexpr char ST_SECTION_BYTES_LITTLE_ENDIAN_UNDEFINED[] { 0x0, 0x0 };

constexpr Elf32_Sym UNDEFINED_SYMBOL
{};

// (2) test.cpp
constexpr char ST_NAME_BYTES_LITTLE_ENDIAN_TEST_CPP[] { 0x1, 0x0, 0x0, 0x0 };
constexpr char ST_VALUE_BYTES_LITTLE_ENDIAN_TEST_CPP[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_SIZE_BYTES_LITTLE_ENDIAN_TEST_CPP[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_INFO_BYTES_LITTLE_ENDIAN_TEST_CPP[] { 0x4 };
constexpr char ST_OTHER_BYTES_LITTLE_ENDIAN_TEST_CPP[] { 0x0 };
constexpr char ST_SECTION_BYTES_LITTLE_ENDIAN_TEST_CPP[] { static_cast<char>(0xf1), static_cast<char>(0xff) };

constexpr Elf32_Sym TEST_CPP_SYMBOL
{
    .st_name = 0x1,
    .st_value = 0x0,
    .st_size = 0x0,
    .st_info = 0x4,
    .st_other = 0x0,
    .st_shndx = 0xff'f1
};

// (3) _DYNAMIC
constexpr char ST_NAME_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0xa, 0x0, 0x0, 0x0 };
constexpr char ST_VALUE_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0x78, 0x3d, 0x0, 0x0 };
constexpr char ST_SIZE_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0x0, 0x0, 0x0, 0x0 };
constexpr char ST_INFO_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0x12 };
constexpr char ST_OTHER_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0x0 };
constexpr char ST_SECTION_BYTES_LITTLE_ENDIAN_DYNAMIC[] { 0x23, 0x0 };

constexpr Elf32_Sym DYNAMIC_SYMBOL
{
    .st_name = 0xa,
    .st_value = 0x3d'78,
    .st_size = 0x0,
    .st_info = 0x12,
    .st_other = 0x0,
    .st_shndx = 0x23
};

// (4) my_global_var
constexpr char ST_NAME_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x13, 0x0, 0x0, 0x0 };
constexpr char ST_VALUE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x10, 0x40, 0x0, 0x0 };
constexpr char ST_SIZE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x04, 0x0, 0x0, 0x0 };
constexpr char ST_INFO_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x11 };
constexpr char ST_OTHER_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x0 };
constexpr char ST_SECTION_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR[] { 0x25, 0x0 };

constexpr Elf32_Sym MY_GLOBAL_VAR_SYMBOL
{
    .st_name = 0x13,
    .st_value = 0x40'10,
    .st_size = 0x04,
    .st_info = 0x11,
    .st_other = 0x0,
    .st_shndx = 0x25
};

// (5) main
constexpr char ST_NAME_BYTES_LITTLE_ENDIAN_MAIN[] { 0x21, 0x0, 0x0, 0x0 };
constexpr char ST_VALUE_BYTES_LITTLE_ENDIAN_MAIN[] { 0x05, 0x12, 0x0, 0x0 };
constexpr char ST_SIZE_BYTES_LITTLE_ENDIAN_MAIN[] { static_cast<char>(0x80), 0x10, 0x0, 0x0 };
constexpr char ST_INFO_BYTES_LITTLE_ENDIAN_MAIN[] { 0x12 };
constexpr char ST_OTHER_BYTES_LITTLE_ENDIAN_MAIN[] { 0x0 };
constexpr char ST_SECTION_BYTES_LITTLE_ENDIAN_MAIN[] { 0x16, 0x0 };

constexpr Elf32_Sym MAIN_FUNC_SYMBOL
{
    .st_name = 0x21,
    .st_value = 0x12'05,
    .st_size = 0x10'80,
    .st_info = 0x12,
    .st_other = 0x0,
    .st_shndx = 0x16
};

}


namespace reader
{

using namespace ::testing;


std::string generate32BitLittleEndianSymbolTable()
{
    std::string l_streamContent;

    l_streamContent.append(ST_NAME_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(Elf32_Word));
    l_streamContent.append(ST_VALUE_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(Elf32_Addr));
    l_streamContent.append(ST_SIZE_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(Elf32_Word));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(unsigned char));
    l_streamContent.append(ST_SECTION_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(Elf32_Section));

    l_streamContent.append(ST_NAME_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(Elf32_Word));
    l_streamContent.append(ST_VALUE_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(Elf32_Addr));
    l_streamContent.append(ST_SIZE_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(Elf32_Word));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(unsigned char));
    l_streamContent.append(ST_SECTION_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(Elf32_Section));

    l_streamContent.append(ST_NAME_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(Elf32_Word));
    l_streamContent.append(ST_VALUE_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(Elf32_Addr));
    l_streamContent.append(ST_SIZE_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(Elf32_Word));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(unsigned char));
    l_streamContent.append(ST_SECTION_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(Elf32_Section));

    l_streamContent.append(ST_NAME_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(Elf32_Word));
    l_streamContent.append(ST_VALUE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(Elf32_Addr));
    l_streamContent.append(ST_SIZE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(Elf32_Word));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(unsigned char));
    l_streamContent.append(ST_SECTION_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(Elf32_Section));

    l_streamContent.append(ST_NAME_BYTES_LITTLE_ENDIAN_MAIN, sizeof(Elf32_Word));
    l_streamContent.append(ST_VALUE_BYTES_LITTLE_ENDIAN_MAIN, sizeof(Elf32_Addr));
    l_streamContent.append(ST_SIZE_BYTES_LITTLE_ENDIAN_MAIN, sizeof(Elf32_Word));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_MAIN, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_MAIN, sizeof(unsigned char));
    l_streamContent.append(ST_SECTION_BYTES_LITTLE_ENDIAN_MAIN, sizeof(Elf32_Section));

    return l_streamContent;
}

std::string generate32BitBigEndianSymbolTable()
{
    std::string l_streamContent;
    l_streamContent.append(std::rbegin(ST_NAME_BYTES_LITTLE_ENDIAN_UNDEFINED),
                           std::rend(ST_NAME_BYTES_LITTLE_ENDIAN_UNDEFINED));
    l_streamContent.append(std::rbegin(ST_VALUE_BYTES_LITTLE_ENDIAN_UNDEFINED),
                           std::rend(ST_VALUE_BYTES_LITTLE_ENDIAN_UNDEFINED));
    l_streamContent.append(std::rbegin(ST_SIZE_BYTES_LITTLE_ENDIAN_UNDEFINED),
                           std::rend(ST_SIZE_BYTES_LITTLE_ENDIAN_UNDEFINED));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_UNDEFINED, sizeof(unsigned char));
    l_streamContent.append(std::rbegin(ST_SECTION_BYTES_LITTLE_ENDIAN_UNDEFINED),
                           std::rend(ST_SECTION_BYTES_LITTLE_ENDIAN_UNDEFINED));

    l_streamContent.append(std::rbegin(ST_NAME_BYTES_LITTLE_ENDIAN_TEST_CPP),
                           std::rend(ST_NAME_BYTES_LITTLE_ENDIAN_TEST_CPP));
    l_streamContent.append(std::rbegin(ST_VALUE_BYTES_LITTLE_ENDIAN_TEST_CPP),
                           std::rend(ST_VALUE_BYTES_LITTLE_ENDIAN_TEST_CPP));
    l_streamContent.append(std::rbegin(ST_SIZE_BYTES_LITTLE_ENDIAN_TEST_CPP),
                           std::rend(ST_SIZE_BYTES_LITTLE_ENDIAN_TEST_CPP));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_TEST_CPP, sizeof(unsigned char));
    l_streamContent.append(std::rbegin(ST_SECTION_BYTES_LITTLE_ENDIAN_TEST_CPP),
                           std::rend(ST_SECTION_BYTES_LITTLE_ENDIAN_TEST_CPP));

    l_streamContent.append(std::rbegin(ST_NAME_BYTES_LITTLE_ENDIAN_DYNAMIC),
                           std::rend(ST_NAME_BYTES_LITTLE_ENDIAN_DYNAMIC));
    l_streamContent.append(std::rbegin(ST_VALUE_BYTES_LITTLE_ENDIAN_DYNAMIC),
                           std::rend(ST_VALUE_BYTES_LITTLE_ENDIAN_DYNAMIC));
    l_streamContent.append(std::rbegin(ST_SIZE_BYTES_LITTLE_ENDIAN_DYNAMIC),
                           std::rend(ST_SIZE_BYTES_LITTLE_ENDIAN_DYNAMIC));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_DYNAMIC, sizeof(unsigned char));
    l_streamContent.append(std::rbegin(ST_SECTION_BYTES_LITTLE_ENDIAN_DYNAMIC),
                           std::rend(ST_SECTION_BYTES_LITTLE_ENDIAN_DYNAMIC));


    l_streamContent.append(std::rbegin(ST_NAME_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR),
                           std::rend(ST_NAME_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR));
    l_streamContent.append(std::rbegin(ST_VALUE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR),
                           std::rend(ST_VALUE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR));
    l_streamContent.append(std::rbegin(ST_SIZE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR),
                           std::rend(ST_SIZE_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR, sizeof(unsigned char));
    l_streamContent.append(std::rbegin(ST_SECTION_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR),
                           std::rend(ST_SECTION_BYTES_LITTLE_ENDIAN_MY_GLOBAL_VAR));

    l_streamContent.append(std::rbegin(ST_NAME_BYTES_LITTLE_ENDIAN_MAIN),
                           std::rend(ST_NAME_BYTES_LITTLE_ENDIAN_MAIN));
    l_streamContent.append(std::rbegin(ST_VALUE_BYTES_LITTLE_ENDIAN_MAIN),
                           std::rend(ST_VALUE_BYTES_LITTLE_ENDIAN_MAIN));
    l_streamContent.append(std::rbegin(ST_SIZE_BYTES_LITTLE_ENDIAN_MAIN),
                           std::rend(ST_SIZE_BYTES_LITTLE_ENDIAN_MAIN));
    l_streamContent.append(ST_INFO_BYTES_LITTLE_ENDIAN_MAIN, sizeof(unsigned char));
    l_streamContent.append(ST_OTHER_BYTES_LITTLE_ENDIAN_MAIN, sizeof(unsigned char));
    l_streamContent.append(std::rbegin(ST_SECTION_BYTES_LITTLE_ENDIAN_MAIN),
                           std::rend(ST_SECTION_BYTES_LITTLE_ENDIAN_MAIN));


    return l_streamContent;
}


void expectSymbolsAreEqual(const std::vector<Elf32_Sym>& p_targetSymbols,
                           const std::vector<Elf32_Sym>& p_expectedSymbols)
{
    EXPECT_EQ(p_targetSymbols.size(), p_expectedSymbols.size());

    int l_size = p_expectedSymbols.size();
    for (int i = 0; i < l_size; ++i)
    {
        const auto& l_targetSymbol { p_targetSymbols[i] };
        const auto& l_expectedSymbol { p_expectedSymbols[i] };
        EXPECT_THAT(l_targetSymbol, FieldsAre(
            l_expectedSymbol.st_name,
            l_expectedSymbol.st_value,
            l_expectedSymbol.st_size,
            l_expectedSymbol.st_info,
            l_expectedSymbol.st_other,
            l_expectedSymbol.st_shndx
        ));
    }
} 

TEST(ElfSectionReaderX32TestSuite, shouldNotReadAnySymbolFrom32BitLittleEndianFile)
{
    std::string l_streamContent { generate32BitLittleEndianSymbolTable() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfSectionReaderX32 l_sectionReader { l_stubStream };
    auto l_targetSymbols { l_sectionReader.readSymbols(SYMBOL_TABLE_SECTION_WITH_ZERO_ELEMENTS, 
                                                       LITTLE_ENDIAN_VALUE,
                                                       HOST_ENDIANNESS) };

    std::vector<Elf32_Sym> l_expectedSymbols;

    expectSymbolsAreEqual(l_targetSymbols, l_expectedSymbols);
}

TEST(ElfSectionReaderX32TestSuite, shouldNotReadAnySymbolFrom32BitBigEndianFile)
{
    std::string l_streamContent { generate32BitBigEndianSymbolTable() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfSectionReaderX32 l_sectionReader { l_stubStream };
    auto l_targetSymbols { l_sectionReader.readSymbols(SYMBOL_TABLE_SECTION_WITH_ZERO_ELEMENTS, 
                                                       BIG_ENDIAN_VALUE,
                                                       HOST_ENDIANNESS) };

    std::vector<Elf32_Sym> l_expectedSymbols;

    expectSymbolsAreEqual(l_targetSymbols, l_expectedSymbols);
}

TEST(ElfSectionReaderX32TestSuite, shouldReadAllSymbolsFrom32BitLittleEndianFile)
{
    std::string l_streamContent { generate32BitLittleEndianSymbolTable() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfSectionReaderX32 l_sectionReader { l_stubStream };
    auto l_targetSymbols { l_sectionReader.readSymbols(SYMBOL_TABLE_SECTION_WITH_FIVE_ELEMENTS, 
                                                       LITTLE_ENDIAN_VALUE,
                                                       HOST_ENDIANNESS) };

    std::vector<Elf32_Sym> l_expectedSymbols {
        UNDEFINED_SYMBOL, TEST_CPP_SYMBOL, DYNAMIC_SYMBOL, MY_GLOBAL_VAR_SYMBOL, MAIN_FUNC_SYMBOL };
    
    expectSymbolsAreEqual(l_targetSymbols, l_expectedSymbols);
}

TEST(ElfSectionReaderX32TestSuite, shouldReadAllSymbolsFrom32BitBigEndianFile)
{
    std::string l_streamContent { generate32BitBigEndianSymbolTable() };
    std::istringstream* l_stubStream { new std::istringstream(l_streamContent) };

    ElfSectionReaderX32 l_sectionReader { l_stubStream };
    auto l_targetSymbols { l_sectionReader.readSymbols(SYMBOL_TABLE_SECTION_WITH_FIVE_ELEMENTS, 
                                                       BIG_ENDIAN_VALUE,
                                                       HOST_ENDIANNESS) };

    std::vector<Elf32_Sym> l_expectedSymbols {
        UNDEFINED_SYMBOL, TEST_CPP_SYMBOL, DYNAMIC_SYMBOL, MY_GLOBAL_VAR_SYMBOL, MAIN_FUNC_SYMBOL };
    
    expectSymbolsAreEqual(l_targetSymbols, l_expectedSymbols);
}

}

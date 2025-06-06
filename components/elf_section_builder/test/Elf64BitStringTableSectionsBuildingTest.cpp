#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ElfSectionBuilder.hpp"
#include "TargetMachineInfo.hpp"
#include <sstream>
#include "ElfObjectX64.hpp"
#include "ElfStructureInfoX64.hpp"
#include <tuple>
#include <vector>
#include "StringTableSection.hpp"
#include <memory>
#include <map>
#include "NoteDescriptorBuilderMock.hpp"


namespace
{

constexpr int LITTLE_ENDIAN_VALUE { ELFDATA2LSB };
constexpr int BIG_ENDIAN_VALUE { ELFDATA2MSB };

Elf64_Shdr STRING_TABLE_SECTION_WITH_ZERO_ELEMENTS
{
    .sh_type = SHT_STRTAB,
    .sh_offset = 0x0,
    .sh_size = 0x0
};


// (1) first section
std::string MAIN_CPP_NAME { "main.cpp" };
long unsigned int MAIN_CPP_INDEX { 0x0 };

std::string STD_COUT_NAME { "_ZSt4cout@GLIBCXX_3.4" };
long unsigned int STD_COUT_INDEX { MAIN_CPP_NAME.size() + 1 };

std::string MAIN_NAME { "main" };
long unsigned int MAIN_INDEX { STD_COUT_INDEX + STD_COUT_NAME.size() + 1 };

std::string DATA_START_NAME { "__data_start" };
long unsigned int DATA_START_INDEX { MAIN_INDEX + MAIN_NAME.size() + 1 };

std::string BSS_START_NAME { "__bss_start" };
long unsigned int BSS_START_INDEX { DATA_START_INDEX + DATA_START_NAME.size() + 1 };

std::string MY_CONST_GLOBAL_VAR_NAME { "ZL19my_const_global_var" };
long unsigned int MY_CONST_GLOBAL_VAR_INDEX { BSS_START_INDEX + BSS_START_NAME.size() + 1 };

std::string GLOBAL_OFFSET_TABLE_NAME { "GLOBAL_OFFSET_TABLE" };
long unsigned int GLOBAL_OFFSET_TABLE_INDEX { MY_CONST_GLOBAL_VAR_INDEX + MY_CONST_GLOBAL_VAR_NAME.size() + 1 };

std::string MY_FUNC_INT_NAME { "Z7my_funci" };
long unsigned int MY_FUNC_INT_INDEX { GLOBAL_OFFSET_TABLE_INDEX + GLOBAL_OFFSET_TABLE_NAME.size() + 1  };

std::string INIT_NAME { "_init" };
long unsigned int INIT_INDEX { MY_FUNC_INT_INDEX + MY_FUNC_INT_NAME.size() + 1 };

std::string GMON_START_NAME { "__gmon_start__" };
long unsigned int GMON_START_INDEX { INIT_INDEX + INIT_NAME.size() +  1 };

Elf64_Shdr STRING_TABLE_SECTION_WITH_TEN_ELEMENTS
{
    .sh_type = SHT_STRTAB,
    .sh_offset = 0x0,
    .sh_size = static_cast<Elf64_Word>(GMON_START_INDEX + GMON_START_NAME.size())
};



// (2) second section 
std::string STRTAB_NAME { ".strtab" };
long unsigned int STRTAB_INDEX { 0x0 };

std::string TEXT_NAME { ".text" };
long unsigned int TEXT_INDEX { STRTAB_INDEX + STRTAB_NAME.size() + 1 };

std::string RODATA_NAME { ".rodata" };
long unsigned int RODATA_INDEX { TEXT_INDEX + TEXT_NAME.size() + 1 };

std::string DYN_NAME { ".dyn" };
long unsigned int DYN_INDEX { RODATA_INDEX + RODATA_NAME.size() + 1 };

std::string RELA_PLT_NAME { "rela.plt" };
long unsigned int RELA_PLT_INDEX { DYN_INDEX + DYN_NAME.size() + 1 };

Elf64_Shdr STRING_TABLE_SECTION_WITH_FIVE_ELEMENTS
{
    .sh_type = SHT_STRTAB,
    .sh_offset = STRING_TABLE_SECTION_WITH_TEN_ELEMENTS.sh_size + 1,
    .sh_size = static_cast<Elf64_Word>(RELA_PLT_INDEX + RELA_PLT_NAME.size())
};


std::string generateFirstStringTableContent()
{
    std::string l_content;

    l_content += MAIN_CPP_NAME + '\0';
    l_content += STD_COUT_NAME + '\0';
    l_content += MAIN_NAME + '\0';
    l_content += DATA_START_NAME + '\0';
    l_content += BSS_START_NAME + '\0';
    l_content += MY_CONST_GLOBAL_VAR_NAME + '\0';
    l_content += GLOBAL_OFFSET_TABLE_NAME + '\0';
    l_content += MY_FUNC_INT_NAME + '\0';
    l_content += INIT_NAME + '\0';
    l_content += GMON_START_NAME + '\0';

    return l_content;
}

std::string generateSecondStringTableContent()
{
    std::string l_content;

    l_content += STRTAB_NAME + '\0';
    l_content += TEXT_NAME + '\0';
    l_content += RODATA_NAME + '\0';
    l_content += DYN_NAME + '\0';
    l_content += RELA_PLT_NAME + '\0';

    return l_content;
}

}


using namespace ::testing;


class Elf64BitStringTableSectionsBuildingTestSuite : public TestWithParam<std::tuple<int, std::string>>
{
protected:
    void expectStringTableSectionsAreEqual(const StringTableSection<Elf64_Shdr>& p_targetStringTableSection,
                                           const StringTableSection<Elf64_Shdr>& p_expectedStringTableSection);
};

void Elf64BitStringTableSectionsBuildingTestSuite::expectStringTableSectionsAreEqual(const StringTableSection<Elf64_Shdr>& p_targetStringTableSection,
                                                                                     const StringTableSection<Elf64_Shdr>& p_expectedStringTableSection)
{
    auto l_targetSectionHeader { p_targetStringTableSection.getSectionHeader() };
    auto l_expectedSectionHeader { p_expectedStringTableSection.getSectionHeader() };

    EXPECT_EQ(std::addressof(*l_targetSectionHeader), std::addressof(*l_expectedSectionHeader));
    
    
    auto l_targetStringTable { p_targetStringTableSection.getStringTable() };
    auto l_expectedStringTable { p_expectedStringTableSection.getStringTable() };

    
    ASSERT_EQ(l_targetStringTable.size(), l_expectedStringTable.size());

    auto l_targetStringTableIt { l_targetStringTable.begin() };
    auto l_expectedStringTableIt { l_expectedStringTable.begin() };
    auto l_expectedStringTableEnd { l_expectedStringTable.end() };

    for (; l_expectedStringTableIt != l_expectedStringTableEnd; ++l_targetStringTableIt, ++l_expectedStringTableIt)
    {
        EXPECT_EQ(l_targetStringTableIt->first, l_expectedStringTableIt->first);
        EXPECT_EQ(l_targetStringTableIt->second, l_expectedStringTableIt->second);
    }
}


TEST_P(Elf64BitStringTableSectionsBuildingTestSuite, shouldReadEmptyStringTableSectionWhenTheSectionHeaderSizeIsZero)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf64_Shdr>(&STRING_TABLE_SECTION_WITH_ZERO_ELEMENTS) };

    l_elfObjectBuilder.buildStringTableSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetStringTableSection { dynamic_cast<StringTableSection<Elf64_Shdr>&>(*l_sections[0]) };
 
    std::map<int, std::string> l_expectedStringTable {};

    auto l_expectedStringTableSection { StringTableSection<Elf64_Shdr>(l_sectionHeader, l_expectedStringTable) };

    expectStringTableSectionsAreEqual(l_targetStringTableSection, l_expectedStringTableSection);
}

TEST_P(Elf64BitStringTableSectionsBuildingTestSuite, shouldReadStringTableSectionWithAllTenStrings)
{
    auto l_params { GetParam() };
    auto l_endianness { std::get<0>(l_params) };
    auto l_streamContent { std::get<1>(l_params) };

    std::stringstream l_stubStream { l_streamContent };
    TargetMachineInfo l_targetMachineInfo;
    l_targetMachineInfo.endianness = l_endianness;

    NiceMock<NoteDescriptorBuilderMock> l_noteDescriptorBuilderMock;
    ElfSectionBuilder<ElfObjectX64, ElfStructureInfoX64> l_elfObjectBuilder (&l_stubStream, l_targetMachineInfo, l_noteDescriptorBuilderMock);

    auto l_sectionHeader { std::shared_ptr<Elf64_Shdr>(&STRING_TABLE_SECTION_WITH_TEN_ELEMENTS) };

    l_elfObjectBuilder.buildStringTableSection(l_sectionHeader);

    auto l_sections { (l_elfObjectBuilder.getResult()->sections)};
    ASSERT_EQ(l_sections.size(), 1);

    auto l_targetStringTableSection { dynamic_cast<StringTableSection<Elf64_Shdr>&>(*l_sections[0]) };
 
    std::map<int, std::string> l_expectedStringTable
    {
        std::make_pair(MAIN_CPP_INDEX, MAIN_CPP_NAME),
        std::make_pair(STD_COUT_INDEX, STD_COUT_NAME),
        std::make_pair(MAIN_INDEX, MAIN_NAME),
        std::make_pair(DATA_START_INDEX, DATA_START_NAME),
        std::make_pair(BSS_START_INDEX, BSS_START_NAME),
        std::make_pair(MY_CONST_GLOBAL_VAR_INDEX, MY_CONST_GLOBAL_VAR_NAME),
        std::make_pair(GLOBAL_OFFSET_TABLE_INDEX, GLOBAL_OFFSET_TABLE_NAME),
        std::make_pair(MY_FUNC_INT_INDEX, MY_FUNC_INT_NAME),
        std::make_pair(INIT_INDEX, INIT_NAME),
        std::make_pair(GMON_START_INDEX, GMON_START_NAME)
    };

    auto l_expectedStringTableSection { StringTableSection<Elf64_Shdr>(l_sectionHeader, l_expectedStringTable) };

    expectStringTableSectionsAreEqual(l_targetStringTableSection, l_expectedStringTableSection);
}

INSTANTIATE_TEST_SUITE_P(StringTableSectionsBuilding,
                         Elf64BitStringTableSectionsBuildingTestSuite,
                         Values(std::make_tuple(LITTLE_ENDIAN_VALUE, generateFirstStringTableContent()),
                                std::make_tuple(BIG_ENDIAN_VALUE, generateFirstStringTableContent())));

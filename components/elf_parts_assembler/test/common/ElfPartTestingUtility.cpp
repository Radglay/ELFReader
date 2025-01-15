#include "ElfPartTestingUtility.hpp"
#include "ElfPart.hpp"



void ElfPartAssemblerTestSuite::expectElfPartsAreEqual(const ElfPart& p_targetElfPart, const ElfPart& p_expectedElfPart)
{
    expectElfPartTextContentsAreEqual(p_targetElfPart, p_expectedElfPart);

    for (int i = 0; i < p_targetElfPart.fields.size(); ++i)
        expectElfFieldsAreEqual(p_targetElfPart.fields[i], p_expectedElfPart.fields[i]);
}

void ElfPartAssemblerTestSuite::expectElfFieldsAreEqual(const ElfField& p_targetField, const ElfField& p_expectedField)
{
    expectElfFieldTextContentsAreEqual(p_targetField, p_expectedField);

    const auto& l_targetSubFields { p_targetField.fields };
    const auto& l_expectedSubFields { p_expectedField.fields };

    ASSERT_EQ(l_targetSubFields.size(), l_expectedSubFields.size());
    for (int i = 0; i <  l_targetSubFields.size(); ++i)
    {
        expectElfFieldsAreEqual(l_targetSubFields[i], l_expectedSubFields[i]);
    }
}

void ElfPartAssemblerTestSuite::expectElfFieldTextContentsAreEqual(const ElfField& p_targetField, const ElfField& p_expectedField)
{
    EXPECT_EQ(p_targetField.name, p_expectedField.name);
    EXPECT_EQ(p_targetField.type, p_expectedField.type);
    EXPECT_EQ(p_targetField.value, p_expectedField.value);
    EXPECT_EQ(p_targetField.description, p_expectedField.description);
}


void ElfPartAssemblerTestSuite::expectElfPartTextContentsAreEqual(const ElfPart& p_targetElfPart, const ElfPart& p_expectedElfPart)
{
    EXPECT_EQ(p_targetElfPart.name, p_expectedElfPart.name);
    EXPECT_EQ(p_targetElfPart.type, p_expectedElfPart.type);
    EXPECT_EQ(p_targetElfPart.offset, p_expectedElfPart.offset);
    EXPECT_EQ(p_targetElfPart.size, p_expectedElfPart.size);
    EXPECT_EQ(p_targetElfPart.description, p_expectedElfPart.description);
}

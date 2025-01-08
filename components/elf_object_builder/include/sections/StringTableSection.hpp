#pragma once

#include "IElfSection.hpp"
#include <map>
#include <memory>
#include <string>
#include "ElfPart.hpp"
#include "IElfPartAssembler.hpp"


template <typename SectionHeader>
class StringTableSection : public IElfSection<SectionHeader>
{
public:
    StringTableSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::map<int, std::string> p_stringTable)
        : m_sectionHeader { p_sectionHeader }
        , m_stringTable { p_stringTable }
    {}

    ElfPart acceptElfPartAssembler(IElfPartAssembler& p_visitor, const std::string& p_sectionName) override
    {
        // return p_visitor.assembleElfPartFromSection(*this);
    }

    std::shared_ptr<SectionHeader> getSectionHeader() const override
    {
        return m_sectionHeader;
    }

    std::string getStringByOffset(int p_offset)
    {
        return m_stringTable[p_offset];
    }

    const std::map<int, std::string> getStringTable() const
    {
        return m_stringTable;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::map<int, std::string> m_stringTable;
};


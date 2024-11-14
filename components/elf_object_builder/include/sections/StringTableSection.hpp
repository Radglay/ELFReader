#pragma once

#include "IElfSection.hpp"
#include <map>
#include <memory>
#include <string>


template <typename SectionHeader>
class StringTableSection : public IElfSection<SectionHeader>
{
public:
    StringTableSection(std::shared_ptr<SectionHeader> p_sectionHeader, std::map<int, std::string> p_stringTable)
        : m_sectionHeader { p_sectionHeader }
        , m_stringTable { p_stringTable }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

    std::map<int, std::string> getStringTable() const
    {
        return m_stringTable;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
    std::map<int, std::string> m_stringTable;
};


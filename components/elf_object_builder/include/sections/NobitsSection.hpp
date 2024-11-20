#pragma once

#include "IElfSection.hpp"
#include <vector>
#include <memory>


template <typename SectionHeader>
class NobitsSection : public IElfSection<SectionHeader>
{
public:
    NobitsSection(std::shared_ptr<SectionHeader> p_sectionHeader)
        : m_sectionHeader { p_sectionHeader }
    {}

    std::shared_ptr<SectionHeader> getSectionHeader() const
    {
        return m_sectionHeader;
    }

private:
    std::shared_ptr<SectionHeader> m_sectionHeader;
};

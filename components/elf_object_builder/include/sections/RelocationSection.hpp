#pragma once

#include "IElfSection.hpp"
#include <vector>


template <typename SectionHeader, typename Relocation>
class RelocationSection : public IElfSection<SectionHeader>
{
public:
    RelocationSection(SectionHeader* p_sectionHeader, std::vector<Relocation> p_relocationHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_relocationHeaders { p_relocationHeaders }
    {}

    SectionHeader* getSectionHeader()
    {
        return m_sectionHeader;
    }

    std::vector<Relocation> getRelocationHeaders()
    {
        return m_relocationHeaders;
    }

private:
    SectionHeader* m_sectionHeader;
    std::vector<Relocation> m_relocationHeaders;
};

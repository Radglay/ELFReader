#pragma once

#include "IElfSection.hpp"
#include <vector>


template <typename SectionHeader, typename RelocationWithAddend>
class RelocationWithAddendSection : public IElfSection<SectionHeader>
{
public:
    RelocationWithAddendSection(SectionHeader* p_sectionHeader, std::vector<RelocationWithAddend> p_relocationWithAddendHeaders)
        : m_sectionHeader { p_sectionHeader }
        , m_relocationWithAddendHeaders { p_relocationWithAddendHeaders }
    {}

    SectionHeader* getSectionHeader()
    {
        return m_sectionHeader;
    }

    std::vector<RelocationWithAddend> getRelocationWithAddendHeaders()
    {
        return m_relocationWithAddendHeaders;
    }

private:
    SectionHeader* m_sectionHeader;
    std::vector<RelocationWithAddend> m_relocationWithAddendHeaders;
};
